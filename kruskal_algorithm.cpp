#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>
#include <climits>
using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// ---------- Disjoint Set Union for Kruskal ----------
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        parent[ra] = rb;
        return true;
    }
};

// ---------- Kruskal's Algorithm ----------
void kruskal(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    int totalWeight = 0;

    cout << "\nKruskal's Minimum Spanning Tree:\n";
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            cout << "(" << e.u << ", " << e.v << ") - weight " << e.weight << "\n";
            totalWeight += e.weight;
        }
    }
    cout << "Total weight: " << totalWeight << "\n";
}

// ---------- Prim's Algorithm ----------
void prim(int n, const vector<vector<pair<int, int>>>& adj) {
    vector<int> key(n, INT_MAX), parent(n, -1);
    vector<bool> inMST(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;

        for (auto [v, w] : adj[u]) {
            if (!inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
                pq.push({w, v});
            }
        }
    }

    int totalWeight = 0;
    cout << "\nPrim's Minimum Spanning Tree:\n";
    for (int i = 1; i < n; ++i) {
        cout << "(" << parent[i] << ", " << i << ") - weight " << key[i] << "\n";
        totalWeight += key[i];
    }
    cout << "Total weight: " << totalWeight << "\n";
}

// ---------- Main Function ----------
int main() {
    string filename;
    cout << "Enter input filename: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    int n, m;
    infile >> n >> m;
    vector<Edge> edges(m);
    char ch1, ch2, comma;

    for (int i = 0; i < m; ++i) {
        infile >> ch1 >> edges[i].u >> comma >> edges[i].v >> comma >> edges[i].weight >> ch2;
    }
    infile.close();

    // Build adjacency list for Prim
    vector<vector<pair<int, int>>> adj(n);
    for (const auto& e : edges) {
        adj[e.u].emplace_back(e.v, e.weight);
        adj[e.v].emplace_back(e.u, e.weight);
    }

    int choice;
    cout << "\nChoose Algorithm:\n1. Kruskal's\n2. Prim's\nEnter choice (1 or 2): ";
    cin >> choice;

    if (choice == 1)
        kruskal(n, edges);
    else if (choice == 2)
        prim(n, adj);
    else
        cout << "Invalid choice.\n";

    return 0;
}

// Compile with: g++ -std=c++11 kruskal_algorithm.cpp -o kruskal_algorithm
// Run with: ./kruskal_algorithm
// Input file format: n m (number of vertices and edges)
// Followed by m lines of edges in the format: (u, v, weight)