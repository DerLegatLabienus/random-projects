#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <memory>
#include <queue>
#include <deque>

using namespace ::std;

struct Tuple {
	string from;
	string to;
	double conversion_rate;
};


struct Node {
	private:
		string from_currency;
		map<shared_ptr<Node>, double> outgoing;
	public:
		Node(string from) : from_currency(from) {}
		void add_edge(double cost, shared_ptr<Node> dest) {
			outgoing.insert({dest,cost});
		}
		auto get_outgoing() -> decltype(outgoing)& {
			//TODO:: warning returning references
			return outgoing;
		}
		string get_name() {
			return from_currency;
		}
};

class Graph {


	private:
#if defined(BFS) || defined(DFS)
		map<string,bool> is_visited;
#endif
		map<string,shared_ptr<Node>> nodes; //assuming hash backend impl
	public:
		Graph(vector<Tuple> list) {
			for_each(list.begin(),list.end(), [&](Tuple &t){
					add_node(t.from);
					add_node(t.to);
					add_edge(t.from,t.to,t.conversion_rate);
					add_edge(t.to,t.from,1.0/t.conversion_rate);
				});
		}

		void add_node(string n_name) {
			if (nodes.find(n_name) != nodes.end()) {
				nodes.insert({n_name,make_shared<Node>(Node(n_name))});
				is_visited.insert({n_name,false});
			}
		}

		void add_edge(string from, string to, double cost) {
			auto it_from =  nodes.find(from);
			auto it_to   =  nodes.find(to);

			if (it_from != nodes.end() || it_from != nodes.end())
				throw new ::std::invalid_argument("no such node");
			it_from->second->add_edge(cost,it_to->second);
		}

		shared_ptr<Node> get_node_from_string(string s) {
			auto result = nodes.find(s);
			if (result == nodes.end())
				throw new ::std::invalid_argument("no such node");
			return result->second;
		}
#if defined(BFS) || defined(DFS)
		bool is_node_visited(string node_name)  {
			if (is_visited.find(node_name) == is_visited.end())
				throw new ::std::invalid_argument("no such node");
			return is_visited.find(node_name)->second;
		}

		void set_node_as_visited(string node_name) {
			if (is_visited.find(node_name) == is_visited.end())
				throw new ::std::invalid_argument("no such node");
			is_visited.find(node_name)->second = true;
		}
#endif
		void reset_graph() {
			for (auto pair : is_visited) {
				pair.second = false;
			}
		}

};



class CurrencyConvertor { 

	public:
	CurrencyConvertor(Graph gr) : g(move(gr)){}
	
	double convert(string s, string d) {
		using DestCost = pair<shared_ptr<Node>,double>;
		shared_ptr<Node> start = g.get_node_from_string(s);
		deque<DestCost> ptr;
#ifdef BFS
		ptr.push_back(make_pair(start,1.0));
#elif DFS
		ptr.push_front(make_pair(start,1.0));
#else
#error "BFS or DFS Symbol must be defined"
#endif
		while (!ptr.empty()) {
			auto cur = ptr.front(); ptr.pop_front();
			shared_ptr<Node> cur_node = cur.first;
			double accumlated_conversion = cur.second;
			g.set_node_as_visited(cur_node->get_name());
			auto neighbors = cur_node->get_outgoing();
			if (cur_node->get_name() == d) {
				g.reset_graph();
				return accumlated_conversion;
			}

			for (DestCost e : neighbors) {
				shared_ptr<Node> nei = e.first;
				string nei_name = nei->get_name();
				double nei_cost = e.second;
				double cost = nei_cost*accumlated_conversion;
				if (nei_name == s) continue;
				if (g.is_node_visited(nei_name)) continue;
#ifdef BFS
				ptr.push_back(make_pair(nei,cost));
#elif DFS
				ptr.push_front(make_pair(nei,cost));
#else
#error "BFS or DFS Symbol must be defined"
#endif

			}
		}
		g.reset_graph();
		return -1;

	}
	private:
		Graph g;

};


int main(int argc, char **argv) {

	vector<Tuple> v;
	Graph g(v);
	g.add_node("EUR");
	g.add_node("DOLLAR");
	g.add_edge("EUR","DOLLAR",1.7);
	CurrencyConvertor c(g);

	cout << " result is : " << c.convert("EUR","DOLLAR") << endl;

	return 0;
}
