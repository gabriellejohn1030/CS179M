#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "ship.h"

using namespace std;


// string ret_larger_side(Ship*);
// vector<int> sort_larger_mass(Ship*);

map<double, vector<vector<int>>> duplicate;

int main(){
  
    Ship *problem = new Ship();
   
    // problem->print();
    // cout << problem->balance_mass() << endl;
    // cout << problem->deficit() << endl;
    // cout << problem->find_mass_left() << endl;
    //  cout << problem->find_mass_right() << endl;

  //  vector<pair<int,int>> tmp = problem->pickUp();
   
    //cout << problem->ret_larger_side() << endl;
    //cout << problem->find_mass_left() << endl;
    //cout << problem->find_mass_right() << endl;
    //vector<int> temp = problem->sort_larger_mass();
    cout << "NUM CONTAINERS: " << endl;
    vector<int> temp = problem->find_num_containers();
    for(int i = 0; i < temp.size();i++){
        cout << temp.at(i) << " ";
    }
    cout << endl;
    cout << "Nearest Col: " << problem->find_nearest_col() << endl;
    cout << "HN: "<< problem->calculate_hn() << endl;

   // problem->dropDown(tmp[0]);
    // vec1 = sort_larger_mass(problem);
    // for(int i = 0; i < vec1.size(); ++i){
    //     cout << vec1.at(i) << " ";
    // }


    return 0;
}
