#include<iostream>
#include<string>
#include<cmath>
using namespace std;
int main(){
    for(int y=1;y<=9;y++){
        for(int a=1;a<=y;a++){
            if(a*y<10){
            cout<<a<<"x"<<y<<"="<<a*y<<"  ";  
            }
            else
            cout<<a<<"x"<<y<<"="<<a*y<<" ";
            

        }
        cout<<endl;
    }
    return 0;
}
