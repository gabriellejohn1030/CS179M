#include "helper.h"

void helper::emptyQueue(queue<Ship*> &q){
  while(!q.empty()){
    if(qSize < q.size()){
        qSize = q.size();
    }
    q.pop();
  }
}

queue<Ship*> helper::sortQueue(queue<Ship*> q){
  vector<Ship*> tempQ;
  Ship* temp;
  vector<double> allFn;

  if(q.empty() || q.size() < 2){return q;}

  while(!q.empty()){
    temp = q.front();
    tempQ.push_back(temp);
    allFn.push_back(temp->getCost());
    q.pop();
  }
  for(int i = 0; i < allFn.size(); ++i){
    for(int j = i+1; j < allFn.size(); ++j){
      if(allFn.at(i) > allFn.at(j)){
        swap(allFn.at(i), allFn.at(j));
        swap(tempQ.at(i), tempQ.at(j));
      }
    }
  }

  for(int i = 0; i < tempQ.size(); ++i){
      q.push(tempQ.at(i));
  }

  return q;
}

bool helper::isGoalState(Ship* goal){
    // goal->print();
    double left = goal->find_mass_left();
    double right = goal->find_mass_right();
    double top = min(left, right);
    double bottom = max(left, right);
    double result = top/bottom;

    // cout << "GOAL: " << (result >= 0.9) << endl;

    return (result >= 0.9);
}

vector<string> helper::outputGoalSteps(Ship *goal){
    vector<Ship*> steps;
    vector<string> moves;
    Ship* tmp = goal;
    Ship* parent;
    Ship* child;

    while(tmp != NULL){
        steps.push_back(tmp);
        tmp = tmp->getParent();
    }
    reverse(steps.begin(), steps.end());

    for(int i = 0; i < steps.size() - 1; ++i){
        parent = steps.at(i);
        child = steps.at(i + 1);
//        steps.at(i)->print();
        moves.push_back(steps.at(i)->outputMoves(parent, child));
    }
//    steps.at(steps.size()-1)->print();
    return moves;
}

Ship* helper::loadManifest(string fileName){
    fstream file;

    file.open(fileName);
    if(!file.is_open()){
        qDebug() << "FAILED LOADING MANIFEST";
        return nullptr;
    }
    vector<vector<Container*>> grid = initializeVec();
    string idx, weight, content;
    int w = 0, row = 0, column = 0, containers = 0;

    while(file >> idx){
        file >> weight;
        file >> content;

        w = stoi(weight.substr(1, 5));
        if (content != "UNUSED" && content != "NAN") containers++;
        row = stoi(idx.substr(1, 2)) - 1;
        column = stoi(idx.substr(4, 2)) - 1;

        grid[row][column] = new Container(w, content);
    }
    reverse(grid.begin(), grid.end());
    Ship* tmp = new Ship(grid);
    if (containers > 0) {
        emit containersFound(containers);
    }
    // tmp->print();
    return tmp;
}

vector<vector<Container*>> helper::initializeVec(){
    vector<vector<Container*>> tmp;
    tmp = {                                   // ij (row, column)
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 00 01 02 03 04 05
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},      // 10 11 12 13 14 15
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 20 21 22 23 24 25
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 30 31 32 33 34 35
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 40 41 42 43 44 45
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 50 51 52 53 54 55
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},     // 60 61 62 63 64 65
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")}      // 70 71 72 73 74 75
    };
    return tmp;
}

void helper::balance(Ship *problem) {
    queue<Ship*> q;
    q.push(problem);
    // if we are not using sift, first if, else we are doing sift
    if (!problem->check_SIFT()) {
        while(!q.empty()){
          qSize = max(qSize, q.size());
          Ship* node = q.front();

          duplicate.insert(pair<double, vector<vector<Container*>>>(node->getUniqueKey(), node->getGrid())); //inserts node into the map

          if (isGoalState(node)){
            qDebug() << "GOAL STATE FOUND";
            moves = outputGoalSteps(node);
            emit balanceFinished(true);
            return;
          }

          Ship *goal = balanceAlgorithm(q);

          if(goal != NULL){
              qDebug() << "GOAL STATE FOUND";
              moves = outputGoalSteps(goal);
              emit balanceFinished(true);
              return;
          }
        }
    } else {
        moves = SIFT(problem);
        emit balanceFinished(true);
        return;
    }
    emit balanceFinished(false);
    return;
}

bool helper::unloadAndLoadAlgorithm(vector<pair<int,int>> idxs, Ship* p, vector<Container*> c){
    vector<vector<Container*>> buffer = intializeBuf();
    Ship *temp = p;
    vector<Ship*> steps;
    temp->trickleDown();
    steps.push_back(new Ship(temp));
    for(int i = 0; i < idxs.size(); ++i){
        vector<pair<Ship*, Container*>> tmp = temp->unloadContainer(idxs, i);
        for(int j = 0; j < tmp.size(); ++j){
            if(tmp.at(j).first == NULL && tmp.at(j).second == NULL){continue;}
            else if(tmp.at(j).first == NULL && tmp.at(j).second != NULL){continue;}
            else{
                temp = new Ship(tmp.at(j).first);
                temp->trickleDown();
                steps.push_back(new Ship(temp));
            }
        }

        for(int k = 0; k < tmp.size(); ++k){
            if(tmp.at(k).first == NULL && tmp.at(k).second == NULL && k == tmp.size()-1){ //container to load
                vector<vector<Container*>> g = temp->getGrid();
                temp->trickleDown();
                steps.push_back(new Ship(temp));
                temp->removeContainer(g[idxs[i].first][idxs[i].second]);
                steps.push_back(new Ship(temp));
            }
            else if(tmp.at(k).first == NULL && tmp.at(k).second != NULL){ //container to buffer
                moveToBuffer(buffer, tmp.at(k).second);
                temp->trickleDown();
                temp->removeContainer(tmp.at(k).second);
            }
        }
        idxs.erase(idxs.begin()+i);
        --i;
    }
    pair<int, int> emptyBuffer = bufferEmpty(buffer);
    while(emptyBuffer.first != -1){
        temp->addContainer(buffer[emptyBuffer.first][emptyBuffer.second], -1);
        temp->trickleDown();
        steps.push_back(new Ship(temp));
        removeFromBuffer(buffer, emptyBuffer);
        emptyBuffer = bufferEmpty(buffer);
    }
    for(int i = 0; i < steps.size()-1; ++i){
        if(compare(steps.at(i),steps.at(i+1))){
            steps.erase(steps.begin()+i);
            --i;
        }
    }

    for(int i = 0; i < c.size(); ++i){
        Container *loadingContainer = new Container(c.at(i)->weight, c.at(i)->contents);
        int n = availableColumn(idxs);
        temp->addContainer(loadingContainer, n);
        temp->trickleDown();
        steps.push_back(new Ship(temp));
    }

    for(int i = steps.size()-1; i > 0; --i){
        steps[i]->setParent(steps[i-1]);
    }
    steps.at(0)->setParent(NULL);
    moves = outputGoalSteps(steps.at(steps.size()-1));
    emit loadAndUnloadFinished(true);
    return true;
}

vector<vector<Container*>> helper::intializeBuf(){
    vector<vector<Container*>> buf = {                                   // ij (row, column)
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")},
        {new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"), new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED"),new Container(0, "UNUSED")}
    };
    return buf;
}

Ship* helper::balanceAlgorithm(queue<Ship*> &q){
  Ship* parent = q.front();
  q.pop();

  vector<pair<int,int>> pick_up_indexes = parent->pickUp();
  vector<Ship*> children;
  for(int i = 0; i < pick_up_indexes.size(); ++i){
    vector<Ship*> drop_tmp = parent->dropDown(pick_up_indexes.at(i));
    for(int j = 0; j < drop_tmp.size(); ++j){
      children.push_back(drop_tmp.at(j));
    }
  }

  for(int i = 0; i < children.size(); i++){
    if(duplicate.find(children.at(i)->getUniqueKey()) != duplicate.end()){ //checks if child has already been explored
      children.erase(children.begin() + i);
      --i;
      continue;
    }

    if(isGoalState(children.at(i))){
      emptyQueue(q);
      cout << "CALLED GOAL STATE IN SEARCH" << endl;
      //dont we want to return the goal state somewhere here?
      return children.at(i);
    }

    if(qSize < q.size()){qSize = q.size();}

  }
  for(int i = 0; i < children.size(); i++){
    q.push(children.at(i));
  }
  q = sortQueue(q);

  return NULL;
}

void helper::moveToBuffer(vector<vector<Container*>> &buf, Container *c){
    for(int i = 0; i < buf[0].size(); ++i){
        for(int j = buf.size()-1; j > -1; --j){
            if(buf[j][i]->weight == -1){
                buf[j][i] = c;
                return;
            }
        }
    }
}

void helper::outputBuffer(vector<vector<Container*>> b){
    for(int i = 0; i < b.size(); ++i ){
        for(int j = 0; j < b.at(0).size(); j++){
            cout << b.at(i).at(j)->weight << '\t';
        }
        cout << endl;
    }
    cout << endl;
}


pair<int,int> helper::bufferEmpty(vector<vector<Container*>> buf){
    pair<int, int> idx = make_pair(-1,-1);
    for(int i = 0; i < buf[0].size(); ++i){
        for(int j = buf.size()-1; j > -1; --j){
            if(buf[j][i]->weight != -1){
                idx.first = j;
                idx.second = i;
                return idx;
            }
        }
    }
    return idx;
}


void helper::removeFromBuffer(vector<vector<Container*>> &buf, pair<int,int> idx){
    buf[idx.first][idx.second] = new Container(0, "UNUSED");
}

int helper::availableColumn(vector<pair<int,int>> idxs){
    int n = -1;
    bool exists = false;
    for(int i = 0; i < 12; ++i){
        exists = false;
        for(int j = 0; j < idxs.size(); ++j){
            if(idxs.at(j).second == i){
                exists = true;
            }
        }
        if(!exists){
            return i;
        }
    }
    return n;
}

bool helper::compare(Ship* a, Ship* b){
    vector<vector<Container*>> gridA = a->getGrid();
    vector<vector<Container*>> gridB = b->getGrid();

    for(int i = 0; i < gridA.size(); ++i){
        for(int j = 0; j < gridA[0].size(); ++j){
            if(gridA[i][j]->weight != gridB[i][j]->weight){
                return false;
            }
        }
    }
    return true;
}

void helper::buf_print(vector<vector<Container*>> buf){
    for(int i =0; i < buf.size(); ++i ){
        for(int j = 0; j < buf.at(0).size(); j++){
            cout << buf.at(i).at(j)->weight << "   ";
        }
        cout << endl;
    }
    cout << endl;

}

pair<int, int> helper::first_buffer_loc(vector<vector<Container*>> buf){
  for(int i = buf.size()-1; i > 0; i--){
    for(int j = buf.at(i).size()-1; j > 0; j--){
      if(buf.at(i).at(j)->weight == -1){
        return make_pair(i,j);
      }
    }
  }

  return make_pair(0,0);
}


vector<Container*> helper::sort_buf(vector<Container*> buf){
    int min_idx;
    for (int i = 0; i < buf.size()-1; i++){
        // Find the minimum element in unsorted array
        min_idx = i;
        for (int j = i+1; j < buf.size(); j++){
          if (buf.at(j)->weight < buf.at(i)->weight){
              min_idx = j;
          }
        }
        // Swap the found minimum element with the first element
        swap(buf.at(min_idx), buf.at(i));
    }

    return buf;
}

vector<string> helper::SIFT(Ship* problem){

    if(problem->check_SIFT()){
      //do sift operation instead of other operation

      vector<string> move_output;
      // Move container from row 1 and column 5 to row 7 and column 6 (of buffer)
      vector<vector<Container*>> buf =  intializeBuf();

      vector<Container*> buf_sorted;
      int first = 0;
      int second = 0;
      int third = 0;
      int fourth = 0;

      Ship *move = new Ship(problem->getGrid());
      vector<vector<Container*>> g = move->getGrid();
      for(int i = 0; i < g.size();i++){
        for(int j = 0; j < g.at(0).size(); ++j){
            if(g.at(i).at(j)->weight != -1 && g.at(i).at(j)->weight != -2){
              // need helper to find first avalible in bottom row of buf
              first = i+1;
              second = j+1;
              pair<int, int> location = first_buffer_loc(buf);
              third = location.first + 1;
              fourth = location.second + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " to row " + to_string(third) + " and column " + to_string(fourth) + " of buffer ";
              move_output.push_back(directions);
              buf.at(location.first).at(location.second)->weight = g.at(i).at(j)->weight;
              Container* element = new Container(g.at(i).at(j)->weight, g.at(i).at(j)->contents);
              buf_sorted.push_back(element);
              g.at(i).at(j)->weight = -1;

            }
        }
      }



      // buf_print(buf);
      sort_buf(buf_sorted);

      int buf_population = buf_sorted.size();

      int z = 0;
      int buf_mod = 0;
      second = 26;
      for(int i = g.size()-1; i > 0;i--){
        for(int j = ((g.at(0).size()/2) - 1); j > 0; j--){
            buf_mod = z % 2;
            if(g.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 0  && g.at(i).at(j)->weight != -2){
              g.at(i).at(j)->weight = buf_sorted.at(z)->weight;
              g.at(i).at(j)->contents = buf_sorted.at(z)->contents;
              first = 4;
              second = second - 2;
              third = i + 1;
              fourth = j + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);
              move_output.push_back(directions);
              z = z + 2;
            }
        }
      }


      z = 1;
      buf_mod = 0;
      second = 25;
      for(int i = g.size()-1; i > 0;i--){
        for(int j = ((g.at(0).size()/2)); j < g.at(0).size(); j++){
            buf_mod = z % 2;
            if(g.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 1 && g.at(i).at(j)->weight != -2 ){
              g.at(i).at(j)->weight = buf_sorted.at(z)->weight;
              first = 4;
              second = second - 2;
              third = i + 1;
              fourth = j + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);
              move_output.push_back(directions);
              z = z + 2;
            }
        }
      }
      move->setGrid(g);

      // for(int i = 0; i < move_output.size(); i++){
      //   cout << move_output.at(i) << endl;
      // }


      return move_output;
    }
}




int helper::estimated_time_SIFT(Ship* problem){

    if(problem->check_SIFT()){
      //do sift operation instead of other operation

      vector<string> move_output;
      int time = 0;
      // Move container from row 1 and column 5 to row 7 and column 6 (of buffer)
      vector<vector<Container*>> buf =  intializeBuf();

      vector<Container*> buf_sorted;
      int first = 0;
      int second = 0;
      int third = 0;
      int fourth = 0;

      Ship *move = new Ship(problem->getGrid());
      vector<vector<Container*>> g = move->getGrid();
      for(int i = 0; i < g.size();i++){
        for(int j = 0; j < g.at(0).size(); ++j){
            if(g.at(i).at(j)->weight != -1 && g.at(i).at(j)->weight != -2){
              // need helper to find first avalible in bottom row of buf
              first = i+1;
              second = j+1;
              pair<int, int> location = first_buffer_loc(buf);
              third = location.first + 1;
              fourth = location.second + 1;
              time = time + i + j + (2 * 15) + (2 * third) + (2 * (24 - fourth));
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " to row " + to_string(third) + " and column " + to_string(fourth) + " of buffer ";
              move_output.push_back(directions);
              buf.at(location.first).at(location.second)->weight = g.at(i).at(j)->weight;
              Container* element = new Container(g.at(i).at(j)->weight, g.at(i).at(j)->contents);
              buf_sorted.push_back(element);
              g.at(i).at(j)->weight = -1;

            }
        }
      }

      sort_buf(buf_sorted);

      int buf_population = buf_sorted.size();

      int z = 0;
      int buf_mod = 0;
      second = 26;
      for(int i = g.size()-1; i > 0;i--){
        for(int j = ((g.at(0).size()/2) - 1); j > 0; j--){
            buf_mod = z % 2;
            if(g.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 0  && g.at(i).at(j)->weight != -2){
              g.at(i).at(j)->weight = buf_sorted.at(z)->weight;
              g.at(i).at(j)->contents = buf_sorted.at(z)->contents;
              time = time + (i+1) + (j + 1);
              first = 4;
              second = second - 2;
              third = i + 1;
              fourth = j + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);
              move_output.push_back(directions);
              z = z + 2;
            }
        }
      }


      z = 1;
      buf_mod = 0;
      second = 25;
      for(int i = g.size()-1; i > 0;i--){
        for(int j = ((g.at(0).size()/2)); j < g.at(0).size(); j++){
            buf_mod = z % 2;
            if(g.at(i).at(j)->weight == -1 && buf_sorted.size() > z && buf_mod == 1 && g.at(i).at(j)->weight != -2 ){
              g.at(i).at(j)->weight = buf_sorted.at(z)->weight;
              time = time + (i+1) + (j + 1);
              first = 4;
              second = second - 2;
              third = i + 1;
              fourth = j + 1;
              string directions = "Move container from row " + to_string(first) + " and column " + to_string(second) + " of the buffer " + " to row " + to_string(third) + " and column " + to_string(fourth);
              move_output.push_back(directions);
              z = z + 2;
            }
        }
      }
      move->setGrid(g);


      return time;
    }
}


vector<double> helper::estimatedTime(Ship* p, vector<Container*> load){
    vector<double> times;
    Ship *child = p;
    Ship *parent = child->getParent();
    bool movedToTruck = true;

    while(parent != NULL){
        double t = 0.0;
        vector<vector<Container*>> cg = child->getGrid(), pg = parent->getGrid();
        pair<int, int> cIdx = make_pair(-1,-1), pIdx = make_pair(-1,-1);

        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 12; ++j){
                if(cg[i][j]->weight != pg[i][j]->weight && cg[i][j]->weight > -1){
                    cIdx.first = i;
                    cIdx.second = j;
                } else if(cg[i][j]->weight != pg[i][j]->weight && pg[i][j]->weight > -1){
                    pIdx.first = i;
                    pIdx.second = j;
                }
            }
        }
        if(cIdx.first == -1 || pIdx.first == -1){ //container in buffer or truck
            t = pIdx.first + 1;
            if(movedToTruck){
                t += 2;
                movedToTruck = false;
            } else{
                t += 15;
            }
        } else{
            t = max(cIdx.first, pIdx.first) - min(cIdx.first, pIdx.first) + 1;
            t += max(cIdx.second, pIdx.second) - min(cIdx.second, pIdx.second) + 1;
            int cnt = 0;
            int low, high, height;
            if(min(cIdx.second, pIdx.second) == pIdx.second){
                low = pIdx.second;
                high = cIdx.second;
                height = pIdx.first;
            } else{
                low = cIdx.second;
                high = pIdx.second;
                height = cIdx.first;
            }
            if(height < 0){height = 1;}
            for(int i = low; i <= high; ++i){
                if(pg[height][i]->weight < 0){
                    ++cnt;
                    continue;
                }
                int raised = 0;
                while(height > -1 && pg[height][i]->weight > -1){
                    ++raised;
                    --height;
                    if(height < 0){
                        ++raised;
                        break;
                    }
                }
                if(height < 0){
                    height = 0;
                }
                raised = raised*2;
                cnt += raised;
            }
            t += cnt;
        }
        t += load.size() * 2;

        cIdx = pIdx = make_pair(-1,-1);
        times.push_back(t);
        child = parent;
        parent = child->getParent();
    }
    return times;
}
