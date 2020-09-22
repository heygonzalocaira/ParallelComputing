#include <bits/stdc++.h>
#include "randomMatrix.cpp"
#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;
int main(){
    int MAX = 100;
    int A[MAX][MAX];
    int y[MAX];
    int x[MAX];
    unsigned start,end;

    for (int h = 0; h < MAX; h++){
          for (int w = 0; w < MAX; w++){
                A[h][w] = 1+rand()%(100-1);
          }
    }
    for (int w = 0; w < MAX; w++){
        x[w] = 1+rand()%(100-1);
    }

    start = clock();
    for(int i = 0; i< MAX;i++){
        for(int j =0; j<MAX;j++){
            y[i] +=A[i][j]*x[j];
        }
    }
    end = clock();
    cout << "Elapsed time 1: " << (double(end-start)/CLOCKS_PER_SEC)<<endl;

    for (int w = 0; w < MAX; w++){
        y[w] = 0;
    }
    
    start = clock();
    for(int j = 0; j< MAX; j++){
        for(int i =0; i<MAX; i++){
            y[i] +=A[i][j]*x[j];
        }
    }
    end = clock();
    cout << "Elapsed time 2: " << (double(end-start)/CLOCKS_PER_SEC)<<endl;
}