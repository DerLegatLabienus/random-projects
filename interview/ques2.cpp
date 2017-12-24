nclude <string>
#include <set>

template <int N>
int max_subsequence(string s) {
	   
	map<char,int> b;
	int count = N;
	int max   = N;
	for (int i = N; i < s.length(); i++) {
		auto it = b.find(s[i]);
		if (b.size() < N) {
			if (it == b.end())
				auto p = b.insert({s[i],1});
			else 
				it->second++;
			max = ++count;
			continue;
		}
		if (it != b.end()) {
			it->second++;
			count++; 
		} else {
			b.insert(make_pair(s[i],1));
			bool not_ready= true;
			while (not_ready) {
				char to_remove = s[i-count];
				auto it = b.find(to_remove);
				it->second--;
				if (!it->second) {
					b.remove(to_remove);
				}
				count--;
				if (b.size() == N) not_ready = false;
			} 
			max = ::std::max(count,max);
		}
	}

	return max;

		  
}

int main() {
	  
	  
	  return 1;
}

