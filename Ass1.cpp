#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;
// Structure to hold individual order details
struct OrderData {
 string orderId;
 time_t timestamp;
};
// Function to generate simulated purchases
void createPurchaseData(vector<OrderData> &orderRecords, int totalOrders) {
 tm startTime = {};
 startTime.tm_year = 2024 - 1900;
 startTime.tm_mon = 10; // November
 startTime.tm_mday = 15;
 startTime.tm_hour = 9; // Starting from 09:00 AM
 time_t baseTime = mktime(&startTime);
 for (int i = 0; i < totalOrders; ++i) {
 int timeOffsetMinutes = rand() % 100000; // Changed offset range for different spread
 time_t orderTime = baseTime + (timeOffsetMinutes * 60);
 OrderData newOrder;
 newOrder.orderId = "ORD" + to_string(10000 + i); // Changed ID format
 newOrder.timestamp = orderTime;
 orderRecords.push_back(newOrder);
 }
}
// Merges two sorted halves of the array
void mergeSections(vector<OrderData> &orders, int left, int mid, int right) {
 int lenLeft = mid - left + 1;
 int lenRight = right - mid;
 vector<OrderData> leftPart(orders.begin() + left, orders.begin() + mid + 1);
 vector<OrderData> rightPart(orders.begin() + mid + 1, orders.begin() + right + 1);
 int i = 0, j = 0, k = left;
 while (i < lenLeft && j < lenRight) {
 if (leftPart[i].timestamp <= rightPart[j].timestamp)
 orders[k++] = leftPart[i++];
 else
 orders[k++] = rightPart[j++];
 }
 while (i < lenLeft) orders[k++] = leftPart[i++];
 while (j < lenRight) orders[k++] = rightPart[j++];
}
// Recursive merge sort
void mergeSortOrders(vector<OrderData> &orders, int left, int right) {
 if (left >= right) return;
 int mid = left + (right - left) / 2;
 mergeSortOrders(orders, left, mid);
 mergeSortOrders(orders, mid + 1, right);
 mergeSections(orders, left, mid, right);
}
// Display top 'n' sorted orders
void displayTopOrders(const vector<OrderData> &orders, int count) {
 cout << "\n--- Top " << count << " Earliest Orders ---\n";
 for (int i = 0; i < count && i < (int)orders.size(); ++i) {
 char formattedTime[30];
 tm *timeInfo = gmtime(&orders[i].timestamp);
 strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%dT%H:%M:%SZ", timeInfo);
 cout << "Order: " << setw(8) << left << orders[i].orderId
 << " | Timestamp: " << formattedTime << '\n';
 }
}
int main() {
 srand(static_cast<unsigned>(time(nullptr)));
 const int TOTAL_ORDERS = 750000;
 vector<OrderData> allOrders;
 allOrders.reserve(TOTAL_ORDERS);
 createPurchaseData(allOrders, TOTAL_ORDERS);
 mergeSortOrders(allOrders, 0, TOTAL_ORDERS - 1);
 displayTopOrders(allOrders, 10);
 return 0;
}
