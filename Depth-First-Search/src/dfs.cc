#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stack>
#include <time.h>
#define START 0

using namespace std;
int dfs(map<int, vector<int>>& nodeMap, int totalNode);
int dfs_find(map<int, vector<int>>& nodeMap, vector<int>& dfsNumber, vector<int>& lowNumber, 
                                  int& current, int& parent, int& dfsCounter, vector<int>& hasPrint, 
                                  stack<pair<int, int>>& edgeStack);
int totalArt = 0;
double edgeCount = 0;
int componentNum = 1;
int main(int argc, char** argv) {
  
  // check if number of input is 2
  if (argc != 2) {
    cerr << "ERROR: Invalid Input" << endl;
    return EXIT_FAILURE;  // return failure
  }
  string filename = argv[1];
  int node;
  int neighbor;
  int totalNode = 0;
  map<int, vector<int>> nodeMap;
  // open the file
  ifstream fd(filename);
  // check if open successfully
  if (fd.is_open()) {
    // read by words
    fd >> totalNode;
    while (fd >> node >> neighbor) {
      // check current word is inside of map
      if (nodeMap.count(node) > 0) {
        nodeMap[node].push_back(neighbor);
      } else {
        // if not, insert it into map
        vector<int> temp;
        temp.push_back(neighbor);
        nodeMap.insert(pair<int, vector<int>>(node, temp));
      }
      if (nodeMap.count(neighbor) > 0) {
        nodeMap[neighbor].push_back(node);
      } else {
        // if not, insert it into map
        vector<int> temp;
        temp.push_back(node);
        nodeMap.insert(pair<int, vector<int>>(neighbor, temp));
      }
    }
  } else {
    cerr << "ERROR: Unable to open file" << endl;
    return EXIT_FAILURE;  // return failure
  }
  // print each element out
  // close the file
  fd.close();
  
  clock_t start_tick, end_tick;
  double elapsed;

  start_tick = clock();
  
  
  if (dfs(nodeMap, totalNode) < 0) {
    return EXIT_FAILURE;
  }
  end_tick = clock();
  elapsed = (end_tick - start_tick) / (double)CLOCKS_PER_SEC;
  cout << "Time: " << elapsed << "s" << endl;
  cout << "Total nodes: " << totalNode << endl;
  for (auto node: nodeMap) {
    edgeCount+= node.second.size(); 
  }
  cout << "Total edge: " << edgeCount / 2 << endl;
  cout << "Total biconnected components: " << componentNum << endl;
  cout << "Total articulation points: " << totalArt << endl;
  return EXIT_SUCCESS;  // return success
}


int dfs(map<int, vector<int>>& nodeMap, int totalNode) {

    int dfsCounter = 0;
    stack<pair<int, int>> edgeStack;
    vector<int> dfsNumber(totalNode);
    vector<int> lowNumber(totalNode);
    vector<int> hasPrint(totalNode);
    int current = START;
    int parent = -1;
    for(auto& counter: dfsNumber) {
      counter = -1;
    }
    for(auto& counter: lowNumber) {
      counter = -1;
    }
    for(auto& counter: hasPrint) {
      counter = -1;
    }
    dfs_find(nodeMap, dfsNumber, lowNumber, current, parent, dfsCounter, hasPrint, edgeStack);


    return EXIT_SUCCESS;
    
}

int dfs_find(map<int, vector<int>>& nodeMap, vector<int>& dfsNumber, vector<int>& lowNumber, 
                                      int& current, int& parent, int& dfsCounter, vector<int>& hasPrint, 
                                                            stack<pair<int, int>>& edgeStack) {
    int counter = 0;
    dfsCounter+=1;    
    dfsNumber[current] = dfsCounter;
    lowNumber[current] = dfsCounter;

    for (auto neighbor : nodeMap[current]) {
      int cur = neighbor;
      pair<int, int> curEdge(current, neighbor);
      if (dfsNumber[neighbor] == -1) {
        counter+=1;
        
        if (parent != neighbor) {
          edgeStack.push(curEdge);
        }
        dfs_find(nodeMap, dfsNumber, lowNumber, cur, current, dfsCounter, hasPrint, edgeStack);
        lowNumber[current] = min(lowNumber[current], lowNumber[neighbor]);
        if (lowNumber[neighbor] >= dfsNumber[current] && current != START){
            if (hasPrint[current] == -1) {              
              totalArt+=1;
            }
            cout << current << " is articulation point" << endl;
            pair<int, int> temp = edgeStack.top();
            cout << "Biconnected component: ";
            while ((temp.first != curEdge.first) || (temp.second != curEdge.second)) {
              cout << "(" << temp.first << ", " << temp.second << ")";  
              edgeStack.pop();
              temp = edgeStack.top();
            }
            temp = edgeStack.top();
            cout << "(" << temp.first << ", " << temp.second << ")";  
            edgeStack.pop();
            cout << " " << endl;
            hasPrint[current] = 1;
            componentNum+=1;
        }
      } else {
        if (parent != neighbor) {
          if (dfsNumber[current] > dfsNumber[neighbor]) {
            edgeStack.push(curEdge);
            lowNumber[current] = min(lowNumber[current], dfsNumber[neighbor]);
          }
        }
      }
    }
    if (current == START) {
      if (counter > 1) {
        cout << current << " is art point " << endl;
       
        
      }
      cout << "Biconnected component: ";
      while(!edgeStack.empty()) {
          pair<int, int> temp = edgeStack.top();
          cout << "(" << temp.first << ", " << temp.second << ")";  
          edgeStack.pop();
          
      }
      cout << endl;
    }

    return 1;

}


