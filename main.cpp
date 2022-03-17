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
void updateManifest(Ship*);
void emptyQueue(queue<Ship*> &);
queue<Ship*> sortQueue(queue<Ship*>);
Ship* balanceAlgorithm(queue<Ship*> &);
Ship* unloadAndLoadAlgorithm(vector<pair<int,int>>, Ship*, vector<Container*> c);
vector<vector<Container*>> initializeVec();
vector<vector<Container*>> intializeBuf();
void moveToBuffer(vector<vector<Container*>>&, Container*);
void outputBuffer(vector<vector<Container*>>);
pair<int,int> bufferEmpty(vector<vector<Container*>>);
void removeFromBuffer(vector<vector<Container*>>&, pair<int,int> idx);
int availableColumn(vector<pair<int,int>>);
bool compare(Ship* a, Ship* b);



map<double, vector<vector<Container*>>> duplicate;
map<double, vector<vector<Container*>>>::iterator it;
        
int qSize = 0; // size of queue

int main(){
    Ship *problem;
    queue<Ship*> q;
    
    problem = loadManifest();
      
    q.push(problem);
      
    qSize = q.size();

    vector<pair<int,int>> unload = {make_pair(7,4), make_pair(7,3)};
    vector<Container*> load = {new Container(153, "Nat"), new Container(2321, "Rat")};
    // vector<pair<int,int>> tmp = problem->pickUp();

    // for(int i = 0; i < tmp.size(); ++i){
    //     cout << tmp[i].first << ' ' << tmp[i].second << endl;
    // }
    // problem->print();
    Ship* test = unloadAndLoadAlgorithm(unload, problem, load);

    outputGoalSteps(test);
    // while(1){
    //   if(q.empty()){break;}
    //   if(qSize < q.size()){qSize = q.size();}
     
    //   Ship* node = q.front();

    //   duplicate.insert(pair<double, vector<vector<Container*>>>(node->uniqueKey, node->grid)); //inserts node into the map

    //   if(isGoalState(node)){
    //     cout << "GOAL STATE FOUND" << endl;
    //     outputGoalSteps(node);
    //     break;
    //   }
      
    //   Ship *goal = balanceAlgorithm(q);

    //   if(goal != NULL){
    //       cout << "GOAL STATE FOUND" << endl;
    //       outputGoalSteps(goal);
    //       break;
    //   }

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
    allFn.push_back(temp->getCost());
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

Ship* balanceAlgorithm(queue<Ship*> &q){
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

  for(int i = 0; i < children.size(); i++){
    if(duplicate.find(children.at(i)->getUniqueKey()) != duplicate.end()){ //checks if child has already been explored
      children.erase(children.begin() + i);
      --i;
      continue;
    }

    if(isGoalState(children.at(i))){
      emptyQueue(q);
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
    double left = goal->find_mass_left();
    double right = goal->find_mass_right();
    double top = min(left, right);
    double bottom = max(left, right);
    double result = top/bottom;

    return (result >= 0.9); 
}

void outputGoalSteps(Ship *goal){
    vector<Ship*> steps;
    Ship* tmp = goal;
    
    while(tmp != NULL){
        steps.push_back(tmp);
        tmp = tmp->getParent();
    }
    reverse(steps.begin(), steps.end());
    for(int i = 0; i < steps.size(); ++i){
        steps.at(i)->print();
    }
    return;
}

Ship* loadManifest(){
    fstream file;

    file.open("manifests/ShipCase10.txt");
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

    // tmp->print();
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

vector<vector<Container*>> intializeBuf(){
    vector<vector<Container*>> buf = {                                   // ij (row, column)
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},      
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},      
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")} 
    };
    return buf; 
}

Ship* unloadAndLoadAlgorithm(vector<pair<int,int>> idxs, Ship* p, vector<Container*> c){
    vector<vector<Container*>> buffer = intializeBuf();
    Ship *temp = p;
    vector<Ship*> steps;
    temp->trickleDown();
    steps.push_back(new Ship(temp));
    for(int i = 0; i < idxs.size(); ++i){
        vector<pair<Ship*, Container*>> tmp = temp->unloadContainer(idxs, i);
        for(int j = 0; j < tmp.size(); ++j){
            if(tmp.at(j).first == NULL && tmp.at(j).second == NULL){continue;}
            else if(tmp.at(j).first == NULL && tmp.at(j).second != NULL){continue;}
            else{
                temp = new Ship(tmp.at(j).first);
                temp->trickleDown();
                steps.push_back(new Ship(temp));
            }
        }
        
        for(int k = 0; k < tmp.size(); ++k){
            if(tmp.at(k).first == NULL && tmp.at(k).second == NULL && k == tmp.size()-1){ //container to load
                vector<vector<Container*>> g = temp->getGrid(); 
                temp->trickleDown();
                steps.push_back(new Ship(temp));
                temp->removeContainer(g[idxs[i].first][idxs[i].second]);
                steps.push_back(new Ship(temp));
            }
            else if(tmp.at(k).first == NULL && tmp.at(k).second != NULL){ //container to buffer
                moveToBuffer(buffer, tmp.at(k).second);
                temp->trickleDown();
                temp->removeContainer(tmp.at(k).second);
            }
        }
        idxs.erase(idxs.begin()+i);
        --i;
    }
    pair<int, int> emptyBuffer = bufferEmpty(buffer);
    while(emptyBuffer.first != -1){
        temp->addContainer(buffer[emptyBuffer.first][emptyBuffer.second], -1);
        temp->trickleDown();
        steps.push_back(new Ship(temp));
        removeFromBuffer(buffer, emptyBuffer);
        emptyBuffer = bufferEmpty(buffer);
    }
    for(int i = 0; i < steps.size()-1; ++i){
        if(compare(steps.at(i),steps.at(i+1))){
            steps.erase(steps.begin()+i);
            --i;
        }
    }

    for(int i = 0; i < c.size(); ++i){
        Container *loadingContainer = new Container(c.at(i)->weight, c.at(i)->contents);
        int n = availableColumn(idxs);
        temp->addContainer(loadingContainer, n);
        temp->trickleDown();
        steps.push_back(new Ship(temp));
    }

    for(int i = steps.size()-1; i > 0; --i){
        steps[i]->setParent(steps[i-1]);
    }
    steps.at(0)->setParent(NULL);
    return steps.at(steps.size()-1);
}

void moveToBuffer(vector<vector<Container*>> &buf, Container *c){
    for(int i = 0; i < buf[0].size(); ++i){
        for(int j = buf.size()-1; j > -1; --j){
            if(buf[j][i]->weight == -1){
                buf[j][i] = c;
                return;
            }
        }
    }
}

void outputBuffer(vector<vector<Container*>> b){
    for(int i = 0; i < b.size(); ++i ){
        for(int j = 0; j < b.at(0).size(); j++){
            cout << b.at(i).at(j)->weight << '\t';
        }
        cout << endl;
    }
    cout << endl;
}

pair<int,int> bufferEmpty(vector<vector<Container*>> buf){
    pair<int, int> idx = make_pair(-1,-1);
    for(int i = 0; i < buf[0].size(); ++i){
        for(int j = buf.size()-1; j > -1; --j){
            if(buf[j][i]->weight != -1){
                idx.first = j;
                idx.second = i;
                return idx;
            }
        }
    }
    return idx;
}

void removeFromBuffer(vector<vector<Container*>> &buf, pair<int,int> idx){
    buf[idx.first][idx.second] = new Container(0, "UNUSED");
}

int availableColumn(vector<pair<int,int>> idxs){
    int n = -1;
    bool exists = false;
    for(int i = 0; i < 12; ++i){
        exists = false;
        for(int j = 0; j < idxs.size(); ++j){
            if(idxs.at(j).second == i){
                exists = true;
            }
        }
        if(!exists){
            return i;
        }
    }
    return n;
}

bool compare(Ship* a, Ship* b){
    vector<vector<Container*>> gridA = a->getGrid();
    vector<vector<Container*>> gridB = b->getGrid();

    for(int i = 0; i < gridA.size(); ++i){
        for(int j = 0; j < gridA[0].size(); ++j){
            if(gridA[i][j]->weight != gridB[i][j]->weight){
                return false;
            }
        }
    }
    return true;
}