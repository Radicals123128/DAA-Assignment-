#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (ll)9e15;
struct Node {
vector<vector<ll>> mat;
vector<int> path;
int level;
ll lb;
ll cost;
};
ll reduce_matrix(vector<vector<ll>>& m) {
int n = m.size();
ll red = 0;
for (int i = 0; i < n; i++) {
ll rmin = INF;
for (int j = 0; j < n; j++) rmin = min(rmin, m[i][j]);
if (rmin == INF || rmin == 0) continue;
red += rmin;
for (int j = 0; j < n; j++)
if (m[i][j] < INF) m[i][j] -= rmin;
}
for (int j = 0; j < n; j++) {
ll cmin = INF;
for (int i = 0; i < n; i++) cmin = min(cmin, m[i][j]);
if (cmin == INF || cmin == 0) continue;
red += cmin;
for (int i = 0; i < n; i++)
if (m[i][j] < INF) m[i][j] -= cmin;
}
return red;
}
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);
int N;
cin >> N;
vector<vector<ll>> cost(N, vector<ll>(N));
for (int i = 0; i < N; i++)
for (int j = 0; j < N; j++)
cin >> cost[i][j];
int start;
cin >> start;
for (int i = 0; i < N; i++) cost[i][i] = INF;
vector<vector<ll>> rootMat = cost;
ll lb0 = reduce_matrix(rootMat);
Node root;
root.mat = rootMat;
root.path = {start};
root.level = 0;
root.lb = lb0;
root.cost = 0;
auto cmp = [](const Node& a, const Node& b) {
return a.lb > b.lb;
};
priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);
pq.push(root);
ll bestCost = INF;
vector<int> bestPath;
while (!pq.empty()) {
Node node = pq.top();
pq.pop();
if (node.lb >= bestCost) continue;
if (node.level == N - 1) {
int last = node.path.back();
if (cost[last][start] < INF) {
ll total = node.cost + cost[last][start];
if (total < bestCost) {
bestCost = total;
bestPath = node.path;
bestPath.push_back(start);
}
}
continue;
}
int u = node.path.back();
for (int v = 0; v < N; v++) {
if (find(node.path.begin(), node.path.end(), v) != node.path.end()) continue;
if (node.mat[u][v] == INF) continue;
Node child;
child.mat = node.mat;
for (int j = 0; j < N; j++) child.mat[u][j] = INF;
for (int i = 0; i < N; i++) child.mat[i][v] = INF;
child.mat[v][start] = INF;
ll edgeCost = cost[u][v];
if (edgeCost >= INF) continue;
ll childCost = node.cost + edgeCost;
ll reduction = reduce_matrix(child.mat);
child.lb = childCost + reduction;
child.cost = childCost;
child.path = node.path;
child.path.push_back(v);
child.level = node.level + 1;
if (child.lb < bestCost) pq.push(child);
}
}
if (bestCost == INF) {
cout << "No feasible tour\n";
return 0;
}
cout << bestCost << "\n";
for (size_t i = 0; i < bestPath.size(); i++) {
if (i) cout << " ";
cout << bestPath[i];
}
cout << "\n";
return 0;
}
