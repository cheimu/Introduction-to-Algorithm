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

#define K 12

using namespace std;

void Nussinov(string sequence);
int coreAlgorithm(string str, int** opt);
void traceback(string str, int** opt, int i, int j);
bool match(char t, char b);
string random_seq(int len);

int main(int argc, char** argv) {
  
  // check if number of input is 2
  if (argc != 1) {
    cerr << "ERROR: Invalid Input" << endl;
    return EXIT_FAILURE;  // return failure
  }
  string temp;
  vector<string> mem;
  // when for real simulation
  while (getline(cin, temp)) {
    // if not read successfully
    if (!cin.good()) {
      cout << "Failed to read!" << endl;
      return EXIT_FAILURE;  // return failure
    }
    for (string::iterator it = temp.begin(); it != temp.end(); ++it) {
      if (*it != 'A' && *it != 'G' && *it != 'C' && *it != 'U') {
        return EXIT_FAILURE;
      }
    }
    mem.push_back(temp);
  }

  
  for (auto str: mem) {
    
    Nussinov(str);
  } 

  // when for timing analysis
  /*for (int i = 4; i <= K; i++) {
    Nussinov(random_seq(pow(2,i)));
  }*/

 
  
  return EXIT_SUCCESS;  // return success
}

// wrapper function including core algorithm and traceback
void Nussinov(string sequence) {
  int size = sequence.length();

  int* buffer[size+1];
  int* OPT[size];
  for (int i = 0; i < size; i++) {
    buffer[i] = (int*) malloc(sizeof(int) * (size+1)); 
    for (int j = 0; j < size; j++) {
      buffer[i][j] = 0;
    }
    OPT[i] = buffer[i]+1;
  }
  
  auto start_t = chrono::high_resolution_clock::now();
  int optRes = coreAlgorithm(sequence, OPT);
  cout << endl;
  cout << "Sequence is: " << endl;
  cout << sequence << endl;
  traceback(sequence, OPT, 0, size-1);
  cout<<endl;
  auto end_t = chrono::high_resolution_clock::now();
  auto elapsed = end_t - start_t;
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  
  cout << "Length = " << size << ", Pairs = " << optRes << endl;
  cout << "Time = " << time << " μs" << endl;
  
  if (size <= 25) {
    cout << "OPT Table is: " << endl;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        cout << OPT[i][j] << " ";
      }
      cout << endl;
    }
  }
  cout << endl;
  for (int i = 0; i < size; i++) {
   free(buffer[i]); 
  }

}

int coreAlgorithm(string str, int** opt) {
  int size = str.length();
  const char* seq = str.c_str();
  for(int j = 0; j < size; j++) {
    for(int i = j; i >= 0; i--) {
      if (i >= j-4) {
        opt[i][j] = 0; 
      } else {
        int m = opt[i][j-1];
        for (int t = i; t < j-4; t++) {
          if (match(seq[t], seq[j])) {
            int newm = opt[i][t-1] + 1 + opt[t+1][j-1];
            m = max(m, newm);
          }
        }
        opt[i][j] = m;
      }
    }
  }
  return opt[0][size-1];
}

void traceback(string str, int** opt, int i, int j) {
  const char* seq = str.c_str();
  if (j < i) {
    return;
  } else if (j == i) {
    cout << ".";
  }else if (opt[i][j] == opt[i][j-1]) {
    traceback(str, opt, i, j-1);
    cout << ".";
    return;
  } else {
    for (int t = i; t < j; t++) {
      if (match(seq[t], seq[j])) {
        if (opt[i][j] == opt[i][t-1] + opt[t+1][j-1] + 1) {
          traceback(str, opt, i, t-1);
          cout << "(";
          traceback(str, opt, t+1, j-1);
          cout << ")";
          return;
        }
      }
    }
  }
  
}

// check whether two codes match or not
bool match(char t, char b) {
  return ((t == 'A' && b == 'U') || (t == 'U' && b == 'A') ||
          (t == 'C' && b == 'G') || (t == 'G' && b == 'C') );
}

// random AGCU sequence generator
string random_seq(int len) {
  vector<string> tb;
  tb.push_back("A");
  tb.push_back("G");
  tb.push_back("C");
  tb.push_back("U");
  string res = "";
  for (int i = 0; i < len; i++) {
    res.append(tb[rand() % 4]);
  }
  return res;
}


