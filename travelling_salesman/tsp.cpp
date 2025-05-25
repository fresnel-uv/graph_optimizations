#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int n;
vector<vector<int>> dist;

// Hamiltonian Cycle Check
bool isHamiltonianUtil(vector<vector<int>>& graph, vector<bool>& visited, int pos, int count, int start) {
    if (count == n && graph[pos][start] != INF) {
        return true;
    }
    for (int v = 0; v < n; ++v) {
        if (!visited[v] && graph[pos][v] != INF) {
            visited[v] = true;
            if (isHamiltonianUtil(graph, visited, v, count + 1, start))
                return true;
            visited[v] = false;
        }
    }
    return false;
}

bool hasHamiltonianCycle(vector<vector<int>>& graph) {
    vector<bool> visited(n, false);
    visited[0] = true;
    return isHamiltonianUtil(graph, visited, 0, 1, 0);
}

// Held-Karp Algorithm (Dynamic Programming TSP)
int heldKarp(vector<vector<int>>& path) {
    int VISITED_ALL = (1 << n) - 1;
    vector<vector<int>> dp(1 << n, vector<int>(n, INF));
    dp[1][0] = 0;

    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) continue;
            for (int v = 0; v < n; ++v) {
                if ((mask & (1 << v)) == 0 && dist[u][v] != INF) {
                    if (dp[mask | (1 << v)][v] > dp[mask][u] + dist[u][v]) {
                        dp[mask | (1 << v)][v] = dp[mask][u] + dist[u][v];
                        path[mask | (1 << v)][v] = u;
                    }
                }
            }
        }
    }

    int res = INF, last_index = -1;
    for (int i = 1; i < n; ++i) {
        if (dist[i][0] != INF && dp[VISITED_ALL][i] + dist[i][0] < res) {
            res = dp[VISITED_ALL][i] + dist[i][0];
            last_index = i;
        }
    }

    // Trace the path
    vector<int> tsp_path;
    int mask = VISITED_ALL;
    while (last_index != -1) {
        tsp_path.push_back(last_index);
        int prev = path[mask][last_index];
        mask ^= (1 << last_index);
        last_index = prev;
    }
    reverse(tsp_path.begin(), tsp_path.end());

    // Print the path
    cout << "TSP Path: ";
    for (int i = 0; i < tsp_path.size(); ++i) {
        cout << tsp_path[i] << " ";
    }
    cout << "\n";

    return res;
}

// Load from CSV
void loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename << "\n";
        exit(1);
    }

    string line;
    int maxNode = 0;
    vector<tuple<int, int, int>> edges;

    while (getline(file, line)) {
        stringstream ss(line);
        string a_str, b_str, w_str;
        if (!getline(ss, a_str, ',')) continue;
        if (!getline(ss, b_str, ',')) continue;
        if (!getline(ss, w_str, ',')) continue;

        int a = stoi(a_str), b = stoi(b_str), w = stoi(w_str);
        maxNode = max({maxNode, a, b});
        edges.emplace_back(a, b, w);
    }

    file.close();
    n = maxNode + 1;
    dist.assign(n, vector<int>(n, INF));
    for (int i = 0; i < n; ++i)
        dist[i][i] = 0;

    for (auto& [a, b, w] : edges) {
        dist[a][b] = w;
        // Uncomment to make undirected
        // dist[b][a] = w;
    }
}

int main() {
    cout << "Choose input method:\n";
    cout << "1. Manual input\n";
    cout << "2. Load from CSV file\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        int m;
        cout << "Enter number of vertices (cities): ";
        cin >> n;
        cout << "Enter number of directed edges: ";
        cin >> m;

        dist.assign(n, vector<int>(n, INF));
        for (int i = 0; i < n; ++i)
            dist[i][i] = 0;

        cout << "Enter edges in format (a,b,w):\n";
        for (int i = 0; i < m; ++i) {
            int a, b, w;
            char ch1, ch2, ch3, ch4;
            cin >> ch1 >> a >> ch2 >> b >> ch3 >> w >> ch4;
            dist[a][b] = w;
            // Uncomment to make undirected
            // dist[b][a] = w;
        }
    } else if (choice == 2) {
        string filename;
        cout << "Enter CSV file name (e.g., graph.csv): ";
        cin >> filename;
        loadFromCSV(filename);
    } else {
        cout << "Invalid choice. Exiting.\n";
        return 1;
    }

    if (!hasHamiltonianCycle(dist)) {
        cout << "Graph does not have a Hamiltonian cycle. TSP not solvable.\n";
    } else {
        vector<vector<int>> path(1 << n, vector<int>(n, -1));  // Path reconstruction
        int result = heldKarp(path);
        cout << "Minimum TSP cost (Held-Karp): " << result << "\n";
    }

    return 0;
}