// Multistage shortest path (DP) with real-time edge multipliers and batch queries.
// Compile: g++ -std=c++17 -O2 multistage.cpp -o multistage
#include <bits/stdc++.h>
using namespace std;
const double INF = 1e18;
struct Edge {
int id;
int from, to;
double base_cost; // e.g., monetary cost
double base_time; // e.g., time in minutes
double multiplier; // real-time multiplier (1.0 = no change)
Edge(int _id=0,int _f=0,int _t=0,double c=0,double tm=0):
id(_id),from(_f),to(_t),base_cost(c),base_time(tm),multiplier(1.0){}
double cost(double costWeight, double timeWeight) const {
// Combined objective: costWeight * (effective cost) + timeWeight * (effective time)
return costWeight * (base_cost * multiplier) + timeWeight * (base_time * multiplier);
}
};
// Graph stores adjacency list; nodes are 0..V-1
struct MultistageGraph {
int V;
int stagesCount;
vector<int> stageOf; // stageOf[node] in [1..stagesCount]
vector<vector<int>> nodesInStage; // nodesInStage[k] = nodes with stage = k (1-based index)
vector<vector<Edge>> adj; // adj[from] = list of outgoing edges
unordered_map<int, pair<int,int>> edgeIndex; // id -> (from, idx in adj[from])
int nextEdgeId = 1;
MultistageGraph(int V_ = 0, int stages = 0) { init(V_, stages); }
void init(int V_, int stages) {
V = V_;
stagesCount = stages;
stageOf.assign(V, 0);
adj.assign(V, {});
nodesInStage.assign(stages+1, {});
edgeIndex.clear();
nextEdgeId = 1;
}
void setNodeStage(int node, int stage) {
if (node < 0 || node >= V) throw runtime_error("node index out of range");
if (stage < 1 || stage > stagesCount) throw runtime_error("stage out of range");
stageOf[node] = stage;
}
// Add edge only allowed from stage k to k+1 (enforces multistage property).
int addEdge(int u, int v, double cost, double time) {
if (stageOf[u] == 0 || stageOf[v] == 0)
throw runtime_error("node stages must be set before adding edges");
if (stageOf[v] != stageOf[u] + 1) {
// We strictly enforce consecutive-stage edges to guarantee you visit each stage.
throw runtime_error("Edges allowed only from stage k to stage k+1 in this model");
}
int id = nextEdgeId++;
Edge e(id, u, v, cost, time);
adj[u].push_back(e);
edgeIndex[id] = {u, (int)adj[u].size() - 1};
nodesInStage[stageOf[u]].push_back(u); // may push duplicates; we'll fix after building graph
nodesInStage[stageOf[v]].push_back(v);
return id;
}
// Set multiplier for an edge (simulate traffic/weather/fuel effect). multiplier >= 0
void setEdgeMultiplier(int edgeId, double mult) {
auto it = edgeIndex.find(edgeId);
if (it == edgeIndex.end()) throw runtime_error("Edge id not found");
int u = it->second.first;
int idx = it->second.second;
adj[u][idx].multiplier = mult;
}
// Clean nodesInStage duplicates (call after graph construction)
void finalizeStages() {
for (int k = 1; k <= stagesCount; ++k) {
auto &vec = nodesInStage[k];
sort(vec.begin(), vec.end());
vec.erase(unique(vec.begin(), vec.end()), vec.end());
}
}
// DP to find optimal path that passes exactly through stages 1..stagesCount in order.
// sources: vector of source nodes (must belong to stage 1).
// destinations: vector of destination nodes (must belong to stage stagesCount).
// costWeight, timeWeight: objective = costWeight * cost + timeWeight * time
// returns pair(bestCost, path). If no path, bestCost = INF and path empty.
pair<double, vector<int>> findOptimalPathAcrossStages(
const vector<int>& sources,
const vector<int>& destinations,
double costWeight = 1.0,
double timeWeight = 0.0)
{
// Validate inputs
for (int s : sources) if (stageOf[s] != 1) throw runtime_error("all sources must be in stage 1");
for (int d : destinations) if (stageOf[d] != stagesCount) throw runtime_error("all destinations must be
in final stage");
vector<double> dp(V, INF);
vector<int> parent(V, -1);
// initialize dp for stage 1 nodes: we consider starting cost zero at source nodes
for (int s : sources) dp[s] = 0.0;
// iterate stage by stage
for (int k = 1; k < stagesCount; ++k) {
// for each node u in stage k that has finite dp, relax edges to nodes in stage k+1
for (int u : nodesInStage[k]) {
if (dp[u] >= INF) continue;
for (const Edge &e : adj[u]) {
// e.to must be in stage k+1 by construction
double w = e.cost(costWeight, timeWeight);
if (dp[u] + w < dp[e.to]) {
dp[e.to] = dp[u] + w;
parent[e.to] = u;
}
}
}
}
// pick best destination
double best = INF;
int bestNode = -1;
for (int d : destinations) {
if (dp[d] < best) { best = dp[d]; bestNode = d; }
}
if (bestNode == -1) return {INF, {}};
// reconstruct path backwards (it will include exactly one node per stage)
vector<int> path;
int cur = bestNode;
while (cur != -1) {
path.push_back(cur);
if (stageOf[cur] == 1) break; // reached stage 1
cur = parent[cur];
}
reverse(path.begin(), path.end());
return {best, path};
}
};
/////////////////////// Example usage (main) ///////////////////////
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);
/* Example input format (you can adapt):
V E stages
for i in 0..V-1: stage_of_node[i] (1-based stage)
then E lines: u v cost time (0-based node indices; edges must be from stage k to k+1)
Q (number of queries)
For each query:
sCount s1 s2 ... dCount d1 d2 ... objectiveType costWeight timeWeight
objectiveType unused in code (we use costWeight/timeWeight)
After queries, optionally real-time updates:
U (num updates)
for each update: edgeId newMultiplier
*/
cout << "Enter V E stages:\n";
int V, E, stages;
if (!(cin >> V >> E >> stages)) {
cerr << "Bad input\n"; return 0;
}
MultistageGraph G(V, stages);
cout << "Enter stage for each node (0-based node index order):\n";
for (int i = 0; i < V; ++i) {
int st; cin >> st;
G.setNodeStage(i, st);
}
cout << "Enter edges (u v cost time). Edges must go from stage k to k+1:\n";
vector<int> edgeIds;
for (int i = 0; i < E; ++i) {
int u,v; double c,t;
cin >> u >> v >> c >> t;
try {
int id = G.addEdge(u,v,c,t);
edgeIds.push_back(id);
} catch (exception &ex) {
cerr << "Error adding edge: " << ex.what() << "\n";
return 0;
}
}
G.finalizeStages();
cout << "Enter Q (queries):\n";
int Q; cin >> Q;
for (int qi = 0; qi < Q; ++qi) {
int sCount; cin >> sCount;
vector<int> sources(sCount);
for (int i = 0; i < sCount; ++i) cin >> sources[i];
int dCount; cin >> dCount;
vector<int> dests(dCount);
for (int i = 0; i < dCount; ++i) cin >> dests[i];
double costW, timeW; cin >> costW >> timeW;
auto res = G.findOptimalPathAcrossStages(sources, dests, costW, timeW);
if (res.first >= INF/2) {
cout << "No feasible path for query " << qi+1 << "\n";
} else {
cout << "Query " << qi+1 << " best objective = " << res.first << "\n";
cout << "Path (nodes by index):";
for (int x : res.second) cout << " " << x;
cout << "\n";
}
}
// Example of applying real-time updates (optional)
cout << "Enter U (edge multiplier updates), 0 to skip:\n";
int U; cin >> U;
for (int i = 0; i < U; ++i) {
int edgeId; double mult;
cin >> edgeId >> mult;
try { G.setEdgeMultiplier(edgeId, mult); }
catch (exception &ex) { cerr << "Update error: " << ex.what() << "\n"; }
}
// After updates, you can run queries again or recompute as above.
cout << "Done.\n";
return 0;
}
