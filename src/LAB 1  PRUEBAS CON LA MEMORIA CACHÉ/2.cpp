#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <time.h>
using namespace std;

void matrix_multiplication(int size){
    int MAX = size;
    int A[MAX][MAX],X[MAX][MAX],Y[MAX][MAX];

    unsigned start,end;

    for (int h = 0; h < MAX; h++){
          for (int w = 0; w < MAX; w++){
                A[h][w] = 0;
                X[h][w] = 1+rand()%(100-1);
                Y[h][w] = 1+rand()%(100-1);
          }
    }

    start = clock();
    for(int i = 0; i < MAX; ++i){
        for(int j = 0; j < MAX; ++j){
            for(int k = 0; k < MAX; ++k)
            {
                A[i][j] += X[i][k] * Y[k][j];
            }
        }
    }
    end = clock();
    cout << "Elapsed time with "<<size<<" : "<< (double(end-start)/CLOCKS_PER_SEC)<<endl;
}

int main(){
    matrix_multiplication(100);
    matrix_multiplication(200);
    matrix_multiplication(300);
    matrix_multiplication(400);
    matrix_multiplication(500);
}