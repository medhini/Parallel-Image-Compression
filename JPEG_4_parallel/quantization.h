#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<omp.h>
#define pixel 8
using namespace std;

int quantArr[8][8]={{16,11,12,14,12,10,16,14},
      {13,14,18,17,16,19,24,40},
      {26,24,22,22,24,49,35,37},
      {29,40,58,51,61,60,57,51},
      {56,55,64,72,92,78,64,68},
      {87,69,55,56,80,109,81,87},
      {95,98,103,104,103,62,77,113},
      {121,112,100,120,92,101,103,99}
  };

  float globalDCT[3005][3005];

struct INF_COMPRESS
{
  vector<int> v;
}finalMatrixCompress[3000][3000];

int finalMatrixDecompress[3000][3000];

void quantizeBlock(int R,int C)
{
 //Quantization part
 int block[9][9],i,j;

 for(i=1;i<=pixel;i++)
   for(j=1;j<=pixel;j++)
      block[i][j]=globalDCT[(R-1)*pixel+i-1][(C-1)*pixel+j-1];
 

 for(i=1;i<=pixel;i++)
   for(j=1;j<=pixel;j++)
      block[i][j]=(int)round((float)block[i][j]/quantArr[i-1][j-1]);
 
 vector<int> vRLE;
 for(i=1;i<=pixel;i++){
  for(j=1;j<=pixel;j++){
     vRLE.push_back(block[i][j]);
  }
 }
 finalMatrixCompress[R][C].v=vRLE;
 //ohilfunc(vRLE);
 return ;
}
 
void quantize(int n,int m)
{
 int i,j;
 // #pragma omp parallel for schedule(runtime)
 for(i=1;i<=n/pixel;i++){
  for(j=1;j<=m/pixel;j++){
     quantizeBlock(i,j);
   // encodedData[i][j]=quantizeBlock(i,j);
  }
 }
 return ;
}


