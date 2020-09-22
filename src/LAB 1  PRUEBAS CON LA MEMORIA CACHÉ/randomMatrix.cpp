
#include <bits/stdc++.h>

int** random2dArray(unsigned height, unsigned width){
    int** array2D = 0;
    array2D = new int*[height];
    for (int h = 0; h < height; h++){
          array2D[h] = new int[width];
          for (int w = 0; w < width; w++){
                array2D[h][w] = 1+rand()%(1000-1);
          }
    }
    return array2D;
}
int* random1dArray(unsigned width){
    int* array1D = 0;
    for (int w = 0; w < width; w++){
        array1D[w] = 1+rand()%(1000-1);
    }
    return array1D;
}
/*
int main(){
    int** my2DArray = random2dArray(100, 100);
    printf("Array sized [%i,%i] created.\n\n", 100,100);
    for (int h = 0; h < 100; h++){
      for (int w = 0; w < 100; w++){
            printf("%i,", my2DArray[h][w]);
      }
       printf("\n");
      }
            // important: clean up memory
      printf("\n");
      printf("Cleaning up memory...\n");
      for ( int h = 0; h < 100; h++){
        delete [] my2DArray[h];
      }
      delete [] my2DArray;
      my2DArray = 0;
      printf("Ready.\n");
}
*/