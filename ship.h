#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <map>
#include <utility>
#include <cstdlib>
#include "container.h"
using namespace std;

class Ship{
    private:
        vector<vector<Container*>> grid;
        double hN;
        double fN;
        double gN;
        double uniqueKey;
        Ship* parent;

    public:
        Ship();
        Ship(vector<vector<Container*>>);
        Ship(Ship* p);
        Ship(Ship*, int);
        void print();           
        double find_mass_left();                                //find total mass of left side
        double find_mass_right();                               //find total mass of right side
        double balance_mass();                                  //
        double deficit();                                       //
        void setUniqueKey();                                    //gives the grid a unique number
        vector<pair<int,int>> pickUp();                         //returns indexs of every container that can be picked up in every column
        vector<Ship*> dropDown(pair<int, int>);                 //returns all of the children of the picked up container
        vector<pair<Ship*,Container*>> unloadContainer(vector<pair<int, int>> allCont, int index);  //returns first availible spot on left or puts in buffer
        string ret_larger_side();       
        vector<int> sort_larger_mass(); 
        vector<int> find_num_containers(); 
        vector<pair<int,int>> find_container_location(); 
        int find_nearest_col(); 
        void calculate_hn();
        double getCost(){return fN;}
        bool check_SIFT();
        void trickleDown();
        void removeContainer(Container*);
        void addContainer(Container*, int);
        vector<vector<Container*>> getGrid();
        void setGrid(vector<vector<Container*>>);
        double getUniqueKey(); 
        Ship* move_right(Ship*, pair<int, int> &);
        Ship* move_left(Ship*, pair<int, int> &);
        int containersAbove(Ship*, pair<int, int>);
        Ship* getParent(){return parent;}
        Ship* setParent(Ship* p){parent = p;}
};

#endif