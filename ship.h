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
        int find_mass_left();
        int find_mass_right();
        int balance_mass();
        int deficit();
        void setUniqueKey();
        vector<pair<int,int>> pickUp();
        vector<Ship*> dropDown(pair<int, int>);
};

#endif