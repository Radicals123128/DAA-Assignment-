#include <bits/stdc++.h>
using namespace std;
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);
int C;
if (!(cin >> C)) return 0;
vector<string> courseNames(C);
unordered_map<string, int> courseIndex;
for (int i = 0; i < C; i++) {
cin >> ws;
getline(cin, courseNames[i]);
courseIndex[courseNames[i]] = i;
}
int S;
cin >> S;
vector<int> studentsIn(C, 0);
vector<vector<int>> adj(C);
for (int si = 0; si < S; si++) {
string sid;
int k;
cin >> ws >> sid >> k;
vector<int> enrolled;
for (int j = 0; j < k; j++) {
string cname;
cin >> ws;
getline(cin, cname);
if (courseIndex.find(cname) == courseIndex.end()) continue;
int idx = courseIndex[cname];
enrolled.push_back(idx);
studentsIn[idx]++;
}
sort(enrolled.begin(), enrolled.end());
enrolled.erase(unique(enrolled.begin(), enrolled.end()), enrolled.end());
for (size_t a = 0; a < enrolled.size(); a++)
for (size_t b = a + 1; b < enrolled.size(); b++) {
int u = enrolled[a], v = enrolled[b];
adj[u].push_back(v);
adj[v].push_back(u);
}
}
for (int i = 0; i < C; i++) {
sort(adj[i].begin(), adj[i].end());
adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
}
vector<int> degree(C);
for (int i = 0; i < C; i++) degree[i] = adj[i].size();
vector<int> order(C);
iota(order.begin(), order.end(), 0);
sort(order.begin(), order.end(), [&](int a, int b) {
if (degree[a] != degree[b]) return degree[a] > degree[b];
return a < b;
});
vector<int> color(C, -1);
for (int idx : order) {
vector<char> used(C + 1, false);
for (int v : adj[idx])
if (color[v] != -1) used[color[v]] = true;
int c = 1;
while (used[c]) c++;
color[idx] = c;
}
int maxColor = 0;
for (int i = 0; i < C; i++) maxColor = max(maxColor, color[i]);
int R;
cin >> R;
vector<pair<string, int>> rooms(R);
for (int i = 0; i < R; i++) {
string rname;
int cap;
cin >> ws;
getline(cin, rname);
cin >> cap;
rooms[i] = {rname, cap};
}
vector<int> roomOrder(R);
iota(roomOrder.begin(), roomOrder.end(), 0);
sort(roomOrder.begin(), roomOrder.end(), [&](int a, int b) {
if (rooms[a].second != rooms[b].second)
return rooms[a].second > rooms[b].second;
return a < b;
});
vector<unordered_set<int>> roomOccupied(maxColor + 1);
vector<string> assignedRoom(C, "Unassigned");
for (int t = 1; t <= maxColor; t++) {
vector<int> exams;
for (int i = 0; i < C; i++)
if (color[i] == t) exams.push_back(i);
sort(exams.begin(), exams.end(), [&](int a, int b) {
if (studentsIn[a] != studentsIn[b]) return studentsIn[a] > studentsIn[b];
return a < b;
});
for (int e : exams) {
for (int ri : roomOrder) {
if (rooms[ri].second >= studentsIn[e] && roomOccupied[t].count(ri) == 0) {
assignedRoom[e] = rooms[ri].first;
roomOccupied[t].insert(ri);
break;
}
}
}
}
cout << "Course\tTimeSlot\tRoom\n";
for (int i = 0; i < C; i++) {
cout << courseNames[i] << "\tSlot " << color[i] << "\t" << assignedRoom[i] << "\n";
}
return 0;
}
