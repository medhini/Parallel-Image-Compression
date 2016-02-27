#include <sys/time.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>

using namespace std;

int main(int argc, char **argv) 
{

    FILE *fp;
    fp=fopen("./info.txt","r");
    float ans=0.0f,temp;
    while(fscanf(fp,"%f ",&temp)==1){
    ans+=temp;
    }
    cout<<ans<<endl;
    fclose(fp);
return 0;
}
