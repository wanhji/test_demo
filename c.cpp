#include<iostream>
#include<string>
#include<cmath>
using namespace std;

class MyClass{
    public:
    void fun(int leng[],int len)
    {int b=0;
     int a=0;

        for(int i=1;i<len;i++)
        {
            if(leng[i]<=leng[a])
            {
             a=i;
            }


        }
        b=leng[a];
        leng[a]=leng[len-1];
        leng[len-1]=b;
    }

    };
    int main()
    {int len;
     int y=0;   
        cout <<"输入长度";
        cin >>len;
        int leng[len-1];
        for(int x=0;x<len;x++)
        {   cout<<"输入数据";
            cin>>leng[x];

        }
        MyClass ow;
        ow.fun(leng,len);
        for(y=0;y<len;y++)
        {
            cout<<leng[y]<<" ";

        }
    return 0;
    
    }
