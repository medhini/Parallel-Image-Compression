#include <sys/time.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include "dequantization.h"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <string>

#define numThreads 8 //total number of threads

using namespace cv;
using namespace std;

int n,m;




void dct(float **DCTMatrix, float **Matrix, int N, int M);
void write_mat(FILE *fp, float **testRes, int N, int M);
void free_mat(float **p);
void divideMatrix(float **grayContent, int dimX, int dimY, int n, int m);



void print_mat(float **m, int N, int M){

   for(int i = 0; i < N; i++)
   {
    for(int j = 0; j < M; j++)
    {
      cout<<m[i][j]<<" ";
    }
    cout<<endl;
   }
   cout<<endl;
}

void divideMatrix(float **grayContent, int dimX, int dimY, int n, int m)
{
  float **smallMatrix = calloc_mat(dimX, dimY);
  float **DCTMatrix = calloc_mat(dimX, dimY);
  float **newMatrix = calloc_mat(dimX, dimY);

  #pragma omp parallel for schedule (runtime)
  for(int i = 0; i < n; i += dimX)
  {
    for(int j = 0; j < m; j += dimY)
    {
      for(int k = i; k < i + pixel && k < n; k++)
      {
        for(int l = j; l < j + pixel && l < m; l++)
        {
          smallMatrix[k-i][l-j] = grayContent[k][l];
        }
      }
      dct(DCTMatrix, smallMatrix, dimX, dimY);
      for(int k=i; k<i+pixel && k<n ;k++)
      {
        for(int l=j; l<j+pixel && l<m ;l++)
        {
          globalDCT[k][l]=DCTMatrix[k-i][l-j];

        }
      } 
    }
  }
  delete [] smallMatrix;
  delete [] DCTMatrix;
  // free_mat(smallMatrix);
  // free_mat(DCTMatrix);
}


void dct(float **DCTMatrix, float **Matrix, int N, int M){

    int i, j, u, v;
    // #pragma omp parallel for schedule(runtime)
    for (u = 0; u < N; ++u) {
        for (v = 0; v < M; ++v) {
        DCTMatrix[u][v] = 0;
            for (i = 0; i < N; i++) {
                for (j = 0; j < M; j++) {
                    DCTMatrix[u][v] += Matrix[i][j] * cos(M_PI/((float)N)*(i+1./2.)*u)*cos(M_PI/((float)M)*(j+1./2.)*v);
                }
            }
        }
    }
    //print_mat(DCTMatrix, 8, 8);
 }

void compress(Mat img,int num)
{

      
      
      n = img.rows;
      m = img.cols;

      int add_rows = (pixel - (n % pixel) != pixel ? pixel - (n % pixel) : 0);
      int add_columns = (pixel - (m % pixel) != pixel ? pixel - (m % pixel) : 0) ;

      float **grayContent = calloc_mat(n + add_rows, m + add_columns);

      int dimX = pixel, dimY = pixel;
      
      #pragma omp parallel for schedule(runtime)
      for (int i = 0; i < n; i++)
      {
        for(int j = 0; j < m; j++)    
        {
          Vec3b bgrPixel = img.at<Vec3b>(i, j);
          grayContent[i][j] = (bgrPixel.val[0] + bgrPixel.val[1] + bgrPixel.val[2])/3;
        }
      }

      //setting extra rows to 0
      #pragma omp parallel for schedule(runtime)
      for (int j = 0; j < m; j++)
      {
        for (int i = n; i < n + add_rows; i++)
        {
          grayContent[i][j] = 0;
        }
      }

      //setting extra columns to 0
      #pragma omp parallel for schedule(runtime)
      for (int i = 0; i < n; i++)
      {
        for(int j = m; j < m + add_columns; j++)
        {
          grayContent[i][j] = 0;
        }
      }

      n = add_rows + n; //making rows as a multiple of 8
      m = add_columns + m; //making columns as a multiple of 8

    divideMatrix(grayContent, dimX, dimY, n, m);

    
    quantize(n,m);
    dequantize(n,m);

    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < n; i++)
      {
        for(int j = 0; j < m; j++)    
        {
          Vec3b bgrPixel;
          bgrPixel.val[0] = bgrPixel.val[1] = bgrPixel.val[2] = finalMatrixDecompress[i][j];
          img.at<Vec3b>(i,j) = bgrPixel;
        }
      }

    // free_mat(grayContent);  
    //delete [] grayContent;
   free(grayContent);
}


int main(int argc, char **argv) 
{

    FILE *fp;
    fp=fopen("./info.txt","a+"); 
    //omp_set_num_threads(numThreads);
    string str="./images/";
    string ext=".jpg";

    struct timeval  TimeValue_Start, ts;
    struct timezone TimeZone_Start, tz_s;

    struct timeval  TimeValue_Final, tf;
    struct timezone TimeZone_Final, tz_f;
    long time_start, time_end;
    double time_overhead;

     //Time before starting parallel execution
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
      string path=str+argv[1]+ext;
      Mat img=imread(path,CV_LOAD_IMAGE_GRAYSCALE);
      compress(img,0);
      cout<<path<<endl;
    gettimeofday(&TimeValue_Final, &TimeZone_Final);

    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    fprintf(fp,"%f ",(float)time_overhead);
    fclose(fp);
return 0;
}
