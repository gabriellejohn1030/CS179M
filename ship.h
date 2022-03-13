#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <map>
#include <utility>
#include "container.h"
using namespace std;

class Ship{
    public:
        vector<vector<Container*>> grid;
        double hN;
        double fN;
        double gN;
        double uniqueKey;

        Ship();
        Ship(vector<vector<Container*>>);
        Ship(Ship* p);
        void print();           // changed here
        double find_mass_left();   // changed here
        double find_mass_right();  // changed here
        double balance_mass();      // changed here
        double deficit();           // changed here
        void setUniqueKey();        // changed here
        vector<pair<int,int>> pickUp();     //changed here
        vector<Ship*> dropDown(pair<int, int>);  // changed here
        string ret_larger_side();       // good
        vector<int> sort_larger_mass(); // good
        vector<int> find_num_containers(); // good
        vector<pair<int,int>> find_container_location(); // good
        int find_nearest_col(); // added change here
        void calculate_hn(); // added change here
        double getCost(){return fN;}
};

#endif