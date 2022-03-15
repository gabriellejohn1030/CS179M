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
        //Ship* parent;

        Ship();
        Ship(vector<vector<Container*>>);
        Ship(Ship* p);
        Ship(Ship*, int);
        void print();           
        double find_mass_left();                            //find total mass of left side
        double find_mass_right();                           //find total mass of right side
        double balance_mass();                              //
        double deficit();                                   //
        void setUniqueKey();                                //gives the grid a unique number
        vector<pair<int,int>> pickUp();                     //returns indexs of every container that can be picked up in every column
        vector<Ship*> dropDown(pair<int, int>);             //returns all of the children of the picked up container
        string ret_larger_side();       
        vector<int> sort_larger_mass(); 
        vector<int> find_num_containers(); 
        vector<pair<int,int>> find_container_location(); 
        int find_nearest_col(); 
        void calculate_hn();
        double getCost(){return fN;}

};

#endif