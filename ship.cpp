#include "ship.h"
using namespace std;

Ship* move_right(Ship*, pair<int, int> &);
Ship* move_left(Ship*, pair<int, int> &);


Ship::Ship(){
    grid = 
    {                                   // ij (row, column)
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 00 01 02 03 04 05 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 10 11 12 13 14 15 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 20 21 22 23 24 25 
        {-1,-1,4,-1,-1,-1,-1,-1,-1,-1,20,-1},      // 30 31 32 33 34 35 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3},      // 40 41 42 43 44 45 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,6,-1,-1},      // 50 51 52 53 54 55 
        {-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 60 61 62 63 64 65 
        {10,100,99,-1,-1,-1,-1,-1,-1,330,-1,-1}    // 70 71 72 73 74 75
    };
    setUniqueKey();
}

Ship::Ship(vector<vector<int>> p){
    grid = p;
    setUniqueKey();
}

void Ship::print(){
    for(int i =0; i < grid.size(); ++i ){
        for(int j = 0; j < grid.at(0).size(); j++){
            cout << grid.at(i).at(j) << '\t';
        }
        cout << endl;
    }
}

int Ship::find_mass_left(){
    int sum = 0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid.at(0).size()/2; ++j){
            if(grid.at(i).at(j) != 0){
                sum = sum + grid.at(i).at(j);
            }
        }
    }

    return sum;
}

int Ship::find_mass_right(){
    int sum = 0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = grid.at(0).size()/2; j < grid.at(0).size(); ++j){
            if(grid.at(i).at(j) != 0){
                sum = sum + grid.at(i).at(j);
            }
        }
    }

    return sum;
}

int Ship::balance_mass(){
    return (find_mass_left() + find_mass_right())/2;
}

int Ship::deficit(){ // Balance Mass - smaller mass side 
    int left = find_mass_left();
    int right = find_mass_right();
    int bal_mass = balance_mass();
    int def = 0;
    if(left > right){
        def = bal_mass - right;
    }
    else{
        def = bal_mass - left;
    }

    return def;
}

void Ship::setUniqueKey(){
    double lock = 1;
    double key = 0;

    for(int i = 0; i < grid.size(); ++i){
        vector<int> row = grid.at(i);
        for(int j = 0; j < row.size(); ++j){
            key += abs(sqrt(row.at(j)) * lock);
            lock *= sqrt(key/100.0);
        }
    }
    uniqueKey = key;
}

vector<pair<int,int>> Ship::pickUp(){
    vector<int> filled(grid[0].size(), -1);
    vector<pair<int, int>> pickupIdxs(grid[0].size(), make_pair(-1, -1));

    for(int i = 0; i < grid.size(); ++i){
        vector<int> row = grid[i];
        for(int j = 0; j < row.size(); ++j){
            if(grid[i][j] != -1 && filled[j] == -1){
                pickupIdxs[j] = make_pair(i, j);
                filled[j] = 1;
            }
        }
    }
    for(int i = 0; i < pickupIdxs.size(); ++i){
        pickupIdxs.erase(remove(pickupIdxs.begin(), pickupIdxs.end(), make_pair(-1,-1)), pickupIdxs.end());
        // cout << pickupIdxs[i].first << ' ' << pickupIdxs[i].second << ' ' << grid[pickupIdxs[i].first][pickupIdxs[i].second] << endl;
    }
    return pickupIdxs;
}

vector<Ship*> Ship::dropDown(pair<int, int> idx){
    vector<Ship *> children;
    Ship *move = new Ship(grid);
    pair<int, int> left = idx;
    pair<int, int> right = idx;
    int size = 12;
    int i = 0;
    
    while(size > 0){
        move = move_right(move, right);
        if(move == NULL){
            size--;
            continue;
        }
        children.push_back(move);
        size--;
    }
    size = 12;
    move = new Ship(grid);
    while(size > 0){
        move = move_left(move, left);
        if(move == NULL){
            size--;
            continue;
        }
        children.push_back(move);
        size--;
    }
    return children;
}

Ship* move_right(Ship *p, pair<int, int> &idx){
    if(idx.second + 1 >= 12){
        return NULL;
    }
    vector<vector<int>> g = p->grid;
    int row = idx.first;
    int column = idx.second + 1;

    for(int i = 0; i < g.size(); ++i){
        if(g[row][column] == -1){
            if(row + 1 >= g.size()){
                swap(p->grid[row][column], p->grid[idx.first][idx.second]);
                idx.first = row;
                idx.second = column;
                return p;
            }
            else if(g[row + 1][column] != -1){ //check to make sure about row+1 in OR, otherwise g[row+1] is out of bounds
                swap(p->grid[row][column], p->grid[idx.first][idx.second]);
                idx.first = row;
                idx.second = column;
                return p;
            }
            row++;
            while(row < p->grid.size()){ //moves crate to the very bottom or right before first occupied space in the column
                if(g[row][column] != -1){
                    swap(p->grid[row-1][column], p->grid[idx.first][idx.second]);
                    idx.first = row-1;
                    idx.second = column;
                    return p;
                }
                row++;
            }
        }
        row--;
        while(row > -1){ //edge case is if column is filled to the top
                if(g[row][column] == -1){
                    swap(p->grid[row][column], p->grid[idx.first][idx.second]);
                    idx.first = row;
                    idx.second = column;
                    return p;
                }
                row--;
        }
    }
    return p;
}

Ship* move_left(Ship *p, pair<int, int> &idx){ 
    if(idx.second - 1 < 0){
        return NULL;
    }
    vector<vector<int>> g = p->grid;
    int row = idx.first;
    int column = idx.second - 1;

    for(int i = 0; i < idx.second; ++i){
        if(g[row][column] == -1){
            if(row + 1 >= g.size()){
                swap(p->grid[row][column], p->grid[idx.first][idx.second]);
                idx.first = row;
                idx.second = column;
                return p;
            }
            else if(g[row + 1][column] != -1){ //check to make sure about row+1 in OR, otherwise g[row+1] is out of bounds
                swap(p->grid[row][column], p->grid[idx.first][idx.second]);
                idx.first = row;
                idx.second = column;
                return p;
            }
            row++;
            while(row < p->grid.size()){ //moves crate to the very bottom or right before first occupied space in the column
                if(g[row][column] != -1){
                    swap(p->grid[row-1][column], p->grid[idx.first][idx.second]);
                    idx.first = row-1;
                    idx.second = column;
                    return p;
                }
                row++;
            }
        }
        row--;
        while(row > -1){ //edge case is if column is filled to the top
                if(g[row][column] == -1){
                    swap(p->grid[row][column], p->grid[idx.first][idx.second]);
                    idx.first = row;
                    idx.second = column;
                    return p;
                }
                row--;
        }
    }
    return p;
}