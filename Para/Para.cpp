// Para.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"  
#include <iostream>  
#include <pthread.h>  
#include "windows.h"
using namespace std;
const int n = 1000;
const int thread_count = 2;
long long sum = 0.0;

void *Thread_sum(void* rank)
{
	long my_rank = *(long*)rank;
	double factor;
	long long i;
	long long my_n = n / thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	if (my_first_i % 2==0)
	{
		factor = 1.0;
	}
	else
	{
		factor = -1.0;
	}
	for (int i = my_first_i; i < my_last_i;++i,factor=-factor)
	{
		sum += factor / (2 * i + 1);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thread[n];
	long num1 = 0.0;
	long num2 = 1.0;
	pthread_create(&thread[0], NULL, Thread_sum, &num1);
	pthread_create(&thread[1], NULL, Thread_sum, &num2);
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	//system("pause");
	//Sleep(1000);
	Sleep(10000);
	cout << sum << endl;
	return 0;

}