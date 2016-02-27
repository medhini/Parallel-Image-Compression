#include <sys/time.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>

using namespace std;

int main(int argc, char **argv) 
{

    FILE *fp;
    fp=fopen("./info.txt","w"); 
    fprintf(fp,"%f ",9.1f);
    fclose(fp);
return 0;
}
