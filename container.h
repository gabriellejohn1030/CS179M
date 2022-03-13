#ifndef CONTAINER_H
#define CONTAINER_H
#include <string>
using namespace std;

class Container{
    public:
        int weight;
        string contents;

        Container(){}
        Container(int w, string c){  
            if(w != 0){
                weight = w;
            }else{
                weight = -1;
            }
            contents = c;
        }
};
#endif