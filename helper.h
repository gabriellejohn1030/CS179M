#ifndef HELPER_H
#define HELPER_H
#include <queue>
#include <fstream>

#include <QObject>

#include "ship.h"

class helper : public QObject {
    Q_OBJECT
    public:
        helper() = default;
        bool isGoalState(Ship*);
        void outputGoalSteps(Ship*);
        Ship* loadManifest(string fileName);
        void emptyQueue(queue<Ship*> &);
        queue<Ship*> sortQueue(queue<Ship*>);
        Ship* searchAlgorithm(queue<Ship*> &);
        vector<vector<Container*>> initializeVec();
        void balance(Ship *problem);
        Ship* unloadAlgorithm(vector<pair<int,int>> &idxs, Ship* p);

    signals:
        void balanceFinished(bool success);

    private:
        map<double, vector<vector<Container*>>> duplicate;
        unsigned long qSize;
};

#endif // HELPER_H
