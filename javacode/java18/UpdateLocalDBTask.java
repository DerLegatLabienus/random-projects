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
public class SynchronizedHashMapLines implements Map<LineNumber, List<StopETA>>{
	private HashMap<LineNumber,List<StopETA>> rawDb;
	private Map<LineNumber,List<StopETA>> m = Collections.synchronizedMap(rawDb);
	private static SynchronizedHashMapLines singelton = null; 
	
	
	private SynchronizedHashMapLines() {
		// TODO Auto-generated constructor stub
	}
	
	public static SynchronizedHashMapLines getObject() {
		if (singelton == null) {
			singelton = new SynchronizedHashMapLines();
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
	public Set<java.util.Map.Entry<LineNumber, List<StopETA>>> entrySet() {
		return m.entrySet();
	}
	@Override
	public List<StopETA> get(Object key) {
		return m.get(key);
	}
	@Override
	public boolean isEmpty() {
		return m.isEmpty();
	}
	@Override
	public Set<LineNumber> keySet() {
		return m.keySet();
	}
	@Override
	public List<StopETA> put(LineNumber key, List<StopETA> value) {
		return m.put(key, value);
	}
	@Override
	public void putAll(Map<? extends LineNumber, ? extends List<StopETA>> other) {
		m.putAll(other);
	}
	@Override
	public List<StopETA> remove(Object key) {
		return m.remove(key);
	}
	@Override
	public int size() {
		return m.size();
	}
	@Override
	public Collection<List<StopETA>> values() {
		return m.values();
	}
	   
	   

}
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

