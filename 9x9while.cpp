#include <iostream>
#include<string>
#include<cmath>
using namespace std;
int main() {
    int a=1;
    while(a<=9){
        int b=1;
        while(b<=a){
            if(a*b<10){
                cout<<b<<"x"<<a<<"="<<a*b<<"  ";
            }
            else
            cout<<b<<"x"<<a<<"="<<b*a<<" ";
            b++;

        }
        a++;
        cout <<endl;
    }
    return 0;
}
