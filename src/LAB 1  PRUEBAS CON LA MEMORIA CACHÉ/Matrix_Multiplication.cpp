#include <iostream>
#include <chrono>
#include <ctime>
#include <time.h>
#include <fstream>
const int MAX=500;


using namespace std;

int main()
{
    // Using time point and system_clock
    double A[MAX][MAX], B[MAX][MAX], R[MAX][MAX];
    ofstream myfile;
    myfile.open ("matrix1.txt");

    srand (time(NULL));

    for (int i=0;i<MAX;i++){
      for (int j=0;j<MAX;j++){
        A[i][j]=rand()%10;
        B[i][j]=rand()%10;
        R[i][j]=0;
      }
    }
    /*
    for (int i=0;i<MAX;i++){
      for (int j=0;j<MAX;j++){
        cout<<A[i][j]<<" ";
      }
      cout<<endl;
    }
    cout<<"----------------"<<endl;
    for (int i=0;i<MAX;i++){
      for (int j=0;j<MAX;j++){
        cout<<B[i][j]<<" ";
      }
      cout<<endl;
    }
    */
    //Primer loop
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    for(int i = 0; i < MAX; ++i)
        for(int j = 0; j < MAX; ++j)
            for(int k = 0; k < MAX; ++k)
            {
                R[i][j] += A[i][k] * B[k][j];
            }
    //std::cout << "f(42) = " << fibonacci(42) << '\n';
    end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end - start;

    time_t end_time = chrono::system_clock::to_time_t(end);
    cout<<"1st Loop"<<endl;
    cout << "finished computation at " << ctime(&end_time)
        << "elapsed time: " << elapsed_seconds.count() << "s\n";
    for (int i=0;i<MAX;i++){
      for (int j=0;j<MAX;j++){
        myfile<<R[i][j]<<" ";
      }
      myfile<<"\n";
    }
    myfile.close();

    ofstream myfile2;
    myfile2.open ("matrix2.txt");

    //MATRIX R=0
    for (int i=0;i<MAX;i++){
      for (int j=0;j<MAX;j++){
        R[i][j]=0;
      }
    }
    //Segundo loop
    // B2 - numero de bloques
    // https://malithjayaweera.com/2020/07/blocked-matrix-multiplication/
    // https://iitd-plos.github.io/col729/lec/matrix_multiplication.html
    int B2=MAX/2;
    chrono::time_point<chrono::system_clock> start1, end1;
    start1 = chrono::system_clock::now();

    for (int ii = 0; ii < MAX; ii+=B2) {
      for (int jj = 0; jj < MAX; jj+=B2) {
        for (int kk = 0; kk < MAX; kk+=B2) {
          for (int i = ii; i < ii+B2; i++) {
            for (int j = jj; j < jj+B2; j++) {
              for (int k = kk; k < kk+B2; k++) {
                R[i][j] += A[i][k]*B[k][j];
              }
            }
          }
        }
      }
    }
    end1 = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds1 = end1 - start1;
    time_t end_time1 = chrono::system_clock::to_time_t(end1);
    cout<<"2nd Loop"<<endl;
    cout << "finished computation at " << ctime(&end_time1)
        << "elapsed time: " << elapsed_seconds1.count() << "s\n";
    for (int i=0;i<MAX;i++){
      for (int j=0;j<MAX;j++){
        myfile2<<R[i][j]<<" ";
      }
      myfile2<<endl;
    }
    myfile2.close();
}
