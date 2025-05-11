#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

const int NIL = -1;

int nU, nV; // Sizes of partitions U and V
vector<vector<int>> adj;
vector<int> pairU, pairV, dist;

bool bfs() {
    queue<int> Q;
    for (int u = 0; u < nU; ++u) {
        if (pairU[u] == NIL) {
            dist[u] = 0;
            Q.push(u);
        } else {
            dist[u] = -1;
        }
    }

    bool found = false;
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (int v : adj[u]) {
            if (pairV[v] == NIL) {
                found = true;
            } else if (dist[pairV[v]] == -1) {
                dist[pairV[v]] = dist[u] + 1;
                Q.push(pairV[v]);
            }
        }
    }
    return found;
}

bool dfs(int u) {
    for (int v : adj[u]) {
        if (pairV[v] == NIL || (dist[pairV[v]] == dist[u] + 1 && dfs(pairV[v]))) {
            pairU[u] = v;
            pairV[v] = u;
            return true;
        }
    }
    dist[u] = -1;
    return false;
}

int hopcroft_karp() {
    pairU.assign(nU, NIL);
    pairV.assign(nV, NIL);
    dist.assign(nU, -1);

    int matching = 0;
    while (bfs()) {
        for (int u = 0; u < nU; ++u)
            if (pairU[u] == NIL && dfs(u))
                ++matching;
    }
    return matching;
}

void minimum_vertex_cover() {
    vector<bool> visitedU(nU, false), visitedV(nV, false);
    queue<int> Q;
    
    // Find unmatched vertices in U and mark them as visited
    for (int u = 0; u < nU; ++u)
        if (pairU[u] == NIL)
            Q.push(u), visitedU[u] = true;

    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (int v : adj[u]) {
            if (!visitedV[v] && pairU[u] != v) {
                visitedV[v] = true;
                if (pairV[v] != NIL && !visitedU[pairV[v]]) {
                    visitedU[pairV[v]] = true;
                    Q.push(pairV[v]);
                }
            }
        }
    }

    // Debugging output to verify visited sets
    cout << "\nvisitedU: ";
    for (int i = 0; i < nU; ++i) cout << visitedU[i] << " ";
    cout << "\nvisitedV: ";
    for (int i = 0; i < nV; ++i) cout << visitedV[i] << " ";
    cout << "\n";

    cout << "\nMinimum Vertex Cover:\n";
    cout << "U set: ";
    for (int u = 0; u < nU; ++u)
        if (!visitedU[u]) cout << "U" << u << " ";
    cout << "\nV set: ";
    for (int v = 0; v < nV; ++v)
        if (visitedV[v]) cout << "V" << v << " ";
    cout << '\n';
}

int main() {
    string filename;
    cout << "Enter input filename: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Cannot open file.\n";
        return 1;
    }

    int m;
    infile >> nU >> nV >> m;
    adj.assign(nU, vector<int>());

    for (int i = 0; i < m; ++i) {
        int u, v;
        infile >> u >> v;
        adj[u].push_back(v);
    }
    infile.close();

    int matching = hopcroft_karp();
    cout << "\nMaximum Matching: " << matching << '\n';
    for (int u = 0; u < nU; ++u) {
        if (pairU[u] != NIL)
            cout << "(U" << u << ", V" << pairU[u] << ")\n";
    }

    minimum_vertex_cover();
    return 0;
}
// This code implements the Hopcroft-Karp algorithm for finding the maximum matching in a bipartite graph.
// It also computes the minimum vertex cover based on the maximum matching. 