#include <string>
#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <algorithm>
#include <limits>

using namespace ::std;

struct Node {
	private:
	static int ID;
	int id;
	string currency;

	public:
		Node(string currency); 
		string get_descr() const;
		void   set_descr(string name);
		int get_id() const;
};

int Node::ID = 0;
Node::Node(string currency) : id(++ID), currency(currency){}
string Node::get_descr() const { return currency;}
void Node::set_descr(string name) { currency = name;}
int Node::get_id() const {return id;}

class Graph {

	using InputEdge = tuple<string,string,double>;
	using weight_t = double;
	private:
		map<string,shared_ptr<Node>> name_to_node;
		map<int,shared_ptr<Node>> node_id_to_node;
		vector<vector<weight_t>> adj;
		map<int,bool> bfs_is_visited;
		map<int,weight_t> dijkstra_distance;

	public:
		Graph(vector<string> node_names, vector<InputEdge> edges);
		void add_node(string name);
		void add_edge(int id_from, int id_to, weight_t w);
		void bfs(int id_from, int id_to);
		void bfs(string name_from, string name_to);
		void dfs(int id_from, int id_to);
		void dfs(string name_from, string name_to);
		void dijkstra(int id_from, int id_to);
		void dijksra(string name_from, string name_to);

};

Graph::Graph(vector<string> node_names, vector<InputEdge> edges) {

	for_each(node_names.begin(),node_names.end(),
			[&](string name) {
				add_node(name);
	});
	for_each(edges.begin(),edges.end(),
			[&](InputEdge edge) {
			string from_name, to_name;
			double weight;
			shared_ptr<Node> from_id, to_id;
			std::tie(from_name, to_name, weight) = edge;
			std::tie(ignore,from_id) = *name_to_node.find(from_name);
			std::tie(ignore,to_id)   = *name_to_node.find(to_name);

	});
	adj.resize(1000,vector<weight_t>(0)); // assuming large enough

}

void Graph::add_node(string name) {
	if (name_to_node.find(name) != name_to_node.end())
		return;
	shared_ptr<Node> n(new Node(name));
	int node_id = n->get_id();
	//maps init
	name_to_node.insert({name,n});
	node_id_to_node.insert({node_id,n});
	bfs_is_visited.insert({node_id,false});
	dijkstra_distance.insert(make_pair(node_id,numeric_limits<weight_t>::max()));
}

void Graph::add_edge(int id_from, int id_to,weight_t w) {
	auto it_from = node_id_to_node.find(id_from);
	auto it_to = node_id_to_node.find(id_to);
	auto end = node_id_to_node.end();

	if (it_from == end || it_to == end) {
		adj[id_from][id_to] = w;
		adj[id_to][id_from] = w;
	}
}


int main() {
	
}

