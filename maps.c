#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct child
{
    int start;
    int end;
};

unsigned long long parallel_sum_mmap(unsigned int nProc,unsigned int N)
{

   
   
    unsigned long long *shared = mmap (NULL,nProc * sizeof (unsigned long long), PROT_READ|PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS,0, 0);
    
    //clock_t st,end;
    //double timeint=0;
    struct child ch[nProc];
    int pid[nProc];
    
   int n = 0;
    n = N / nProc ;
    ch[0].start=1;
    ch[0].end=n;
    
    for(int i=1;i<nProc;i++)
    {
        ch[i].start=ch[i-1].end+1;
        ch[i].end=ch[i].start + n-1;
    }
    
    ch[nProc-1].end= N;
    
 
    unsigned long long int s=0;
    
    //st = clock();
    for(int i=0; i<nProc; i++)
    {
       if ((pid[i]=fork())==0)
        {
            unsigned long long sum=0;
            
            for (unsigned long long j=ch[i].start; j<ch[i].end+1; j++)
            {
                sum=sum+j;
                
            }
            shared[i]=sum;
            exit(0);
        }
    }
    
    //for(int j; j<nProc;j++)
    	while(wait(NULL) >=0 );
        
        
        for(int i=0;i<nProc;i++) // loop will run nProc times
        {
            
            s=s+shared[i];
        
        }
        
    munmap(shared, nProc * sizeof (unsigned long long));
    
    //end = clock();
    //timeint = (double)(end - st)/CLOCKS_PER_SEC;
    //printf("%f,%lld\n",timeint,s);
    //printf("Time =%f\n",timeint);
        //printf("Sum=%d\n",s);
    return s;
}

void test1()
{

unsigned long long int parallel_summmap;
clock_t start2,end2;
double cpu_time_used2;
unsigned int N;

printf("parallel_sum_mmap\n" );
for(unsigned int i=1;i<=1000000;i=i+10000)
{
	start2 = clock();
	parallel_summmap=parallel_sum_mmap(7,i);
	end2 = clock();
	cpu_time_used2=((double)(end2-start2)/CLOCKS_PER_SEC);
	printf("%d,%lld,%f\n",i,parallel_summmap,cpu_time_used2);
}
}

void test2()
{
    const unsigned int N=1000000;

    unsigned long long int parallel_summmap;
    clock_t start2,end2;
    double cpu_time_used2;
    
    printf("parallel_sum_maps\n" );
    for(int i = 1; i < 200; i+=2)
    {
	start2 = clock();
       parallel_summmap=parallel_sum_mmap(i,N);
	end2 = clock();
	cpu_time_used2=((double)(end2-start2)/CLOCKS_PER_SEC);
	printf("%d,%lld,%f\n",i,parallel_summmap,cpu_time_used2);
	
    }
    
}

int main ()
{
    test1(); 
}
    
        


