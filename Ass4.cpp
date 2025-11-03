#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
class TrafficNetwork {
private:
int V;
vector<vector<pair<int, int>>> adj; // adjacency list: node -> (neighbor, weight)
vector<int> hospitals;
9
public:
TrafficNetwork(int vertices) {
V = vertices;
adj.resize(V);
}
void addEdge(int u, int v, int w) {
adj[u].push_back({v, w});
adj[v].push_back({u, w}); // undirected graph
}
void updateEdgeWeight(int u, int v, int newWeight) {
for (auto &edge : adj[u]) {
10
if (edge.first == v) edge.second = newWeight;
}
for (auto &edge : adj[v]) {
if (edge.first == u) edge.second = newWeight;
}
}
void addHospital(int node) {
hospitals.push_back(node);
}
pair<int, vector<int>> shortestPathToNearestHospital(int source) {
vector<int> dist(V, INT_MAX);
vector<int> parent(V, -1);
11
dist[source] = 0;
priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
pq.push({0, source});
while (!pq.empty()) {
auto [curDist, u] = pq.top(); pq.pop();
if (curDist > dist[u]) continue;
// Check if current node is a hospital
if (find(hospitals.begin(), hospitals.end(), u) != hospitals.end()) {
vector<int> path;
int v = u;
while (v != -1) {
12
path.push_back(v);
v = parent[v];
}
reverse(path.begin(), path.end());
return {dist[u], path};
}
for (auto [v, w] : adj[u]) {
if (dist[u] + w < dist[v]) {
dist[v] = dist[u] + w;
parent[v] = u;
pq.push({dist[v], v});
}
}
13
}
return {-1, {}}; // No hospital reachable
}
};
int main() {
int V, E;
cout << "Enter number of vertices (intersections): ";
cin >> V;
TrafficNetwork city(V);
cout << "Enter number of edges (roads): ";
cin >> E;
14
cout << "Enter edges (source destination weight):\n";
for (int i = 0; i < E; i++) {
int u, v, w;
cin >> u >> v >> w;
city.addEdge(u, v, w);
}
int numHospitals;
cout << "Enter number of hospitals: ";
cin >> numHospitals;
cout << "Enter hospital locations:\n";
for (int i = 0; i < numHospitals; i++) {
int h;
cin >> h;
15
city.addHospital(h);
}
int ambulanceLocation;
cout << "Enter ambulance starting location: ";
cin >> ambulanceLocation;
auto [distance, path] = city.shortestPathToNearestHospital(ambulanceLocation);
if (distance != -1) {
cout << "Initial shortest travel time: " << distance << "\nPath: ";
for (int node : path) cout << node << " ";
cout << endl;
} else {
cout << "No hospital reachable\n";
16
}
int u, v, newWeight;
cout << "Enter edge to update (u v newWeight): ";
cin >> u >> v >> newWeight;
city.updateEdgeWeight(u, v, newWeight);
auto [newDistance, newPath] = city.shortestPathToNearestHospital(ambulanceLocation);
if (newDistance != -1) {
cout << "After update shortest travel time: " << newDistance << "\nPath: ";
for (int node : newPath) cout << node << " ";
cout << endl;
} else {
cout << "No hospital reachable after update\n";
17
}
return 0;
}
