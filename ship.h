#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <utility>

#include <QDebug>

using namespace std;

struct Container {
    Container(int w, string c){
        if(c == "NAN"){
            weight = -2;
        }else if(w < 1){
            weight = -1;
        }else{
            weight = w;
        }
        contents = c;
    }

    int weight;
    string contents;
};

class Ship{
    public:
        Ship();
        Ship(vector<vector<Container*>>);
        Ship(Ship* p);
        Ship(Ship*, int);

        Ship* move_right(Ship*, pair<int, int> &);
        Ship* move_left(Ship*, pair<int, int> &);
        Ship* getParent() { return parent; }
        int containersAbove(Ship*, pair<int, int>);

        void print();
        void setUniqueKey();                                //gives the grid a unique number
        void calculate_hn();

        double find_mass_left();                            //find total mass of left side
        double find_mass_right();                           //find total mass of right side
        double balance_mass();                              //
        double deficit();
        double getCost() { return fN; }
        double getUniqueKey() { return uniqueKey; }

        int find_nearest_col();

        bool check_SIFT();

        vector<pair<int,int>> pickUp();                     //returns indexs of every container that can be picked up in every column
        vector<Ship*> dropDown(pair<int, int>);             //returns all of the children of the picked up container
        vector<int> sort_larger_mass();
        vector<int> find_num_containers();
        vector<pair<int,int>> find_container_location();
        vector<vector<Container*>> getGrid() { return grid; }
        vector<Ship*> unloadContainer(vector<pair<int, int>> allCont, int index);
        string ret_larger_side();

    private:
        vector<vector<Container*>> grid;
        double hN;
        double fN;
        double gN;
        double uniqueKey;
        Ship* parent;
};

#endif
