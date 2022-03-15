#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <queue>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include "ship.h"

using namespace std;

bool isGoalState(Ship*);
void outputGoalSteps(Ship*);
Ship* loadManifest();
void emptyQueue(queue<Ship*> &);
queue<Ship*> sortQueue(queue<Ship*>);
Ship* searchAlgorithm(queue<Ship*> &);
vector<vector<Container*>> initializeVec();

map<double, vector<vector<Container*>>> duplicate;

int qSize = 0; // size of queue

int main(){
    Ship *problem;
    queue<Ship*> q;
    
    problem = loadManifest();
      
    q.push(problem);
      
    qSize = q.size();
     
    while(1){
      cout << q.size() << endl;
      if(q.empty()){break;}
      if(qSize < q.size()){qSize = q.size();}
     
      Ship* node = q.front();

      duplicate.insert(pair<double, vector<vector<Container*>>>(node->uniqueKey, node->grid)); //inserts node into the map

      if(isGoalState(node)){
        cout <<  "This is the fN value for the goal state" <<node->fN << endl;
        outputGoalSteps(node);
        break;
      }
      
      Ship *goal = searchAlgorithm(q);

      if(goal != NULL){
          cout << "GOAL STATE FOUND" << endl;
          outputGoalSteps(goal);
          break;
      }

    }

    // vector<pair<int,int>> tmp = problem->pickUp();
    // cout << "here";
    // for(int i = 0; i < tmp.size(); ++i){
    //     problem->dropDown(tmp[i]);
    // }

    return 0;
}

void emptyQueue(queue<Ship*> &q){
  while(!q.empty()){
    if(qSize < q.size()){
        qSize = q.size();
    } 
    q.pop(); 
  }
}

queue<Ship*> sortQueue(queue<Ship*> q){
  vector<Ship*> tempQ;
  Ship* temp;
  vector<double> allFn;
  
  if(q.empty() || q.size() < 2){return q;}

  while(!q.empty()){
    temp = q.front();
    tempQ.push_back(temp);
    allFn.push_back(temp->fN);
    q.pop();
  }
  for(int i = 0; i < allFn.size(); ++i){
    for(int j = i+1; j < allFn.size(); ++j){
      if(allFn.at(i) > allFn.at(j)){
        swap(allFn.at(i), allFn.at(j));
        swap(tempQ.at(i), tempQ.at(j));
      }
    }
  }

  for(int i = 0; i < tempQ.size(); ++i){
      q.push(tempQ.at(i));
  }

  return q;

}

Ship* searchAlgorithm(queue<Ship*> &q){
  Ship* parent = q.front();
  q.pop();

  vector<pair<int,int>> pick_up_indexes = parent->pickUp();
  vector<Ship*> children;
  for(int i = 0; i < pick_up_indexes.size(); ++i){
    vector<Ship*> drop_tmp = parent->dropDown(pick_up_indexes.at(i));
    for(int j = 0; j < drop_tmp.size(); ++j){
      children.push_back(drop_tmp.at(j));
    }
  }
//   for(int i = 0; i < children.size(); i++){
//       children.at(i)->print();
//   }

//   for(int i = 0; i < children.size();i++){
//     if(children.at(i) == NULL){cout << "NULL FOUND" << endl; continue;}
//     children.at(i)->calculate_hn();
//     // children.at(i)->print();
//   }
  for(int i = 0; i < children.size(); i++){
    if(duplicate.find(children.at(i)->uniqueKey) != duplicate.end()){ //checks if child has already been explored
    //   cout << "called erase: " << children.size() << endl;
      children.erase(children.begin() + i);
      --i;
      continue;
    }

    if(isGoalState(children.at(i))){
      emptyQueue(q);
      cout << "CALLED GOAL STATE IN SEARCH" << endl;
      //dont we want to return the goal state somewhere here?
      return children.at(i);
    }

    if(qSize < q.size()){qSize = q.size();}

  }
  for(int i = 0; i < children.size(); i++){
    q.push(children.at(i));
  }
  q = sortQueue(q);

  return NULL;
}

bool isGoalState(Ship* goal){
    // goal->print();
    double left = goal->find_mass_left();
    double right = goal->find_mass_right();
    double top = min(left, right);
    double bottom = max(left, right);
    double result = top/bottom;

    // cout << "GOAL: " << (result >= 0.9) << endl;

    return (result >= 0.9); 
}

void outputGoalSteps(Ship *goal){
    vector<Ship*> steps;
    Ship* tmp = goal;
    cout << "Fn: " << goal->fN << endl;
    while(tmp != NULL){
        steps.push_back(tmp);
        tmp = tmp->parent;
    }
    reverse(steps.begin(), steps.end());
    for(int i = 0; i < steps.size(); ++i){
        steps.at(i)->print();
    }
    return;
}

Ship* loadManifest(){
    fstream file;

    file.open("manifests/ShipCase3.txt");
    if(!file.is_open()){
        cout << "FAILED LOADING MANIFEST" << endl;
        return NULL;
    }
    vector<vector<Container*>> grid = initializeVec();
    string idx, weight, content;
    int w = 0, row = 0, column = 0;

    while(file >> idx){
        file >> weight;
        file >> content;

        w = stoi(weight.substr(1, 5));   
        row = stoi(idx.substr(1, 2)) - 1;
        column = stoi(idx.substr(4, 2)) - 1;

        grid[row][column] = new Container(w, content);
    }
    reverse(grid.begin(), grid.end());
    Ship* tmp = new Ship(grid);

    tmp->print();
    return tmp;
}

vector<vector<Container*>> initializeVec(){
    vector<vector<Container*>> tmp;
    tmp = {                                   // ij (row, column)
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 00 01 02 03 04 05 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},      // 10 11 12 13 14 15 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 20 21 22 23 24 25 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 30 31 32 33 34 35 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 40 41 42 43 44 45 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 50 51 52 53 54 55 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 60 61 62 63 64 65 
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")}      // 70 71 72 73 74 75
    };
    return tmp;
}