#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;
struct Movie {
string title;
float rating;
int releaseYear;
int views;
void display() const {
cout << title << " |Score: " << rating
<< " | Year: " << releaseYear
<< " | Popularity: " << views << endl;
}
};
void quickSorter(vector<Movie> &data, int low, int high, bool (*compare)(const Movie &, const Movie
&)) {
if (low >= high) return;
int pivotPos = low;
Movie pivotElement = data[high];
for (int j = low; j < high; j++) {
if (compare(data[j], pivotElement)) {
swap(data[j], data[pivotPos]);
pivotPos++;
}
}
swap(data[pivotPos], data[high]);
quickSorter(data, low, pivotPos - 1, compare);
quickSorter(data, pivotPos + 1, high, compare);
}
bool byRatingAsc(const Movie &x, const Movie &y) { return x.rating < y.rating; }
bool byRatingDesc(const Movie &x, const Movie &y) { return x.rating > y.rating; }
bool byYearAsc(const Movie &x, const Movie &y) { return x.releaseYear < y.releaseYear; }
bool byYearDesc(const Movie &x, const Movie &y) { return x.releaseYear > y.releaseYear; }
bool byViewsAsc(const Movie &x, const Movie &y) { return x.views < y.views; }
bool byViewsDesc(const Movie &x, const Movie &y) { return x.views > y.views; }
vector<Movie> readMovieCSV(const string &path) {
vector<Movie> movieList;
ifstream input(path);
if (!input.is_open()) {
cerr << "Error: Unable to open file " << path << endl;
return movieList;
}
string line;
getline(input, line);
while (getline(input, line)) {
stringstream ss(line);
string titleStr, ratingStr, yearStr, viewsStr;
if (!getline(ss, titleStr, ',') ||
!getline(ss, ratingStr, ',') ||
!getline(ss, yearStr, ',') ||
!getline(ss, viewsStr, ',')) {
continue; /
}
try {
Movie m;
m.title = titleStr;
m.rating = stof(ratingStr);
m.releaseYear = stoi(yearStr);
m.views = stoi(viewsStr);
movieList.push_back(m);
} catch (...) {
continue;
}
}
return movieList;
}
int main() {
string filePath = "movies_real_titles.csv";
vector<Movie> movies = readMovieCSV(filePath);
if (movies.empty()) {
cout << "No movie records found!\n";
return 0;
}
cout << "Sort by which attribute? (rating/year/views): ";
string userChoice;
cin >> userChoice;
transform(userChoice.begin(), userChoice.end(), userChoice.begin(), ::tolower);
cout << "Sort order? (asc/desc): ";
string orderChoice;
cin >> orderChoice;
transform(orderChoice.begin(), orderChoice.end(), orderChoice.begin(), ::tolower);
bool (*cmp)(const Movie &, const Movie &);
if (userChoice == "rating") {
cmp = (orderChoice == "desc") ? byRatingDesc : byRatingAsc;
} else if (userChoice == "year") {
cmp = (orderChoice == "desc") ? byYearDesc : byYearAsc;
} else if (userChoice == "views") {
cmp = (orderChoice == "desc") ? byViewsDesc : byViewsAsc;
} else {
cout << "Invalid input! Default sorting by rating ascending.\n";
cmp = byRatingAsc;
}
auto begin = high_resolution_clock::now();
quickSorter(movies, 0, movies.size() - 1, cmp);
auto end = high_resolution_clock::now();
cout << "\n--- Top 10 Movies Sorted by " << userChoice
<< " (" << orderChoice << ") ---\n";
int lastIdx = max(0, (int)movies.size() - 10);
for (int i = movies.size() - 1; i >= lastIdx; --i) {
movies[i].display();
}
duration<double> totalTime = end - begin;
cout << "\nProcessed " << movies.size() << " entries in "
<< totalTime.count() << " seconds.\n";
return 0;
}
