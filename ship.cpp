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
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 30 31 32 33 34 35 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 40 41 42 43 44 45 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 50 51 52 53 54 55 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},      // 60 61 62 63 64 65 
        {-1,-1,-1,-1,10,6,4,-1,-1,-1,-1,-1}    // 70 71 72 73 74 75
    };
    setUniqueKey();
    fN = 1;
}

Ship::Ship(vector<vector<int>> p){
    grid = p;
    setUniqueKey();
    fN = 1;
}

void Ship::print(){
    for(int i =0; i < grid.size(); ++i ){
        for(int j = 0; j < grid.at(0).size(); j++){
            cout << grid.at(i).at(j) << '\t';
        }
        cout << endl;
    }
}

double Ship::find_mass_left(){
    double sum = 0.0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid.at(0).size()/2; ++j){
            if(grid.at(i).at(j) != -1){
                sum = sum + grid.at(i).at(j);
            }
        }
    }
    return sum;
}

double Ship::find_mass_right(){
    double sum = 0.0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = grid.at(0).size()/2; j < grid.at(0).size(); ++j){
            if(grid.at(i).at(j) != -1){
                sum = sum + grid.at(i).at(j);
            }
        }
    }
    return sum;
}

double Ship::balance_mass(){
    return (find_mass_left() + find_mass_right())/2.0;
}

double Ship::deficit(){ // Balance Mass - smaller mass side 
    double left = find_mass_left();
    double right = find_mass_right();
    double bal_mass = balance_mass();
    double def = 0.0;
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

string Ship::ret_larger_side(){
    double left = find_mass_left();
    double right = find_mass_right();
    string tmp = "";

    if(left > right){
        tmp = "left";
    }
    else{
        tmp = "right";
    }

    return tmp;
}

vector<int> Ship::sort_larger_mass(){
     
    string tmp = ret_larger_side();
    vector<vector<int>> p = grid;
    vector<int> vec1;
    // cout <<  "PSIZE: " <<p.size() << endl;
    // cout << "AT 0 SIZE: " << p.at(0).size()/2 << endl;
    if(tmp == "left"){
        for(int i = 0; i < p.size(); i++){
            for(int j = 0; j < p.at(0).size()/2; ++j){
                if(p.at(i).at(j) != -1){
                    vec1.push_back(p.at(i).at(j));
                }
            }
        }
    }
    else if(tmp == "right"){
        for(int i = 0; i < p.size(); i++){
            for(int j = p.at(0).size()/2; j < p.at(0).size(); ++j){
                if(p.at(i).at(j) != -1){
                    vec1.push_back(p.at(i).at(j));
                }
            }
        }
    }

    //cout << "HERE1" << endl;
    sort(vec1.begin(), vec1.end());
    // cout << "HERE2" << endl;
    reverse(vec1.begin(), vec1.end());
   // cout << "HERE3" << endl;

    return vec1;
}

vector<int> Ship::find_num_containers(){
    vector<int> temp = sort_larger_mass();
    string tmp = ret_larger_side();
    double def = deficit();
    double high_def = def/1.0 + (def * .1);
    double low_def = def/1.0 - (def * .1);
    vector<int> values;
    bool bal = false;
    int i = 0;

    cout << " THIS IS THE DEF: "<< def << endl;
    cout << " THIS IS THE HIGH DEF: "<< high_def << endl;
    cout << " THIS IS THE LOW DEF: "<< low_def << endl;
    for(int j = 0; j < temp.size();j++){
        cout << temp.at(j) << endl;
    }
    cout << endl;

    double left = find_mass_left();
    double right = find_mass_right();
    double balance = 0;

    cout << "LEFT: " << left << endl;
    cout << "RIGHT: " << right << endl;

    while(!(bal)){
        if((temp.at(i)/1.0 >= low_def && temp.at(i)/1.0 <= high_def) || temp.at(i)/1.0 <= def){
            low_def = low_def - temp.at(i);
            high_def = high_def - temp.at(i);
            def = def - temp.at(i);
            cout << "CALCULATIONS: " << def << endl;
            values.push_back(temp.at(i));
            if(tmp == "left"){
                left = left - temp.at(i);
            }
            if(tmp == "right"){
                right = right - temp.at(i);
            }

            if(tmp == "left"){
                right = right + temp.at(i);
            }
            if(tmp == "right"){
                left = left + temp.at(i);
            }

            balance = min(left,right)/max(left,right) ;
            if(balance >= .9){
                bal = true;
            }
        }
        if(i == temp.size()-1){
            bal = true;
        }
        i++;
    }


    values.push_back(values.size());
    return values;
    
}

vector<pair<int,int>> Ship::find_container_location(){
    vector<int> temp = sort_larger_mass();
   
    double def = deficit();
    double high_def = def/1.0 + (def * .1);
    double low_def = def/1.0 - (def * .1);
    vector<pair<int,int>> values;
    bool bal = false;
    int i = 0;

    double left = find_mass_left();
    double right = find_mass_right();
    double balance = 0;

    while(!(bal)){
        if((temp.at(i)/1.0 >= low_def && temp.at(i)/1.0 <= high_def) || temp.at(i)/1.0 <= def){
            def = def - temp.at(i);
            
            string tmp = ret_larger_side();
            vector<vector<int>> p = grid;
            if(tmp == "left"){
                for(int i = 0; i < p.size(); i++){
                    for(int j = 0; j < p.at(0).size()/2; ++j){
                        if(p.at(i).at(j) != -1){
                            values.push_back(make_pair(i,j));
                        }
                    }
                }
            }
            else if(tmp == "right"){
                for(int i = 0; i < p.size(); i++){
                    for(int j = p.at(0).size()/2; j < p.at(0).size(); ++j){
                        if(p.at(i).at(j) != -1){
                            values.push_back(make_pair(i,j));
                        }
                    }
                }
            }
            if(tmp == "left"){
                left = left - temp.at(i);
            }
            if(tmp == "right"){
                right = right - temp.at(i);
            }
             if(tmp == "left"){
                right = right + temp.at(i);
            }
            if(tmp == "right"){
                left = left + temp.at(i);
            }
            balance = min(left,right)/max(left,right);
            if(balance >= .9){
                bal = true;
            }
        }
        if(i == temp.size()-1){
            bal = true;
        }
        i++;
    }

   // values.push_back(values.size());
    return values;
    
}

int Ship::find_nearest_col(){
    // for each container, find the distance to next avalible column
    //probably retrun an int (the column number)

    int column = 0;
    string tmp = ret_larger_side();
    vector<vector<int>> p = grid;
    if(tmp == "right"){
        for(int j = (p.at(0).size()/2)-1; j > 0; j--){
            for(int i = 0; i < p.size(); ++i){
                if(p.at(i).at(j) == -1){
                    return j;
                }
            }
        }
    }
    else if(tmp == "left"){
        for(int j = p.at(0).size()/2; j < p.at(0).size(); ++j){
            for(int i = 0; i < p.size(); ++i){
                if(p.at(i).at(j) == -1){
                    return j;
                }
            }
        }
    }
    return column;
}



int Ship::calculate_hn(){
    // What is needed to calculate Hn:
    // 1) You need the number of moves that are needed to balance and the containers that need to be moved
    // 2) You need to know what the first avalible column is
    // *3) need to know what the first avalible row is in that column
    vector<int> values = find_num_containers(); // remeber the last number is the number we need to move
    vector<pair<int,int>> loc = find_container_location();
    int nearest = find_nearest_col();
    int sum = 0;

    

    for(int i = 0; i < values.size()-1; i++){
        for(int j = 0; j < loc.size(); j++){
            if(values.at(i) == grid.at(loc.at(j).first).at(loc.at(j).second)){
                sum = sum + abs(nearest - loc.at(j).second);
            }
        }
    }

    return sum;

}
