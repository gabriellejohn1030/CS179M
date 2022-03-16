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
            if(c == "NAN"){
                weight = -2;
            }else if(w < 1){
                weight = -1;
            }else{
                weight = w;
            }
            contents = c;
        }
};
#endif