#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

void print_somme(int x,int y, int j)
{
    int l_x=(int)x; 
    int l_y=(int)y; 
    std::cout << "lancé " << j <<std::endl;
	sleep(j);
    std::cout<<l_x<<" + "<<l_y<<" = "<<(l_x+l_y)<<std::endl;
}
int main()
{
  
    # ifdef _OPENMP
      printf("Compiled by an OpenMP-compliant implementation.\n");
      omp_set_num_threads(10);
    # endif
    int i;
    #pragma omp parallel
    for (i =0; i<10; i++)
    {
	print_somme(i,i+1,i);
    }
}