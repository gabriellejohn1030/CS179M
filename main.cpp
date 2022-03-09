#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "ship.h"

using namespace std;


string ret_larger_side(Ship*);
vector<int> sort_larger_mass(Ship*);

map<double, vector<vector<int>>> duplicate;

int main(){
  
    Ship *problem = new Ship();
   
    // problem->print();
    // cout << problem->balance_mass() << endl;
    // cout << problem->deficit() << endl;
    // cout << problem->find_mass_left() << endl;
    //  cout << problem->find_mass_right() << endl;

    vector<pair<int,int>> tmp = problem->pickUp();
    problem->dropDown(tmp[0]);
    // vec1 = sort_larger_mass(problem);
    // for(int i = 0; i < vec1.size(); ++i){
    //     cout << vec1.at(i) << " ";
    // }


    return 0;
}

string ret_larger_side(Ship* p){
    int left = p->find_mass_left();
    int right = p->find_mass_right();
    string tmp = "";

    if(left > right){
        tmp = "left";
    }
    else{
        tmp = "right";
    }

    return tmp;
}

vector<int> sort_larger_mass(Ship *prob){
    string tmp = ret_larger_side(prob);
    vector<vector<int>> p = prob->grid;
    vector<int> vec1;
    if(tmp == "left"){
        for(int i = 0; i < p.size(); i++){
            for(int j = 0; j < p.at(0).size()/2; ++j){
                if(p.at(i).at(j) != 0){
                    vec1.push_back(p.at(i).at(j));
                }
            }
        }
    }
    else if(tmp == "right"){
        for(int i = 0; i < p.size(); i++){
            for(int j = p.at(0).size()/2; j < p.at(0).size(); ++j){
                if(p.at(i).at(j) != 0){
                    vec1.push_back(p.at(i).at(j));
                }
            }
        }
    }

    sort(vec1.begin(), vec1.end());
    reverse(vec1.begin(), vec1.end());

    return vec1;
}