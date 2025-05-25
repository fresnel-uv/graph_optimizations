#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

// Hungarian algorithm class
class Hungarian {
    int n;
    vector<vector<int>> cost;
    vector<int> u, v, p, way;

public:
    Hungarian(const vector<vector<int>>& costMatrix) {
        n = (int)costMatrix.size();
        cost = costMatrix;
        u.assign(n + 1, 0);
        v.assign(n + 1, 0);
        p.assign(n + 1, 0);
        way.assign(n + 1, 0);
    }

    int solve(vector<int>& assignment) {
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            vector<int> minv(n + 1, numeric_limits<int>::max());
            vector<bool> used(n + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], j1;
                int delta = numeric_limits<int>::max();
                for (int j = 1; j <= n; ++j) {
                    if (!used[j]) {
                        int cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }
                for (int j = 0; j <= n; ++j) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != 0);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }

        assignment.assign(n, -1);
        for (int j = 1; j <= n; ++j) {
            if (p[j] > 0)
                assignment[p[j] - 1] = j - 1;
        }

        int costSum = 0;
        for (int i = 0; i < n; ++i) {
            costSum += cost[i][assignment[i]];
        }
        return costSum;
    }
};

// Reads matrix from CSV file
vector<vector<int>> readCSV(const string& filename) {
    vector<vector<int>> matrix;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return {};
    }

    string line;
    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ',')) {
            try {
                row.push_back(stoi(cell));
            } catch (...) {
                cerr << "Invalid integer in CSV: " << cell << "\n";
                return {};
            }
        }
        matrix.push_back(row);
    }

    // Verify square matrix
    int n = (int)matrix.size();
    for (const auto& r : matrix) {
        if ((int)r.size() != n) {
            cerr << "CSV is not a square matrix\n";
            return {};
        }
    }

    return matrix;
}

// Prints assignments neatly
void printAssignments(const vector<int>& assignment) {
    cout << "Assignments:\n";
    for (int i = 0; i < (int)assignment.size(); ++i) {
        cout << "Agent " << i << " -> Task " << assignment[i] << "\n";
    }
}

int main() {
    string filename;
    cout << "Enter CSV file path: ";
    cin >> filename;

    vector<vector<int>> cost = readCSV(filename);
    if (cost.empty()) {
        cerr << "Failed to read matrix from CSV. Exiting.\n";
        return 1;
    }

    Hungarian solver(cost);
    vector<int> assignment;
    int minCost = solver.solve(assignment);

    cout << "Minimum cost: " << minCost << "\n";
    printAssignments(assignment);

    return 0;
}
