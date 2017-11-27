#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;
int main ()
{ 
	step = 1.0/(double) num_steps;
	int step_per_intervals = num_steps / 10;
	int sections = num_steps / step_per_intervals;
	double sums[sections];
	for (int i = 0; i < sections; i++) 
		sums[i]=0.0;

	double pi = 0.0;
	#pragma omp parallel num_threads(sections)
	{
		double x = 0;
		int i = omp_get_thread_num()*step_per_intervals;
		int limit = (omp_get_thread_num()+1)*step_per_intervals;
		double local_thread_sum = 0;
		printf("thread id is %d\n", omp_get_thread_num());
		

		for (; i<limit ; i++){
			x = (i+0.5)*step;
			local_thread_sum += 4.0/(1.0+x*x);
		}
		sums[omp_get_thread_num()] = step * local_thread_sum;
	}

	for (int i =0 ; i < sections ; i++ ) 
	{
		pi += sums[i];
	 	printf ("pi %f\n", pi);
	}


}
