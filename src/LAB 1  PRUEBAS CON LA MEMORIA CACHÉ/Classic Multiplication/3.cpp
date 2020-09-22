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
    int B=MAX/2;
    start = clock();
    for (int x = 0; x < MAX; x+=B) {
      for (int y = 0; y < MAX; y+=B) {
        for (int z = 0; z < MAX; z+=B) {
          for (int i = x; i < x+B; i++) {
            for (int j = y; j < y+B; j++) {
              for (int k = z; k < z+B; k++) {
                A[i][j] += X[i][k]*Y[k][j];
              }
            }
          }
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