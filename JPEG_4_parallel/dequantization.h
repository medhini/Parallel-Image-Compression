#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include "quantization.h"

using namespace std;

void idct(float **Matrix, float **DCTMatrix, int N, int M);
float **calloc_mat(int dimX, int dimY);

float **calloc_mat(int dimX, int dimY)
{
    float **m = (float**) calloc(dimX, sizeof(float*));
    float *p = (float *) calloc(dimX*dimY, sizeof(float));
    int i;

    for(i=0; i <dimX;i++)
    {
        m[i] = &p[i*dimY];
    }

    return m;
}

void free_mat(float **m){
  free(m[0]);
  free(m);
}

void idct(float **Matrix, float **DCTMatrix, int N, int M){
    int i, j, u, v;
    // #pragma omp parallel for schedule(runtime)
    for (u = 0; u < N; ++u) {
        for (v = 0; v < M; ++v) {
          Matrix[u][v] = 1/4.*DCTMatrix[0][0];
          for(i = 1; i < N; i++){
          Matrix[u][v] += 1/2.*DCTMatrix[i][0];
           }
           for(j = 1; j < M; j++){
          Matrix[u][v] += 1/2.*DCTMatrix[0][j];
           }

           for (i = 1; i < N; i++) {
                for (j = 1; j < M; j++) {
                    Matrix[u][v] += DCTMatrix[i][j] * cos(M_PI/((float)N)*(u+1./2.)*i)*cos(M_PI/((float)M)*(v+1./2.)*j);
                }
            }
        Matrix[u][v] *= 2./((float)N)*2./((float)M);
        }
    }
    //print_mat(Matrix, pixel, pixel);
 }

void dequantizeBlock(int R,int C)
{
  int block[9][9],i,j;
  float **finalBlock = calloc_mat(pixel, pixel);
  float **newBlock = calloc_mat(pixel, pixel);
  //reading fromm the compressed vector 
  for(i=1;i<=pixel;i++){
    for(j=1;j<=pixel;j++){
      block[i][j]=finalMatrixCompress[R][C].v[(i-1)*pixel+j-1];
    }
  }
  //dequantization part
  
  for(i=1;i<=pixel;i++){
    for(j=1;j<=pixel;j++){
        newBlock[i-1][j-1]=(float)block[i][j]*quantArr[i-1][j-1];
    }
  }
  idct(finalBlock,newBlock,pixel,pixel);
  
  for(i=0;i<pixel;i++){
    for(j=0;j<pixel;j++){
      finalMatrixDecompress[(R-1)*pixel+i][(C-1)*pixel+j]=finalBlock[i][j];
    }
  }
  return ;
}
 
void dequantize(int n,int m)
{
 int i,j;
 #pragma omp parallel for schedule(runtime)
 for(i=1;i<=n/pixel;i++){
  for(j=1;j<=m/pixel;j++){
     dequantizeBlock(i,j);
  }
 }
 return ;
}