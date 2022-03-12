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

    cout << isGoalState(problem) << ' ' << problem->getCost() << endl;



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