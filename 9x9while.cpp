#include <iostream>
#include<string>
#include<cmath>
using namespace std;
int main() {
    int a=1;
    while(a<=9){
        int b=1;
        while(b<=a){
            cout<<b<<"x"<<a<<"="<<b*a<<" ";
            b++;

        }
        a++;
        cout <<endl;
    }
    return 0;
}