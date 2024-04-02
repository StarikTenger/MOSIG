#include <iostream>
#include <vector>
#include <math.h>
#include <set>
#include <deque>

using namespace std;

struct Point {
	double x = 0;
	double y = 0;
};

struct Connection {
	int target; // Vertex number
	int index = 0; // Unique number of Connection in the graph (used for rendering)
};

// We need operator< for multiset of connections
bool operator<(const Connection& l, const Connection& r) {
	return l.target < r.target;
}

// Eucledean distance between two points
double distance(const Point& l, const Point& r) {
	double dx = l.x - r.x;
	double dy = l.y - r.y;
	return sqrt(dx * dx + dy * dy);
}

// Graph structure. Not the optimal one from architecture point of view, but it works. 
struct Graph {
	vector<Point> points;
	vector<multiset<Connection>> connections;
	int con_counter = 0;

	Graph() {}
	Graph(int size) {
		points = vector<Point>(size);
		connections = vector<multiset<Connection>>(size);
	}

	void add_connection(int i, int j) {
		connections[i].insert({j, con_counter++});
	}

	int size() const {
		return points.size();
	}
};

// Using Prim algorithm to construct minimal spanning tree
Graph min_spanning_tree(const vector<Point>& points) {
	Graph graph(points.size());
	graph.points = points;

	set<int> connected;
	connected.insert(0);

	for (int k = 0; k < points.size() - 1; k++) {
		int closest_in = 0;
		int closest_out = 0;
		double dist_min = 1e9;
		for (int i : connected) {
			for (int j = 0; j < points.size(); j++) {
				if (connected.find(j) != connected.end()) {
					continue;
				}
				if (distance(points[i], points[j]) < dist_min) {
					closest_in = i;
					closest_out = j;
					dist_min = distance(points[i], points[j]);
				}
			}
			
		}
		graph.add_connection(closest_in, closest_out);
		graph.add_connection(closest_out, closest_in);
		connected.insert(closest_out);
	}

	return graph;
}

// Eulers path finding algorithm modified for reducing repetitions
Graph eulerian_path(Graph graph, bool reduce_repetitions) {
	deque<int> stack;
	stack.push_back(0);

	vector<int> order;
	vector<bool> traversed(graph.size());

	while(stack.size()) {
		int v = stack.back();
		if (graph.connections[v].size() == 0) {
			cerr << v << "\n";
			if (!reduce_repetitions || !traversed[v]) {
				order.push_back(v);
				traversed[v] = 1;
			}
			stack.pop_back();
		} else {
			for (auto con : graph.connections[v]) {
				graph.connections[v].erase(con);
				if (reduce_repetitions) {
					graph.connections[con.target].erase({v,0});
				}
				stack.push_back(con.target);
				break;
			}
		}
	}
	if (!reduce_repetitions) {
		order.pop_back();
	}

	Graph new_graph(graph.size());
	new_graph.points = graph.points;
	for (int i = 0; i < order.size(); i++) {
		new_graph.add_connection(order[i], order[(i + 1) % order.size()]);
	}

	return new_graph;
}

Graph resolve_ambiguities_greedy(Graph graph) {
	// Double edges
	for (auto& v : graph.connections) {
		vector<Connection> insert_list;
		for (auto& con : v) {
			insert_list.push_back(con);
		}
		for (auto con : insert_list) {
			v.insert(con);
		}
	}

	// Find ambiguity with the best benefit

	int best_joint = 0;
	while(1) {
		best_joint = -1;
		pair<int, int> best_pair;
		double best_benefit = -1;

		for (int i = 0; i < graph.size(); i++) {
			if (graph.connections[i].size() <= 2) {
				continue;
			}
			for (auto con1 : graph.connections[i]) {
				for (auto con2 : graph.connections[i]) {
					if (con1.target == con2.target) continue;
					if (
						graph.connections[con1.target].count({i,0}) < 2 &&
						graph.connections[con2.target].count({i,0}) < 2
					) continue;

					double benefit = 
						distance(graph.points[con1.target], graph.points[i]) + 
						distance(graph.points[con2.target], graph.points[i]) -
						distance(graph.points[con1.target], graph.points[con2.target]);

					if (benefit > best_benefit) {
						best_benefit = benefit;
						best_joint = i;
						best_pair = {con1.target, con2.target};
					}
				}
			}
		}
		
		// Unambe to find resolvable ambiguity
		if (best_joint == -1) {
			break;
		}
		
		// Remove connections (god forgive me for this piece of code, I will never ever use multisets again)

		graph.connections[best_pair.first].erase(graph.connections[best_pair.first].lower_bound({best_joint, 0}));
		graph.connections[best_pair.second].erase(graph.connections[best_pair.second].lower_bound({best_joint, 0}));
		graph.connections[best_joint].erase(graph.connections[best_joint].lower_bound({best_pair.first, 0}));
		graph.connections[best_joint].erase(graph.connections[best_joint].lower_bound({best_pair.second, 0}));


		// Add new connections

		graph.add_connection(best_pair.first, best_pair.second);
		graph.add_connection(best_pair.second, best_pair.first);

	}
	

	return eulerian_path(graph, true);
}

// Calculate the sum of all edges' distances
double calculate_weight(const Graph& graph) {
	double acc = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (const auto& con : graph.connections[i]) {
			acc += distance(graph.points[i], graph.points[con.target]);
		}
	}
	return acc;
}

// Render graph into .dot format
void render_graph(const Graph& graph) {
	cout << "digraph G{\n";
	cout << "label=\"Path length = " << calculate_weight(graph) << "\"\n";

	for (int i = 0; i < graph.points.size(); i++) {
		cout << i << 
			"[shape=circle, pos=\"" << 
			graph.points[i].x << "," << 
			graph.points[i].y << "!\",label=" << 
			i << "]\n";
	}

	for (int i = 0; i < graph.connections.size(); i++) {
		set<Connection> con_traversed = {};
		for (const auto& con : graph.connections[i]) {
			cout << i << " -> " << con.target << "[label=\"" << con.index << "\"]\n";
		}
	}

	cout << "}\n";
}



int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Specify mode: TREE, DFS or GREED\n";
		return 1;
	}


	int n;
	cin >> n;

	vector<Point> points(n);
	for (auto& p : points) {
		cin >> p.x >> p.y;
	}

	if (string(argv[1]) == "TREE") {
		render_graph(eulerian_path(min_spanning_tree(points), false));
 	} else 
	if (string(argv[1]) == "DFS") {
		render_graph(eulerian_path(min_spanning_tree(points), true));
	} else 
	if (string(argv[1]) == "GREED") {
		render_graph((resolve_ambiguities_greedy(min_spanning_tree(points))));
	} else {
		cerr << "Unknown specifier " << argv[1] << "\n";
		return 1;
	}
	
	return 0;
}