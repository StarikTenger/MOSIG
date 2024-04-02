// Heuristic algorithm based on graph representation

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <deque>
#include <algorithm>

using namespace std;

struct Course {
	int start;
	int end;
	int capacity;
	int rooms_currently = 0;
	vector<int> children = {};
	map<int, int> connection_weights;
	vector<int> parents = {};
	int rank = 0;
};

bool operator<(const Course& l, const Course& r) {
	return l.rank < r.rank;
}

void build_graph(vector<Course>& courses, vector<vector<int>> prep_time) {
	for (int i = 0; i < courses.size(); i++) {
		for (int j = 0; j < courses.size(); j++) {
			if (courses[i].end + prep_time[i][j] < courses[j].start) {
				courses[i].children.push_back(j);
				courses[j].parents.push_back(i);
			}
		}
	}
}

void rank_graph(vector<Course>& courses) {
	deque<int> q;
	for (int i = 0; i < courses.size(); i++) {
		if (courses[i].parents.size() == 0) {
			q.push_back(i);
		}
	}

	while (q.size()) {
		int i = q.back();
		q.pop_back();

		for (auto child : courses[i].children) {
			courses[child].rank = courses[i].rank + 1;
			q.push_back(child);
		}
	}
}

int count_rooms(vector<Course>& courses) {
	deque<int> q;
	for (int i = 0; i < courses.size(); i++) {
		if (courses[i].parents.size() == 0) {
			q.push_back(i);
		}
	}

	int counter = 0;

	while (q.size()) {
		int i = q.back();
		q.pop_back();

		for (auto parent : courses[i].parents) {
			if (courses[parent].rooms_currently > 0) {
				int rooms_taken = min(courses[parent].rooms_currently, courses[i].capacity - courses[i].rooms_currently);
				courses[i].rooms_currently += rooms_taken;
				courses[parent].rooms_currently -= rooms_taken;
				courses[parent].connection_weights[i] = rooms_taken;
			}
		}

		counter += courses[i].capacity - courses[i].rooms_currently;
		courses[i].rooms_currently = courses[i].capacity;

		for (auto child : courses[i].children) {
			if (courses[child].rank == courses[i].rank + 1) {
				q.push_front(child);
			}
		}
	}

	return counter;
}

void render_graph(vector<Course>& courses) {
	string str = "digraph G {\n";
	for (int i = 0; i < courses.size(); i++) {
		str += 
			"a" + to_string(i) + 
			"[label=" + "\"" + 
			to_string(courses[i].capacity) + 
			" rank=" + to_string(courses[i].rank) + 
			"\"" + "]\n";
	}

	for (int i = 0; i < courses.size(); i++) {
		for (auto child : courses[i].children) {
			str += "a" + to_string(i) + "->a" + to_string(child) + "[label=\"" + to_string(courses[i].connection_weights[child]) + "\"]\n";
		}
	}

	str += "}";

	cout << str;
}

int main() {
	int n, m;
	cin >> n >> m;

	vector<Course> courses(n);
	for (auto& course : courses) {
		cin >> course.start >> course.end >> course.capacity;
		course.capacity /= m; // ???
	}

	vector<vector<int>> prep_time;
	for (int i = 0; i < n; i++) {
		prep_time.push_back({});
		for (int j = 0; j < n; j++) {
			prep_time[i].push_back({});
			cin >> prep_time[i][j];
		}
	}

	build_graph(courses, prep_time);
	rank_graph(courses);
	cout << count_rooms(courses) << "\n\n";
	render_graph(courses);
}