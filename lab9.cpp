#include <bits/stdc++.h>
#include <chrono>

using namespace std;

struct Edge {
    int u, v, w;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Начало замера времени
    auto start_time = chrono::high_resolution_clock::now();

    int n, m, start, finish;
    cin >> n >> m >> start >> finish;

    vector<Edge> edges;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    vector<long long> dist(n + 1, numeric_limits<long long>::max());
    dist[start] = 0;

    for (int i = 0; i < n; ++i) {
        bool updated = false;
        for (const auto& edge : edges) {
            int u = edge.u;
            int v = edge.v;
            int w = edge.w;
            if (dist[u] != numeric_limits<long long>::max() && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = true;
            }
        }
        if (!updated) break;
    }

    if (dist[finish] == numeric_limits<long long>::max()) {
        cout << "No solution" << '\n';
    } else {
        cout << dist[finish] << '\n';
    }

    // Конец замера времени
    auto end_time = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cerr << "Время выполнения: " << elapsed.count() << " миллисекунд.\n";

    return 0;
}
