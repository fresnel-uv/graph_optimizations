#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// Original item structure
struct Item {
    int weight;
    int value;
    int count; // max copies available
};

// Split item structure (for binary splitting)
struct SplitItem {
    int weight;
    int value;
    int originalIndex;  // which original item it came from
    int quantity;       // how many copies this split item represents
};

// Reads CSV: each line is "weight,value,count"
vector<Item> readItemsFromCSV(const string& filename) {
    vector<Item> items;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return items;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<int> vals;
        while (getline(ss, cell, ',')) {
            try {
                vals.push_back(stoi(cell));
            } catch (...) {
                cerr << "Invalid integer in CSV: " << cell << endl;
                return {};
            }
        }
        if (vals.size() != 3) {
            cerr << "Each line must have 3 integers: weight,value,count\n";
            return {};
        }
        items.push_back({vals[0], vals[1], vals[2]});
    }
    return items;
}

// Binary splitting transformation of items for bounded knapsack
vector<SplitItem> binarySplitItems(const vector<Item>& items) {
    vector<SplitItem> splitItems;
    for (int i = 0; i < (int)items.size(); ++i) {
        int c = items[i].count;
        int w = items[i].weight;
        int v = items[i].value;
        int k = 1;
        while (c > 0) {
            int take = min(k, c);
            splitItems.push_back({w * take, v * take, i, take});
            c -= take;
            k <<= 1;
        }
    }
    return splitItems;
}

// Solves 0-1 knapsack and tracks item choices for split items
int knapsackWithTracking(int W, const vector<SplitItem>& splitItems, vector<int>& taken) {
    int n = (int)splitItems.size();
    vector<int> dp(W + 1, 0);
    vector<int> choice(W + 1, -1); // stores which split item chosen for dp[w]

    // For reconstruction, store previous weight
    vector<int> prev(W + 1, -1);

    for (int i = 0; i < n; ++i) {
        int wt = splitItems[i].weight;
        int val = splitItems[i].value;

        for (int w = W; w >= wt; --w) {
            if (dp[w - wt] + val > dp[w]) {
                dp[w] = dp[w - wt] + val;
                choice[w] = i;
                prev[w] = w - wt;
            }
        }
    }

    // Reconstruct taken split items
    taken.assign(n, 0);
    int w = W;
    while (w > 0 && choice[w] != -1) {
        int idx = choice[w];
        taken[idx]++;
        w = prev[w];
    }

    return dp[W];
}

// Converts split item selections back to original item counts
vector<int> reconstructOriginalItems(const vector<SplitItem>& splitItems, const vector<int>& taken, int originalCount) {
    vector<int> result(originalCount, 0);
    for (int i = 0; i < (int)splitItems.size(); ++i) {
        result[splitItems[i].originalIndex] += splitItems[i].quantity * taken[i];
    }
    return result;
}

// Prints chosen items nicely
void printChosenItems(const vector<int>& chosenCounts, const vector<Item>& items) {
    cout << "\nItems chosen (original indices):\n";
    for (int i = 0; i < (int)items.size(); ++i) {
        if (chosenCounts[i] > 0) {
            cout << "Item " << i << ": " << chosenCounts[i] << " copies (weight = " 
                 << items[i].weight << ", value = " << items[i].value << ")\n";
        }
    }
}

int main() {
    string filename;
    cout << "Enter CSV file path (format: weight,value,count per line): ";
    cin >> filename;

    vector<Item> items = readItemsFromCSV(filename);
    if (items.empty()) {
        cerr << "Failed to read items from CSV. Exiting.\n";
        return 1;
    }

    int capacity;
    cout << "Enter knapsack capacity: ";
    cin >> capacity;

    vector<SplitItem> splitItems = binarySplitItems(items);

    vector<int> takenSplitItems;
    int maxValue = knapsackWithTracking(capacity, splitItems, takenSplitItems);

    vector<int> chosenOriginal = reconstructOriginalItems(splitItems, takenSplitItems, (int)items.size());

    cout << "\nMaximum achievable value: " << maxValue << "\n";
    printChosenItems(chosenOriginal, items);

    return 0;
}
