#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void print(vector<vector<int>> p){
    for(int i =0; i < p.size(); ++i ){
        for(int j = 0; j < p.at(0).size(); j++){
            cout << p.at(i).at(j) << "    ";
        }
        cout << endl;
    }
}

int find_mass_left(vector<vector<int>> p){
    int sum = 0;
    for(int i = 0; i < p.size(); i++){
        for(int j = 0; j < p.at(0).size()/2; ++j){
            if(p.at(i).at(j) != 0){
                sum = sum + p.at(i).at(j);
            }
        }
    }

    return sum;
}

int find_mass_right(vector<vector<int>> p){
    int sum = 0;
    for(int i = 0; i < p.size(); i++){
        for(int j = p.at(0).size()/2; j < p.at(0).size(); ++j){
            if(p.at(i).at(j) != 0){
                sum = sum + p.at(i).at(j);
            }
        }
    }

    return sum;
}

int balance_mass(vector<vector<int>> p){
    return (find_mass_left(p) + find_mass_right(p))/2;
}

int deficit(vector<vector<int>> p){ // Balance Mass - smaller mass side 
    int left = find_mass_left(p);
    int right = find_mass_right(p);
    int bal_mass = balance_mass(p);
    int def = 0;
    if(left > right){
        def = bal_mass - right;
    }
    else{
        def = bal_mass - left;
    }

    return def;
}

string ret_larger_side(vector<vector<int>> p){
    int left = find_mass_left(p);
    int right = find_mass_right(p);
    string tmp = "";

    if(left > right){
        tmp = "left";
    }
    else{
        tmp = "right";
    }

    return tmp;
}

vector<int> sort_larger_mass(vector<vector<int>> p){
    string tmp = ret_larger_side(p);
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

int main(){
  
    vector<vector<int>> problem = 
    {                                   // ij (row, column)
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 00 01 02 03 04 05 
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 10 11 12 13 14 15 
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 20 21 22 23 24 25 
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 30 31 32 33 34 35 
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 40 41 42 43 44 45 
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 50 51 52 53 54 55 
        {0,0,0,0,0,0,0,0,0,0,0,0},      // 60 61 62 63 64 65 
        {0,100,99,0,0,0,0,0,0,0,0,0}    // 70 71 72 73 74 75 

        
    };

    vector<int> vec1;
   
    // print(problem);
    // cout << balance_mass(problem) << endl;
    // cout << deficit(problem) << endl;
    // cout << find_mass_left(problem) << endl;
    // cout << find_mass_right(problem) << endl;

    // vec1 = sort_larger_mass(problem);
    // for(int i = 0; i < vec1.size(); ++i){
    //     cout << vec1.at(i) << " ";
    // }


    return 0;
}