package Databases;

import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import Moovit.BasicTypes.LineETA;
import Moovit.BasicTypes.LineNumber;
import Moovit.BasicTypes.StopETA;

//maps between StopId to relevant lineETA
public class SynchronizedHashMapStops implements Map<Integer, List<LineETA>>{
	private HashMap<Integer,List<LineETA>> rawDb;
	private Map<Integer,List<LineETA>> m = Collections.synchronizedMap(rawDb);
	private static SynchronizedHashMapStops singelton = null; 
	
	
	private SynchronizedHashMapStops() {
		// TODO Auto-generated constructor stub
	}
	
	public static SynchronizedHashMapStops getObject() {
		if (singelton == null) {
			singelton = new SynchronizedHashMapStops();
		}
		return singelton;
	}
	
	@Override
	public void clear() {
		m.clear();
		
	}
	@Override
	public boolean containsKey(Object key) {
		return m.containsKey(key);
	}
	@Override
	public boolean containsValue(Object value) {
		return m.containsValue(value);
	}
	@Override
	public Set<java.util.Map.Entry<Integer, List<LineETA>>> entrySet() {
		return m.entrySet();
	}
	@Override
	public List<LineETA> get(Object key) {
		return m.get(key);
	}
	@Override
	public boolean isEmpty() {
		return m.isEmpty();
	}
	@Override
	public Set<Integer> keySet() {
		return m.keySet();
	}
	@Override
	public List<LineETA> put(Integer key, List<LineETA> value) {
		return m.put(key, value);
	}
	
	public List<LineETA> put(Integer key, StopETA stop, LineNumber value) {
		List<LineETA> ourList = m.get(key);
		LineETA newData = new LineETA(value.getLineNumber(),stop.getNextETA());
		//TODO:: remove old line in this queue
		ourList.remove(newData);
		ourList.add(newData);
		return m.put(key, ourList);
	
	}
	@Override
	public void putAll(Map<? extends Integer, ? extends List<LineETA>> other) {
		m.putAll(other);
	}
	@Override
	public List<LineETA> remove(Object key) {
		return m.remove(key);
	}
	@Override
	public int size() {
		return m.size();
	}
	@Override
	public Collection<List<LineETA>> values() {
		return m.values();
	}
	   
	   

}
package Databases;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import Moovit.BasicTypes.LineETA;
import Moovit.BasicTypes.LineNumber;
import Moovit.BasicTypes.StopETA;
import Moovit.BasicTypes.Trip;

//maps between StopId to relevant lineETA
public class TripManager {
	private HashMap<LineNumber,List<Trip>> rawTripDb;
	private Map<LineNumber,List<Trip>> mTripDB = Collections.synchronizedMap(rawTripDb);
	
	private HashMap<LineNumber,LocalDateTime> rawTimeDb;
	private Map<LineNumber,LocalDateTime> mTimeDB = Collections.synchronizedMap(rawTimeDb);
	
	private FileWriter fileWriter;

	private static TripManager singelton = null; 
	
	private TripManager() {
		try {
			fileWriter = new FileWriter("TripManagerOutput");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	   
	public static TripManager TripManagerObject() {
		if (singelton == null) {
			singelton = new TripManager();
		}
		
		return singelton;
	}
	
	public LocalDateTime getLastUpdateTime(LineNumber line) {
		return mTimeDB.get(line);
	}
	
	public List<Trip> getLastTripData(LineNumber line) {
		return mTripDB.get(line);
	}
	
	public void updateData(LineNumber line, LocalDateTime lastDataFreshness, List<Trip> newLineTrip) {
		mTimeDB.put(line, lastDataFreshness);
		mTripDB.put(line, newLineTrip);
	}
	
	
	//because IO is heavy, should be queued in a thread and not from TripManager itself.
	public void outputToCSV(LineNumber line) {
		if (fileWriter!=null) {
			List<Trip> trips = mTripDB.get(line);
			for (Trip trip : trips) {
				String string = "TRIPID: " + trip.getId() +
						  " STOPID: " + trip.getNextStop() + 
						  " TIMEOFUPDATE:  " + trip.getTimeLastCheck() + 
						  " ETA: " + trip.getNextStop();
				
				try {
					fileWriter.append(string);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}	
			}
			
			
		}
	}

}
package Databases;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.temporal.ChronoUnit;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.Stack;
import java.util.concurrent.TimeUnit;

import Moovit.BasicTypes.LineETA;
import Moovit.BasicTypes.LineNumber;
import Moovit.BasicTypes.StopETA;
import Moovit.BasicTypes.StopInterval;
import Moovit.BasicTypes.Trip;
import Moovit.PollingModule.PollerTaskResult;

public class UpdateLocalDBTask implements Runnable {

	private List<StopETA> dataETAtoProcess;

	// ... not necessary, since we assume its constant data. we won't process it
	// however.
	private List<StopInterval> dataIntevalsToProcess;
	private LocalDateTime lastUpdateTime;
	private LineNumber lineId;

	public UpdateLocalDBTask(PollerTaskResult result) {
		dataETAtoProcess = result.stops;
		this.lineId = result.line;
		this.lastUpdateTime = result.currUpdateTime;
	}

	@Override
	public void run() {

		// index by StopId
		for (StopETA s : dataETAtoProcess) {
			// this put actually removes old data and insert modified list with
			// the new line data
			// closed deal.
			SynchronizedHashMapStops.getObject().put(
					new Integer(s.getStopId()), s, lineId);

		}

		// index by LineNumber
		List<StopETA> oldData = SynchronizedHashMapLines.getObject().put(
				lineId, dataETAtoProcess);
		// first time updating - we should also create the Trip init data
		if (oldData == null) {
			TripManager.TripManagerObject().updateData(lineId,
					this.lastUpdateTime,
					generateTripBasedOnETA(dataETAtoProcess, true));
			TripManager.TripManagerObject().outputToCSV(lineId);
			return;
		}

		// something went really wrong.
		if (oldData.size() <= 0) {
			System.err.println("Old data is empty on non-initailiazation phase");
			return;
		}

		// TODO:: consider if Trip manager should be on the same thread. Both
		// updates Databases, but Trips adds some analyze logic that maybe
		// should be separated.

		// old data to derive Expectations from
		List<Trip> oldTripDataForEstimation = TripManager.TripManagerObject()
				.getLastTripData(lineId);

		LocalDateTime oldtimeUpdate = TripManager.TripManagerObject()
				.getLastUpdateTime(lineId);
		// new data
		List<Trip> newData = generateTripBasedOnETA(dataETAtoProcess, false);

		List<ArrayList<Trip>> realData = generateCountPerInterval(newData,
				dataIntevalsToProcess);
		List<ArrayList<Trip>> expectedData = generateCountPerInterval(
				oldTripDataForEstimation, dataIntevalsToProcess);

		// we will follow simple matching algorithm
		// #1 scan from end to start. assign expected to finish trips as done -
		// save FIFO.
		// #2 in each interval -> assign local reals wit the local expected.
		// remaining reals are assigned with over expected trips.
		// #3 if there are more expected then reals -> push them
		ListIterator<ArrayList<Trip>> real_it = realData.listIterator(realData
				.size());
		ListIterator<ArrayList<Trip>> expected_it = expectedData
				.listIterator(expectedData.size());

		Stack<Trip> overExpectedTrips = new Stack<>();

		// #1
		expected_it.previous().forEach(trip -> overExpectedTrips.push(trip));

		
		//iterate over the same intervals
		while (real_it.hasPrevious() && expected_it.hasPrevious()) {

			ArrayList<Trip> expec_elem = expected_it.previous();
			ArrayList<Trip> real_elem = real_it.previous();

			//#2 handle real trips. 
			for (Trip real_t : real_elem) {
				if (!expec_elem.isEmpty()) {
					Trip expected_trip = expec_elem.remove(0);
					real_t.mergeTrips(expected_trip);
				} else if (!overExpectedTrips.isEmpty()) {
					real_t.mergeTrips(overExpectedTrips.pop());

				} else {
					real_t.validiateTrip();
				}

			}
			//#3 remaining expected trip push to stack
			for (Trip expected_Trip : expec_elem) {
				overExpectedTrips.push(expected_Trip);
			}
		}
		
		//flat the list
		List<Trip> dbFormat = new ArrayList<>();
		realData.forEach( list -> dbFormat.addAll(list));
		
		TripManager.TripManagerObject().updateData(lineId, lastUpdateTime, dbFormat);
	}



	// isTripsAreReal - means trip should acquire a unique id (they are not old
	// trips)
	private List<Trip> generateTripBasedOnETA(List<StopETA> data,
			boolean tripAllocNewID) {

		ArrayList<Trip> result = new ArrayList<Trip>();
		StopETA prevStop = data.get(0);
		data.remove(0);

		// assuming both data lengths are same
		for (int i = 0; i < data.size() + 1; i++) {
			StopInterval interval = dataIntevalsToProcess.get(i);
			StopETA currStop = data.get(i);

			// if ETA2 < ETA1 + INTERVALINBETWEEN
			if (currStop.getNextETA().compareTo(
					prevStop.getNextETA().plus(interval.intervalTimeToNextStop,
							ChronoUnit.MINUTES)) < 0) {
				Trip trip = new Trip(tripAllocNewID);
				trip.setTimeLastCheck(lastUpdateTime);
				trip.setNextStop(currStop);
				result.add(trip);

			}
		}

		return result;

	}

	private List<List<Trip>> generateExpectedData(List<Trip> oldData,
			LocalDateTime lastUpdate) {
		ZoneId zoneId = ZoneId.systemDefault();
		long epoch = lastUpdate.atZone(zoneId).toEpochSecond();
		long epochNow = LocalDateTime.now().atZone(zoneId).toEpochSecond();
		long offsetInMinutes = Math.round((epochNow - epoch) / 60) + 1;

		ArrayList<List<Trip>> result = new ArrayList<>(
				dataIntevalsToProcess.size() + 1);

		result.stream().map(p -> new ArrayList<Trip>()); // init data

		// for each trip, count how many intervals can he skip according the
		// offset from lastUpdate
		// loop the intervals until
		Iterator<Trip> curTripIterator = oldData.iterator();
		Trip currTrip = curTripIterator.next();
		for (int i = 0; i < dataIntevalsToProcess.size(); i++) {
			StopInterval interval = dataIntevalsToProcess.get(i);
			while (currTrip.getNextStop().getStopId() == (interval.stopId)) {
				long offset = offsetInMinutes;
				int j = 0;
				for (j = 0; j + i < dataIntevalsToProcess.size(); j++) {
					interval = dataIntevalsToProcess.get(i + j);
					if (offset > 0) {
						offset -= interval.intervalTimeToNextStop;
						continue;
					}

					List<Trip> expectedLocation = result.remove(i + j - 1);
					currTrip.setNextStop(currTrip.getNextStop());
					expectedLocation.add(currTrip);
					result.add(expectedLocation);
				}

				// line is expected to be done
				if (j + i == dataIntevalsToProcess.size()) {
					List<Trip> expectedLocation = result.remove(i + j);
					currTrip.setNextStop(new StopETA(StopETA.END_BUSSTOP, LocalDateTime.MAX));
					expectedLocation.add(currTrip);

					// all next trips are expected to finish.
					while (curTripIterator.hasNext()) {
						currTrip.setNextStop(new StopETA(StopETA.END_BUSSTOP, LocalDateTime.MAX));
						expectedLocation.add(currTrip);
					}
					result.add(expectedLocation);

				}

				// check if we have more trips
				if (!curTripIterator.hasNext()) {
					// all new location expectations are in bounds
					return result;
				} else {
					currTrip = curTripIterator.next();
				}

			}

		}

		return result;

	}

	// get trip of list and place them in intervals.
	private List<ArrayList<Trip>> generateCountPerInterval(List<Trip> list,
			List<StopInterval> intervals) {

		List<ArrayList<Trip>> tripCountPerInterval = new ArrayList<>(
				intervals.size());

		tripCountPerInterval.stream().map(p -> new ArrayList<Trip>()); // init
																		// data

		Iterator<Trip> tripListIterator = list.iterator();
		Trip currentTrip = tripListIterator.next();

		for (int i = 0; i < intervals.size() && tripListIterator.hasNext(); i++) {
			while (currentTrip.getNextStop().getStopId() == intervals.get(i).stopId) {

				// remove the list, augment with current trip, and reattach list
				// I assume there is much cleaner way to so...
				ArrayList<Trip> currentTripList = tripCountPerInterval
						.remove(i);
				currentTripList.add(currentTrip);
				currentTrip = tripListIterator.next();

				tripCountPerInterval.add(currentTripList);

			}
		}

		return tripCountPerInterval;

	}
}

	
//	Another try to assign all first new reals before the first expected as validated.
//	I think the solution I got is much better.
//	ArrayList<Trip> expec_elem = expected_it.previous();
//
//	while (expected_it.hasNext() && expec_elem.isEmpty()) {
//		expec_elem = expected_it.next();
//	}
//
//	if (expec_elem != null && !expec_elem.isEmpty()) {
//		firstExpectedStopid = expec_elem.get(0).getNextStop();
//	}
//
//	ArrayList<Trip> real_elem = real_it.next();
//	while (real_it.hasNext()) {
//		if (!real_elem.isEmpty()) {
//			if (real_elem.get(0).getNextStop() != firstExpectedStopid)
//				for (Trip s : real_elem) {
//					s.validiateTrip();
//				}
//		}
//	}

