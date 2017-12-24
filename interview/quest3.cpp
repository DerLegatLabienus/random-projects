#include <queue>



#define K 10
#define N 3000

constexpr int compare( pair<int,int> &lhs, pair<int,int>& rhs) {


	//compare base on the first value in pair
	return lhs.first < hs.first;

}
void print_sorted(int m[K][N]) {

	using min_heap=priority_queue<pair<int,int>,vector<pair<int,int>>,compare>;

	min_heap h;
	int index[K] = {0};

	for (int i = 0; i < k; i++) {
		h.insert(make_pair(m[ i ][ index[i] ]),i);
	}

	while (h.size()) {
		pair<int,int> p = h.top(); h.pop();
		cout << p->first << " ";
		int arr_i = p->second;
		if (++index[arr_i] < N) {
			h.insert(make_pair(m[arr_i][index[arr_i]],arr_i);
		}
	}


}
