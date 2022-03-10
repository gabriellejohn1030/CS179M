#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <map>
#include <utility>
using namespace std;

class Ship{
    public:
        vector<vector<int>> grid;
        double hN;
        double fN;
        double gN;
        double uniqueKey;

        Ship();
        Ship(vector<vector<int>>);
        void print();
        int find_mass_left();   //added change here
        int find_mass_right();  //added change here
        int balance_mass();
        int deficit();
        void setUniqueKey();
        vector<pair<int,int>> pickUp();
        vector<Ship*> dropDown(pair<int, int>);
        string ret_larger_side(); // added change here
        vector<int> sort_larger_mass(); // added change here
        vector<int> find_num_containers(); // added change here
        vector<pair<int,int>> find_container_location(); // added change here
        int find_nearest_col(); // added change here
        int calculate_hn(); // added change here
};

#endif