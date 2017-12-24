#include <stdio>
#include <algorithm>
#include <map>
#include <set>

struct Tuple {
	string from;
	string to;
	double convesion_rate;
};


struct Node {
	public:
		Node(string from) : from_currecy(from) {}
		void add_edge(double cost, Node& dest) {
			outgoing.insert(make_pair(dest,cost));
		}
		int compare(Node& node) {
			//assert(node != NULL)
			return this.from_currency < node.from_currency;

		}
		get_outgoing() -> decltype(outgoing)& {
			//warning returning references
			return outgoing;
		}
		bool is_visited; = false; // TODO:: to be wrapped
	private:
		string from_currecy;
		map<Node, double> outgoing;
};

class Graph {

	public:
		Graph(vector<Tuple> list) {
			for_each(list.begin(),list.end(), [&](Tuple &t){
					add_node(Node(t.from,t.to,t.conversion_rate);
					add_node(Node(t.to,t.from,1.0/t.conversion_rate);
					});
		}

		void add_node(Node& n) {
			nodes.insert(make_pair(n.from_currency,n);

		}

		Node *get_node_from_string(string s) {

			//assume s exists as a node
			return &*nodes.find(s);
		}

	private:
		map<string,Node> nodes; //assuming hash_set
};



class CurrencyConvertor { 

	CurrencyConvertor(Graph gr) : g(move(gr)){}
	
	double convert(string s, string d) {
		Node *start = g.get_node_from_string(s);
		queue<pair<Node *,double> ptr;
		ptr.push(make_pair(start,1));
		while (!ptr.empty()) {
			auto cur = ptr.top(); ptr.pop();
			Node * cur_node = cur->first;
			double accumlated_conversion = cur->second;

			cur_node.first->is_visited = true;
			auto neighbors = cur->get_outgoing();
			
			if (cur_node.from_currency == d) {
				return accumlated_conversion;
			}
			for (pair<Node,double> e : neighbors) {

				Node *n = &e.first;
				double cost = e.second*accumlated_conversion;

				if (n.is_visited)
					continue;
				ptr.push(make_pair(&n,cost);
			}
			return -1;
		}
	}

	private:
		Graph g;

};


int main(int argc, char **argv) {


}
