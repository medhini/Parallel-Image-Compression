#include <sys/time.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;

int main(int argc, char **argv) 
{

    FILE *fp;
    fp=fopen("./info.txt","r");
    ofstream MyFile;
    MyFile.open ("bench_marking.csv", ios::out | ios::ate | ios::app | ios::binary);

    float ans=0.0f,temp;
    while(fscanf(fp,"%f ",&temp)==1){
    ans+=temp;
    }
    cout<<ans<<endl;
    MyFile<<5<<','<<ans<<endl;
    MyFile.close();
    fclose(fp);
return 0;
}
