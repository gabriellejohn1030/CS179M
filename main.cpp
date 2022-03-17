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
Ship* loadManifest(string);
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
void buf_print(vector<vector<Container*>>);
pair<int, int> first_buffer_loc(vector<vector<Container*>>);
vector<Container*> sort_buf(vector<Container*>);
vector<string> SIFT(Ship *);



map<double, vector<vector<Container*>>> duplicate;
map<double, vector<vector<Container*>>>::iterator it;
        
int qSize = 0; // size of queue

int main(){
    Ship *problem;
    queue<Ship*> q;
    
    problem = loadManifest();
      
    q.push(problem);
      
    qSize = q.size();


    // SIFT(problem);

    // vector<pair<int,int>> unload = {make_pair(7,4), make_pair(7,3)};
    // vector<Container*> load = {new Container(5432, "Bat"), new Container(54, "Rat")};

    // Ship* goal = unloadAndLoadAlgorithm(unload, problem, load);

    // outputGoalSteps(goal);

      //the row 

      /*
            00,01,02,03,04,05,06,07,08,09,010,011,012,013,014,015,016,017,018,019,020,021,022,023
            10,11,12,13,14,15,16,17,18,19,110,111,112,113,114,115,116,117,118,119,120,121,122,123
            20,21,22,23,24,25,26,27,28,29,210,211,212,213,214,215,216,217,218,219,220,221,222,223
            30,31,32,33,34,35,36,37,38,39,310,311,312,313,314,315,316,317,318,319,320,321,322,323
      */

    // if(problem->check_SIFT()){
    //   //do sift operation instead of other operation
    //   cout << "DOING SIFT OPERATION" << endl;
    //   vector<string> move_output;
    //   // Move container from row 1 and column 5 to row 7 and column 6 (of buffer)
    //   vector<vector<Container*>> buf =  intializeBuf(); 
    //   buf_print(buf);
    //   vector<Container*> buf_sorted;
    //   int first = 0;
    //   int second = 0;
    //   int third = 0;
    //   int fourth = 0;
     
    //   Ship *move = new Ship(problem->getGrid());
    //   vector<vector<Container*>> g = move->getGrid();
    //   for(int i = 0; i < move->grid.size();i++){        
    //     for(int j = 0; j < move->grid.at(0).size(); ++j){            
    //         if(move->grid.at(i).at(j)->weight != -1 && move->grid.at(i).at(j)->weight != -2){
    //           // need helper to find first avalible in bottom row of buf
    //           first = i+1;
    //           second = j+1;
    //           pair<int, int> location = first_buffer_loc(buf);    
    //           third = location.first + 1;
    //           fourth = location.second + 1;
    //           string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " to row " + to_string(third) + " and column " + to_string(fourth) + " of buffer ";    
    //           move_output.push_back(directions);     
    //           buf.at(location.first).at(location.second)->weight = move->grid.at(i).at(j)->weight;              
    //           Container* element = new Container;              
    //           element->weight = move->grid.at(i).at(j)->weight;              
    //           element->contents = move->grid.at(i).at(j)->contents;              
    //           buf_sorted.push_back(element);
    //           move->grid.at(i).at(j)->weight = -1;
    //           move->print();
    //           buf_print(buf);

    //         }
    //     }
    //   }
    //   move->setGrid(g); 

      
    //   // buf_print(buf);    
    //   sort_buf(buf_sorted);
    //   for(int i = 0; i < buf_sorted.size(); i++){
    //     cout << "this is the output for sorted: " << buf_sorted.at(i)->weight << endl;
    //   }

    //   int buf_population = buf_sorted.size();

    //   int z = 0;
    //   int buf_mod = 0;
    //   second = 26;
    //   for(int i = move->grid.size()-1; i > 0;i--){
    //     for(int j = ((move->grid.at(0).size()/2) - 1); j > 0; j--){
    //         buf_mod = z % 2;
    //         if(move->grid.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 0  && move->grid.at(i).at(j)->weight != -2){
    //           move->grid.at(i).at(j)->weight = buf_sorted.at(z)->weight;
    //           move->grid.at(i).at(j)->contents = buf_sorted.at(z)->contents;
    //           first = 4;
    //           second = second - 2;
    //           third = i + 1;
    //           fourth = j + 1;
    //           string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);    
    //           move_output.push_back(directions); 
    //           z = z + 2;
    //           move->print();
    //         }
    //     }
    //   } 
      
    //   move->print();

    //   z = 1;
    //   buf_mod = 0;
    //   second = 25;
    //   for(int i = move->grid.size()-1; i > 0;i--){
    //     for(int j = ((move->grid.at(0).size()/2)); j < move->grid.at(0).size(); j++){
    //         buf_mod = z % 2;
    //         if(move->grid.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 1 && move->grid.at(i).at(j)->weight != -2 ){
    //           move->grid.at(i).at(j)->weight = buf_sorted.at(z)->weight;
    //           first = 4;
    //           second = second - 2;
    //           third = i + 1;
    //           fourth = j + 1;
    //           string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);    
    //           move_output.push_back(directions); 
    //           z = z + 2;
    //           move->print();
    //         }
    //     }
    //   } 

    //   for(int i = 0; i < move_output.size(); i++){
    //     cout << move_output.at(i) << endl;
    //   }


    //   return 0;
    // }


    // while(1){
    //  // cout << q.size() << endl;
    //   if(q.empty()){break;}
    //   if(qSize < q.size()){qSize = q.size();}
     
    // //   Ship* node = q.front();

    // //   duplicate.insert(pair<double, vector<vector<Container*>>>(node->uniqueKey, node->grid)); //inserts node into the map

    //   if(isGoalState(node)){
    //   //  cout <<  "This is the fN value for the goal state" <<node->fN << endl;
    //     outputGoalSteps(node);
    //     break;
    //   }
      
    //   Ship *goal = searchAlgorithm(q);

    // //   if(goal != NULL){
    // //       cout << "GOAL STATE FOUND" << endl;
    // //       outputGoalSteps(goal);
    // //       break;
    // //   }

    // // }

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
// void outputGoalSteps(Ship *goal){
//     vector<Ship*> steps;
//     vector<string> moves;
//     Ship* tmp = goal;
//     Ship* parent;
//     Ship* child;
    
//     while(tmp != NULL){
//         steps.push_back(tmp);
//         tmp = tmp->parent;
//     }
//     reverse(steps.begin(), steps.end());
    
//     for(int i = 0; i < steps.size() - 1; ++i){

//         parent = steps.at(i);
//         child = steps.at(i + 1);

//         steps.at(i)->print();
//         //moves.push_back( steps.at(i)->outputMoves(parent, child));
        
       
//     }
//     steps.at(steps.size()-1)->print();
//     return;
// }


Ship* loadManifest(string fileName){
    fstream file;

    file.open(fileName);
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

void buf_print(vector<vector<Container*>> buf){
    for(int i =0; i < buf.size(); ++i ){
        for(int j = 0; j < buf.at(0).size(); j++){
            cout << buf.at(i).at(j)->weight << "   ";
        }
        cout << endl;
    }
    cout << endl;

}

pair<int, int> first_buffer_loc(vector<vector<Container*>> buf){
  for(int i = buf.size()-1; i > 0; i--){
    for(int j = buf.at(i).size()-1; j > 0; j--){
      if(buf.at(i).at(j)->weight == -1){
        return make_pair(i,j);
      }
    }
  }

  return make_pair(0,0);
}


vector<Container*> sort_buf(vector<Container*> buf){
    int min_idx; 
    for (int i = 0; i < buf.size()-1; i++){ 
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (int j = i+1; j < buf.size(); j++){ 
          if (buf.at(j)->weight < buf.at(i)->weight){
              min_idx = j; 
          } 
        }
        // Swap the found minimum element with the first element 
        swap(buf.at(min_idx), buf.at(i)); 
    } 

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




vector<string> SIFT(Ship* problem){
  
    if(problem->check_SIFT()){
      //do sift operation instead of other operation
      
      vector<string> move_output;
      // Move container from row 1 and column 5 to row 7 and column 6 (of buffer)
      vector<vector<Container*>> buf =  intializeBuf(); 
      
      vector<Container*> buf_sorted;
      int first = 0;
      int second = 0;
      int third = 0;
      int fourth = 0;
     
      Ship *move = new Ship(problem->getGrid());
      vector<vector<Container*>> g = move->getGrid();
      for(int i = 0; i < g.size();i++){        
        for(int j = 0; j < g.at(0).size(); ++j){            
            if(g.at(i).at(j)->weight != -1 && g.at(i).at(j)->weight != -2){
              // need helper to find first avalible in bottom row of buf
              first = i+1;
              second = j+1;
              pair<int, int> location = first_buffer_loc(buf);    
              third = location.first + 1;
              fourth = location.second + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " to row " + to_string(third) + " and column " + to_string(fourth) + " of buffer ";    
              move_output.push_back(directions);     
              buf.at(location.first).at(location.second)->weight = g.at(i).at(j)->weight;              
              Container* element = new Container;              
              element->weight = g.at(i).at(j)->weight;              
              element->contents = g.at(i).at(j)->contents;              
              buf_sorted.push_back(element);
              g.at(i).at(j)->weight = -1;

            }
        }
      }
       

      
      // buf_print(buf);    
      sort_buf(buf_sorted);

      int buf_population = buf_sorted.size();

      int z = 0;
      int buf_mod = 0;
      second = 26;
      for(int i = g.size()-1; i > 0;i--){
        for(int j = ((g.at(0).size()/2) - 1); j > 0; j--){
            buf_mod = z % 2;
            if(g.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 0  && g.at(i).at(j)->weight != -2){
              g.at(i).at(j)->weight = buf_sorted.at(z)->weight;
              g.at(i).at(j)->contents = buf_sorted.at(z)->contents;
              first = 4;
              second = second - 2;
              third = i + 1;
              fourth = j + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);    
              move_output.push_back(directions); 
              z = z + 2;
            }
        }
      } 
      

      z = 1;
      buf_mod = 0;
      second = 25;
      for(int i = g.size()-1; i > 0;i--){
        for(int j = ((g.at(0).size()/2)); j < g.at(0).size(); j++){
            buf_mod = z % 2;
            if(g.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 1 && g.at(i).at(j)->weight != -2 ){
              g.at(i).at(j)->weight = buf_sorted.at(z)->weight;
              first = 4;
              second = second - 2;
              third = i + 1;
              fourth = j + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);    
              move_output.push_back(directions); 
              z = z + 2;
            }
        }
      } 
      move->setGrid(g);

      // for(int i = 0; i < move_output.size(); i++){
      //   cout << move_output.at(i) << endl;
      // }


      return move_output;
    }
}