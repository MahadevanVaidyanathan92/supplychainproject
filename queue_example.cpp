#include<iostream>
#include<queue>
using namespace std;
main()
   {
        queue <int> Q;
        int i;
        for(i=0;i<10;i++)
            if( i%2) Q.push(i*3);
            else Q.push(i);
        while(!Q.empty())
        {
                cout<<Q.front()<<" ";
                Q.pop();
           }

      }
