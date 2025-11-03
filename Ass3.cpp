#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;
struct Item {
string name;
7
double weight;
double value;
int priority;
bool isDivisible;
double ratio; // value/weight
};
// Comparator: sort by priority first (ascending), then value/weight descending
bool cmp(Item a, Item b) {
if (a.priority != b.priority)
return a.priority < b.priority;
return a.ratio > b.ratio;
}
int main() {
double W;
cout << "Enter maximum boat capacity (kg): ";
cin >> W;
vector<Item> items = {
{"First Aid Box", 2.0, 70, 1, false}, {"Medicine Kit", 6.0, 120, 1, false}, {"Food Pack", 4.0, 80, 2, true}, 8
{"Water Bottles", 5.0, 50, 2, true}, {"Blankets", 3.0, 40, 3, false}
};
// Compute value/weight ratio
for (auto &item : items)
item.ratio = item.value / item.weight;
// Sort items
sort(items.begin(), items.end(), cmp);
// Display sorted items
cout << fixed << setprecision(2);
cout << "\nSorted Items (by priority, then value/weight):\n";
cout << left << setw(20) << "Item" << setw(8) << "Weight" << setw(8) << "Value" << setw(8) << "Priority" << setw(12) << "Value/Weight" << setw(10) << "Type" << endl;
cout << "-----------------------------------------------------------------------------\n";
for (auto &item : items)
9
cout << setw(20) << item.name
<< setw(8) << item.weight
<< setw(8) << item.value
<< setw(8) << item.priority
<< setw(12) << item.ratio
<< setw(10) << (item.isDivisible ? "Divisible" : "Indivisible") << endl;
// Select items
double remainingCapacity = W;
double totalUtility = 0;
cout << "\nItems selected for transport:\n";
cout << "--------------------------------------------------\n";
for (auto &item : items) {
if (remainingCapacity <= 0)
break;
double takenWeight = 0;
double utility = 0;
if (item.weight <= remainingCapacity) {
takenWeight = item.weight;
utility = item.value;
10
} else if (item.isDivisible) {
takenWeight = remainingCapacity;
utility = item.ratio * takenWeight;
} else {
continue; // skip indivisible item if it doesn't fit
}
remainingCapacity -= takenWeight;
totalUtility += utility;
cout << "- " << item.name << ": " << takenWeight << " kg, Utility = " << utility
<< ", Type = " << (item.isDivisible ? "Divisible" : "Indivisible") << endl;
}
cout << "===== Final Report =====\n";
cout << "Total weight carried: " << W - remainingCapacity << " kg\n";
cout << "Total utility value carried: " << totalUtility << " units\n";
return 0;
}
