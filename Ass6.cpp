#include <bits/stdc++.h>
using namespace std;
// Structure for items
struct Item {
 int id;
 int weight;
 int value;
 bool perishable;
};
// ================== Brute Force ==================
int knapsackBruteForce(const vector<Item>& items, int W, int idx = 0) {
 if (idx == (int)items.size() || W <= 0) return 0;
 // Option 1: skip item
 int notTake = knapsackBruteForce(items, W, idx + 1);
 // Option 2: take item if fits
 int take = 0;
 if (items[idx].weight <= W)
 take = items[idx].value + knapsackBruteForce(items, W - items[idx].weight, idx + 1);
 return max(take, notTake);
}
// ================== Dynamic Programming ==================
6
pair<int, vector<int>> knapsackDP(const vector<Item>& items, int W) {
 int N = items.size();
 vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));
 for (int i = 1; i <= N; ++i) {
 for (int w = 0; w <= W; ++w) {
 dp[i][w] = dp[i-1][w]; // not take
 if (items[i-1].weight <= w)
 dp[i][w] = max(dp[i][w], dp[i-1][w - items[i-1].weight] + items[i-1].value);
 }
 }
 // Backtrack to find selected items
 int res = dp[N][W];
 vector<int> chosen;
 int w = W;
 for (int i = N; i > 0 && res > 0; --i) {
 if (res == dp[i-1][w]) continue;
 else {
 chosen.push_back(items[i-1].id);
 res -= items[i-1].value;
 w -= items[i-1].weight;
 }
 }
 reverse(chosen.begin(), chosen.end());
 return {dp[N][W], chosen};
}
// ================== Greedy (Heuristic) ==================
pair<int, vector<int>> greedyKnapsack(vector<Item> items, int W) {
 sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
 double r1 = (double)a.value / a.weight;
7
 double r2 = (double)b.value / b.weight;
 return r1 > r2;
 });
 int totalValue = 0, totalWeight = 0;
 vector<int> chosen;
 for (auto &item : items) {
 if (totalWeight + item.weight <= W) {
 totalWeight += item.weight;
 totalValue += item.value;
 chosen.push_back(item.id);
 }
 }
 return {totalValue, chosen};
}
// ================== Main ==================
int main() {
 ios::sync_with_stdio(false);
 cin.tie(nullptr);
 int N, W;
 cout << "Enter number of items and truck capacity: ";
 cin >> N >> W;
 vector<Item> items(N);
 cout << "Enter weight, utility, perishability(1=perishable,0=non):\n";
 for (int i = 0; i < N; ++i) {
 cin >> items[i].weight >> items[i].value >> items[i].perishable;
 items[i].id = i + 1;
8
 // Increase priority for perishables
 if (items[i].perishable)
 items[i].value = (int)(items[i].value * 1.5);
 }
 cout << "\n========= Dynamic Programming Solution =========\n";
 auto [bestVal, selectedDP] = knapsackDP(items, W);
 cout << "Max Utility (DP): " << bestVal << "\nItems chosen: ";
 for (int id : selectedDP) cout << id << " ";
 cout << "\n";
 if (N <= 20) {
 cout << "\n========= Brute Force (Verification) =========\n";
 cout << "Max Utility (Brute Force): " << knapsackBruteForce(items, W) << "\n";
 }
 cout << "\n========= Greedy Heuristic (Fast Approximation) =========\n";
 auto [greedyVal, selectedGreedy] = greedyKnapsack(items, W);
 cout << "Approx Utility (Greedy): " << greedyVal << "\nItems chosen: ";
 for (int id : selectedGreedy) cout << id << " ";
 cout << "\n";
 cout << "\n========= Performance Comparison =========\n";
 cout << "DP: Optimal, O(N*W)\n";
 cout << "Greedy: Fast, may miss optimum\n";
 cout << "Brute Force: Exact but exponential\n";
 cout << "---------------------------------------------\n";
 cout << "Model easily extendable to multi-truck scenario.\n";
 return 0;
}
