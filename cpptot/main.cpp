/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;


void swap(vector<int>& v, int i1, int i2) {
	/*v[i1] = v[i1] ^ v[i2];
	v[i1] = v[i1] ^ v[i2];
	v[i2] = v[i1] ^ v[i2];*/

	int tmp = v[i1];
	v[i1] = v[i2];
	v[i2] = tmp;
}

inline void print_vector(vector<int>& sorted) {
		cout << "[ ";
		for (int i : sorted) {
			cout << i << " ";
		}
		cout << "]" << endl;
}


vector<int> quicksort(vector<int> v) {

	if (v.empty() || v.size() == 1)
		return v;

	/*std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0, v.size()-1);
	int pivot_i = dis(gen);*/
	int pivot_i = v.size() - 1;

	vector<int> v_low, v_high, v_equal;
	v_low.reserve(v.size());
	v_high.reserve(v.size());
	v_equal.reserve(2);
	for (int val : v) {
		if (val < v[pivot_i])
			v_low.push_back(val);
		else if (val > v[pivot_i])
			v_high.push_back(val);
		else 
			v_equal.push_back(val);
	}

	v.reserve(v.size()+v_high.size()+v_equal.size());
	v_low = move(quicksort(v_low));
	v.swap(v_low);
	v_high = std::move(quicksort(v_high));
	v.insert(v.end(),v_equal.begin(),v_equal.end());
	v.insert(v.end(),v_high.begin(),v_high.end());

	return v;
	
}


/*doesn't sort end elements*/
vector<int> mergesort_iterative(vector<int> vec) {
	bool is_not_sorted = true;
	for (int width = 2; is_not_sorted ; width=2*width) {
		if (width >= vec.size()) {
			is_not_sorted = false;
		}
		vector<int> v_sorted;
		v_sorted.reserve((ssize_t)(width));
		for (int left=0; left < vec.size(); left+=width) {

			int left_to_mid = left;
			const int right = (left+width > vec.size())? vec.size() : left+width;
			const int mid = left + (right-left)/2;
			int mid_to_right =  mid;

		//	cout << left << " "<<  mid << " " << right << endl;

			while (left_to_mid < mid || mid_to_right < right) {
					if (mid_to_right == right) 
						v_sorted.push_back(vec[left_to_mid++]);
					else if (left_to_mid == mid) 
						v_sorted.push_back(vec[mid_to_right++]);
					else if (vec[left_to_mid] < vec[mid_to_right])
						v_sorted.push_back(vec[left_to_mid++]);
					else if (vec[left_to_mid] >= vec[mid_to_right])
						v_sorted.push_back(vec[mid_to_right++]);
			}
			for (int j = left; j < right; j++)
				vec[j] = v_sorted[j-left];
			v_sorted.clear();
	 	}

	}
	return vec;
}
vector<int> mergesort_recursive(const vector<int> vec) {
	int n = vec.size();

	//if (n < CONST)
	// perform insertation or some other trivial sort to optimize merge sort

	if (n < 2) {
		return vec;
	}

	auto v1 = mergesort_recursive(vector<int>(vec.begin(),vec.begin()+n/2));
	auto v2 = mergesort_recursive(vector<int>(vec.begin()+n/2,vec.end()));

	vector<int> v_sorted;
	v_sorted.reserve((ssize_t)(v1.size()+v2.size()));
	auto v1_it = v1.begin();
	auto v2_it = v2.begin();
	while (v1_it != v1.end() || v2_it != v2.end()) {
		if (v1_it == v1.end()) {
			while (v2_it != v2.end()) {
				v_sorted.push_back(*v2_it);
				v2_it++;
			}
			break;
		}
		if (v2_it == v2.end()) {
			while (v1_it != v1.end()) {
				v_sorted.push_back(*v1_it);
				v1_it++;
			}
			break;
		}
		if (*v2_it < *v1_it) {
			v_sorted.push_back(*v2_it);
			v2_it++;
		} else {
			v_sorted.push_back(*v1_it);
			v1_it++;
		}
	}
	return std::move(v_sorted);
}

decltype(mergesort_iterative) *mergesort = &mergesort_iterative;


vector<int> heapsort(vector<int> vec) {

	auto heapifyDown = [](vector<int>& heap, int end_i) {
		int index = 0;
		while (2*index+2 < end_i) {
			int left_i = 2*index+1;
			int right_i= 2*index+2;
			int left_child  = heap[left_i];
			int right_child = heap[right_i];
			if (heap[index] < right_child || heap[index] < left_child) {
				if (right_child > left_child) {
					swap(heap,index,right_i);
					index = right_i;
				}
				else  {
					swap(heap,index,left_i);
					index = left_i;
				}
			} else break;
		}
		if (2*index + 1 < end_i) {
			int left_i = 2*index + 1;
			int left_child = heap[left_i];
			if (heap[index] < left_child) 
				swap(heap,index,left_i);
		}
	};
	//buid heap
	vector<int> heap(0);
	for (int i = 0; i < vec.size(); i++) {
		heap.push_back(vec[i]);
		bool isNotHeap = true;
		int parent_j = i, j = i;
		while (isNotHeap) {
			parent_j  = (j-1)/2;
			if (parent_j >= 0 && heap[parent_j] < heap[j]) {
				swap(heap,parent_j,j);
				j = parent_j;
			} else {
				isNotHeap = false;
			}
		}
	}
	
	vector<int> res(vec.size());
	for (int i = heap.size() - 1; i >= 0; i--) {
		swap(heap,0,i);
		res[i] = heap[i]; 
		heapifyDown(heap,i);
	}

	return res;

}

std::vector<int> radix_sort_lsb(std::vector<int> vec) {
    
    int alephbet_size = 10;
    
    std::vector<vector<int> > bucket1(alephbet_size);
    std::vector<vector<int> > bucket2(alephbet_size);
    vector<vector<vector<int> > > buckets{bucket1,bucket2};
    
    short src = 0, dest = 1;
    //init first bucket
    for (int e : vec) {
        buckets[src][ 0 ].push_back(e); 
    }

    // double buckets are used, one is write active and the other is being read.
    // the readed bucket is being invalidated in the end of iteration
    // the writed bucket becomes the the "readed" in the next iter.
    // do so until keys are being zero. assuming we don't have max key size will cost us extra iteration to find out foundNonZero value.
    bool foundNonZero = true;
    int divisor = 10;
    int p_divisor = 1;
    while  (foundNonZero) {
        foundNonZero = false;
        vector<vector<int> >& source_bucket = buckets[src];
        for (vector<int>& elements : source_bucket) {
            for (int e : elements) {
                int lsb = (e / p_divisor) % divisor;
                buckets[dest][ lsb ].push_back(e);
                if (lsb != 0) foundNonZero = true;
            }
            elements.clear();
        }

        //toggle buckets, query MSBier digit.
        src = dest, dest = (dest + 1) % 2, divisor = 10, p_divisor *= 10;
    }
    
    int addition_i = 0;
    vector<vector<int> >& b = buckets[src];
    for (vector<int>& elements : b) {
        for (int element : elements) {
                vec[addition_i++] = element;
	    }
    }
    return vec;
}

extern "C" vector<int> bubble_sort(vector<int> vec) {
    bool sorted = true;
    int n = vec.size();
    do {
        sorted = true;
        auto it = vec.begin();
        auto it_next = std::next(it,1);
        while (it_next != vec.end()) {
            if (*it > *it_next) {
                auto tmp = *it;
                *it = *it_next;
                *it_next = tmp;
                sorted = false;
            }
            it = std::next(it,1);
            it_next = std::next(it_next,1);
        }
    } while (!sorted);
    
    return vec;
}

extern "C" vector<int> selection_sort(vector<int> vec) {
    int n = vec.size();
    for (int i = 0; i < n; i++) {
        int min = i;
        bool to_swap = false;
        auto data = vec.data();
        for (int j = i+1; j < n; j++) {
            if (vec.at(min) > vec.at(j)) {
                min = j;
                to_swap = true;
                // int tmp = data[i];
                // data[i] = data[j];
                // data[j] = tmp;
            }
        }
        if (to_swap) {
            int tmp = data[i];
            data[i] = data[min];
            data[min] = tmp;
        }
    }
    return vec;
     
}

extern "C" vector<int> insert_sort(vector<int> vec) {
    int n = vec.size();
    for (int i = 1; i < n; i++) {
        int swapEle = i;
        for (int j = i-1; j >= 0 ; j--) {
            if (vec[swapEle] < vec[j]) {
                int tmp = vec[swapEle];
                vec[swapEle] = vec[j];
                vec[j] = tmp;
                swapEle = j;
            }
        }
    }
    
    return vec;
    
}

extern "C" vector<int> counting_sort(vector<int> vec) {
    //assuming range starts from 0...n or simplee k==n
    typename vector<int>::iterator it = std::max_element(vec.begin(),vec.end());
    int k = *it + 1;
    int counters[k] ;
    std::memset(counters,0,k*sizeof(decltype(counters[0])));
    for (int e : vec) {
        counters[e]++;
    }
    vector<int> results(k);
    int j = 0;
    for (int i = 0; i < k; i++) {
        int p_j = j;
        for (; j < p_j + counters[i]; j++) {
            results[j] = i;
        }
    }
    results.resize(j);
    return results;
}

void test() {
    
    vector<int> (*sorters[])(vector<int>)  = {quicksort, mergesort, radix_sort_lsb, bubble_sort, insert_sort, selection_sort,  heapsort, counting_sort}; 
    
    //vector<int> v{50,39,22,3874,672,03734,27355,109184,28464,287506,26240,11,9,87,23,3423,2312312,1290,147,74,11575,7157,5417,177687809,24,1,2,123,45,154,6575,8,8,69,87,85,7,0,6,54123,123,5,7,8,76,6,989879,43,63,63,622,513,34,123,564,123,937,862,983,275,9,0,11}; 

    vector<int> v{0,1,2,3,4,5,2432,32431,9237,371,837,123,6635,981,22,111,123,43,90,91,65,69,32};
    for (auto pFunc : sorters) {
        try {
            auto t1 = std::chrono::system_clock::now();
            vector<int> sorted = pFunc(v);
            auto t2 = std::chrono::system_clock::now();
            cout << "[ ";
            for (int i : sorted) {
                cout << i << " ";
            }
            cout << "] in " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count() << " NANOS "  << endl;
        } catch (std::exception& e) {
            cerr << " failed sort with:" << e.what() << endl;
            continue;
        }
    }
}


//cout << "<" << l << "[" << m << "]" << r << ">" << endl;
bool binarySearch(const int arr[], int n, int v )
{
    int l = 0, r = n-1, m;
    int runs = 0;
    while (l <= r) {
        m = l + (r-l)/2; // [l=0,1,r=2] -> m will 1. [l=0,1,2,r=3]-> m will be 2. [0,1]-> m will be 1 
        if (v < arr[m]) r = m - 1;
        else if ( v > arr[m] ) l = m + 1;
        else break;
    }
    return arr[m] == v;
}

void testBinarSearch() {
       int a1[] = {0,1,2,3,4,5};
    bool success = true;
    (cout<< (success &= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),0))) << endl;
    (cout<< (success &= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),1))) << endl;
    (cout<< (success &= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),2))) << endl;
    (cout<< (success &= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),3))) << endl;
    (cout<< (success &= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),4))) << endl;
//    (cout<< (success &= binarySearch(new int[]{0,1,2,3,4,5},sizeof(a1)/sizeof(a1[0]),5))) << endl;

    cout << (success? "SUPER" : "NOT GOOD") << endl;
    success = true;
    (cout<< (success |= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),-1))) << endl;
    (cout<< (success |= binarySearch(a1,sizeof(a1)/sizeof(a1[0]),10))) << endl;
    cout << (success? "SUPER" : "NOT GOOD") << endl;
}



int main()
{
	

	testBinarSearch();

	test();

    return 0;
}



