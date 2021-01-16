#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stack>
#include <time.h>
#include <limits>
#include <math.h>
#include <iterator>
#include <chrono>
#define X 0
#define Y 1


using namespace std;
double distanceV1(vector<pair<double, double>>& point, vector<pair<double, double>>& res);
double distanceV2(vector<pair<double, double>>& point, vector<pair<double, double>>& res);
double distanceV3(vector<pair<double, double>>& point, vector<pair<double, double>>& res);

void pointSort(vector<pair<double, double>>& res, int flag);
double calDistance(pair<double, double> p1, pair<double, double> p2);

int totalPoint = 0;
int main(int argc, char** argv) {
  
  // check if number of input is 2
  if (argc != 2) {
    cerr << "ERROR: Invalid Input" << endl;
    return EXIT_FAILURE;  // return failure
  }
  string filename = argv[1];
  
  vector<pair<double, double>> point;
  double x, y;
  // open the file
  ifstream fd(filename);
  // check if open successfully
  if (fd.is_open()) {
    // read by words
    
    while (fd >> x >> y) {
      // check current word is inside of map
      pair<double, double> temp(x, y);
      point.push_back(temp);
      totalPoint++;
    }
  } else {
    cerr << "ERROR: Unable to open file" << endl;
    return EXIT_FAILURE;  // return failure
  }
  // print each element out
  // close the file
  fd.close();


  if (point.size() <= 1) {
    cerr << "ERROR: Dataset size is less than 1." << endl;
    return EXIT_FAILURE;
  }
  // Sort all the points by x at top level
  vector<pair<double, double>> buf = point;
  pointSort(buf, X);
  cout << "Total points: " << totalPoint << endl;



  // version 1: O(n^2)
  vector<pair<double, double>> buf1 = buf;
  vector<pair<double, double>> pp;
  auto start_t = chrono::high_resolution_clock::now();
  double res1 = distanceV1(buf1,pp);
  auto end_t = chrono::high_resolution_clock::now();
  auto elapsed = end_t - start_t;
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  cout << "Version 1: Minimum Distance: " << res1 << " between ";
  cout << "("<<pp[0].first<< ", " <<pp[0].second << ") and ";
  cout << "("<<pp[1].first<< ", " <<pp[1].second << ")"<< endl;
  cout << "Time spent: " << time << "μs" << endl;

  
  // version 2: O(nlog^2n)
  vector<pair<double, double>> buf2 = buf;
  pp.clear();
  start_t = chrono::high_resolution_clock::now();
  double res2 = distanceV2(buf2, pp);
  end_t = chrono::high_resolution_clock::now();
  elapsed = end_t - start_t;
  time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  cout << "Version 2: Minimum Distance: " << res2 << " between ";
  cout << "("<<pp[0].first<< ", " <<pp[0].second << ") and ";
  cout << "("<<pp[1].first<< ", " <<pp[1].second << ")"<< endl;
  cout << "Time spent: " << time << "μs" << endl;


  // version 3: O(nlogn)
  vector<pair<double, double>> buf3 = buf;
  pp.clear();
  start_t = chrono::high_resolution_clock::now();
  double res3 = distanceV3(buf3, pp);
  end_t = chrono::high_resolution_clock::now();
  elapsed = end_t - start_t;
  time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  cout << "Version 3: Minimum Distance: " << res3 << " between ";
  cout << "("<<pp[1].first<< ", " <<pp[1].second << ") and ";
  cout << "("<<pp[0].first<< ", " <<pp[0].second << ")"<< endl;
  cout << "Time spent: " << time << "μs" << endl;

  

  return EXIT_SUCCESS;  // return success
}



double distanceV1(vector<pair<double, double>>& point, vector<pair<double, double>>& res) {
  double current;
  double min = numeric_limits<double>::infinity();
  for (uint i = 0; i < point.size(); i++) {
    for (uint j = i+1; j < point.size(); j++) {
        current = calDistance(point[i], point[j]);
        if (current == 0) {
          continue;
        }
        if (current < min) {
          res.clear();
          res.push_back(point[i]);
          res.push_back(point[j]);
          min = current;
        }
    }  
  }
  return min;
}

double distanceV2(vector<pair<double, double>>& point, vector<pair<double, double>>& res) {
  uint size = point.size();
  if(size == 1) {
    res.clear();
    res.push_back(point[0]);
    return numeric_limits<double>::infinity();
  } else if (size == 2) {
    res.clear();
    res.push_back(point[0]);
    res.push_back(point[1]);
    double dis = calDistance(point[0], point[1]);
    if (!dis) {
      dis = numeric_limits<double>::infinity();
    }
    return dis;
  }
  
  double L = point[size / 2 - 1].first / 2 + point[size / 2].first / 2;
  vector<pair<double, double>> lo, hi;
  for (uint i = 0; i < size/2; i++) {
    lo.push_back(point[i]);
    hi.push_back(point[size/2+i]);
  }
  if (size % 2 == 1) {
    hi.push_back(point[size-1]);
  }
  vector<pair<double, double>> tempRes0, tempRes1;
  double delta1 = distanceV2(lo, tempRes0);
  double delta2 = distanceV2(hi, tempRes1);
  //double delta = min(delta1, delta2);
  double delta;
  if (delta1 < delta2) {
    delta = delta1;
    res.clear();
    res.push_back(tempRes0[0]);
    res.push_back(tempRes0[1]);
  } else {
    delta = delta2;
    res.clear();
    res.push_back(tempRes1[0]);
    res.push_back(tempRes1[1]);
  }
  double rangeLo = L-delta;
  double rangeHi = L+delta;
  vector<pair<double, double>> boundary;
  for(auto& p: point) {
    if(p.first >= rangeLo && p.first <= rangeHi) {
      boundary.push_back(p);
    }
  }
  pointSort(boundary, Y);
  uint m = boundary.size();
  for(uint i = 0; i < m; i++) {
    uint k = 1;
    while (i + k < m && (boundary[i+k].second < (boundary[i].second + delta))) {
      //delta = min(delta, calDistance(boundary[i], boundary[i+k]));
      double bound = calDistance(boundary[i], boundary[i+k]);
      if (!bound) {
        bound = numeric_limits<double>::infinity();
      }
      if (delta > bound) {
        res.clear();
        res.push_back(boundary[i+k]);
        res.push_back(boundary[i]);
        delta = calDistance(boundary[i], boundary[i+k]);
      }
      k++;
    }
  }
  return delta;
}


double distanceV3(vector<pair<double, double>>& point, vector<pair<double, double>>& res) {
  vector<pair<double, double>> lo, hi, buf;                                                      
  uint size = point.size();
  if(size == 1) {
    res.clear();
    res.push_back(point[0]);
    return numeric_limits<double>::infinity();
  } else if (size == 2) {
    double dis = calDistance(point[0], point[1]);
    if (!dis) {
      dis = numeric_limits<double>::infinity();
    }
    if (point[0].second > point[1].second) {
      buf.push_back(point[1]);
      buf.push_back(point[0]);
      point = buf;
    }
    res.clear();
    res.push_back(point[0]);
    res.push_back(point[1]);
    return dis;
  }
  
  double L = point[size / 2 - 1].first / 2 + point[size / 2].first / 2;
  
  for (uint i = 0; i < size/2; i++) {
    lo.push_back(point[i]);
    hi.push_back(point[size/2+i]);
  }
  if (size % 2 == 1) {
    hi.push_back(point[size-1]);
  }
  vector<pair<double, double>> tempRes0, tempRes1;

  
  double delta1 = distanceV3(lo, tempRes0);
  double delta2 = distanceV3(hi, tempRes1);
  //double delta = min(delta1, delta2);
  double delta;
  if (delta1 < delta2) {
    delta = delta1;
    res.clear();
    res.push_back(tempRes0[0]);
    res.push_back(tempRes0[1]);
  } else {
    delta = delta2;
    res.clear();
    res.push_back(tempRes1[0]);
    res.push_back(tempRes1[1]);
  }
  double rangeLo = L-delta;
  double rangeHi = L+delta;
  
  //pointSort(boundary, Y);
  vector<pair<double, double>>::iterator it1 = lo.begin();
  vector<pair<double, double>>::iterator it2 = hi.begin();
  while(it1 != lo.end() && it2 != hi.end()) {
    pair<double, double> p1 = *it1;
    pair<double, double> p2 = *it2;
   
    if (p1.second < p2.second) {
      buf.push_back(p1);
      it1++;
    } else {
      buf.push_back(p2);
      it2++;
    }
    
  }
  while (it1 != lo.end()) {
    buf.push_back(*it1);
    it1++;
  }
  while (it2 != hi.end()) {
    buf.push_back(*it2);
    it2++;
  }
  point = buf;
  vector<pair<double, double>> boundary;
  for(auto& p: point) {
    if(p.first >= rangeLo && p.first <= rangeHi) {
      boundary.push_back(p);
    }
  }

  uint m = boundary.size();
  for(uint i = 0; i < m; i++) {
    uint k = 1;
    while (i + k < m && (boundary[i+k].second < (boundary[i].second + delta))) {
      //delta = min(delta, calDistance(boundary[i], boundary[i+k]));
      double bound = calDistance(boundary[i], boundary[i+k]);
      if (!bound) {
        bound = numeric_limits<double>::infinity();
      }
      if (delta > bound) {
        res.clear();
        res.push_back(boundary[i+k]);
        res.push_back(boundary[i]);
        delta = calDistance(boundary[i], boundary[i+k]);
      }
      k++;
    }
  }
  return delta;
}



double calDistance(pair<double, double> p1, pair<double, double> p2) {
  return sqrt(pow((p1.first - p2.first),2) + pow((p1.second - p2.second),2));
}

void pointSort(vector<pair<double, double>>& res, int flag) {
  int size = res.size();
  if (size == 0) {
    return;
  }
  if (size == 1) {
    return;
  }
  vector<pair<double, double>> lo, hi, buf;
  for (int i = 0; i < size/2; i++) {
    lo.push_back(res[i]);
    hi.push_back(res[size/2+i]);
  }
  if (size % 2 == 1) {
    hi.push_back(res[size-1]);
  }
  pointSort(lo, flag);
  pointSort(hi, flag);
  vector<pair<double, double>>::iterator it1 = lo.begin();
  vector<pair<double, double>>::iterator it2 = hi.begin();
  while(it1 != lo.end() && it2 != hi.end()) {
    pair<double, double> p1 = *it1;
    pair<double, double> p2 = *it2;
    // flag == 0, sort by x    
    if (!flag) {
      if (p1.first < p2.first) {
        buf.push_back(p1);
        it1++;
      } else {
        buf.push_back(p2);
        it2++;
      }
    // flag == 1, sort by y
    } else {
      if (p1.second < p2.second) {
        buf.push_back(p1);
        it1++;
      } else {
        buf.push_back(p2);
        it2++;
      }
    }
  }
  while (it1 != lo.end()) {
    buf.push_back(*it1);
    it1++;
  }
  while (it2 != hi.end()) {
    buf.push_back(*it2);
    it2++;
  }
  res = buf;
}


