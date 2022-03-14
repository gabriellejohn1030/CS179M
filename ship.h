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
        void print();           
        double find_mass_left();  
        double find_mass_right();  
        double balance_mass();     
        double deficit();         
        void setUniqueKey();        
        vector<pair<int,int>> pickUp();     
        vector<Ship*> dropDown(pair<int, int>);  
        string ret_larger_side();       
        vector<int> sort_larger_mass(); 
        vector<int> find_num_containers(); 
        vector<pair<int,int>> find_container_location(); 
        int find_nearest_col(); 
        void calculate_hn();
        double getCost(){return fN;}

};

#endif