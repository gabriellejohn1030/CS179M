#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <queue>
#include "ship.h"

using namespace std;

bool isGoalState(Ship*);

map<double, vector<vector<int>>> duplicate;

int main(){
    
    Ship *problem = new Ship();
    queue<Ship*> myShip;

  //  vector<pair<int,int>> tmp = problem->pickUp();
   
    //cout << problem->ret_larger_side() << endl;
    //cout << problem->find_mass_left() << endl;
    //cout << problem->find_mass_right() << endl;
    //vector<int> temp = problem->sort_larger_mass();
    // cout << "NUM CONTAINERS: " << endl;
    // vector<int> temp = problem->find_num_containers();
    // for(int i = 0; i < temp.size();i++){
    //     cout << "Values: " << temp.at(i) << " ";
    // }
    // cout << endl;
    // cout << "Nearest Col: " << problem->find_nearest_col() << endl;
    cout << "HN: " << endl << problem->calculate_hn() << endl;
  //  cout << isGoalState(problem) << ' ' << problem->getCost() << endl;



    return 0;
}

bool isGoalState(Ship* goal){
    double left = goal->find_mass_left();
    double right = goal->find_mass_right();
    double top = min(left, right);
    double bottom = max(left, right);
    double result = top/bottom;

    return (result >= 0.9); 
}