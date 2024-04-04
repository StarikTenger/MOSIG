#include <iostream>
#include <vector>

using namespace std;

/*
  0  1  2  3  4  5  6  7  8  9  10
0 0  0  0  0  0  0  0  0  0  0  0 
1 
2
3
*/

enum DIR {
	UP, LEFT, PICK
};

int main() {
	vector<int> values  = {8, 9, 6};
	vector<int> weights = {4, 3, 2};
	int max_weight = 11;

	vector<vector<int>> table(values.size() + 1);
	vector<vector<int>> comefrom(values.size() + 1);
	for (auto& r : table) r = vector<int>(max_weight + 1);
	for (auto& r : comefrom) r = vector<int>(max_weight + 1);

	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			if (i == 0 || j - weights[i - 1] < 0) {
				table[i][j] = 0;
			} else {
				int up = table[i-1][j];
				int left = table[i][j - weights[i - 1]] + values[i - 1];
				
				if (up > left) {
					comefrom[i][j] = UP;
					table[i][j] = up;
				} else {
					comefrom[i][j] = LEFT;
					table[i][j] = left;
				}
			}

			cout << table[i][j] << " ";
		}
		cout << "\n";
	}

	vector<int> solution(values.size(), 0);

	int i = values.size();
	int j = max_weight;
	while (i != 0 && j != 0) {
		auto cm = comefrom[i][j];
		comefrom[i][j] = PICK;
		switch (cm)
		{
		case UP:
			i--;
			break;
		case LEFT:
			solution[i - 1]++;
			j -= weights[i - 1];
			break;
		}
	}

	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			switch (comefrom[i][j])
		{
		case UP:
			cout << "u ";
			break;
		case LEFT:
			cout << "l ";
			break;
		default:
			cout << "X ";
		}
		}
		cout << "\n";
	}

	cout << "Solution:\n";
	for (auto s : solution) {
		cout << s << "\n";
	}
}