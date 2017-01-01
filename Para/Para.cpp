// Para.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"  
#include <iostream>  
#include <pthread.h>  
#include "windows.h"
#include <iomanip>
#include<time.h>
using namespace std;
const long long n = 1000000000;
const int thread_count = 16;
long double volatile sum = 0.0;
long volatile flag = 0;
pthread_mutex_t work_mutex;                    //����������work_mutex

void *Thread_sum(void* rank)
{
	long my_rank = *(long*)rank;
	double factor;
	long double i;
	long double my_n = n / thread_count;
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
	for (long double i = my_first_i; i < my_last_i;++i,factor=-factor)
	{
		sum += factor / (2 * i + 1);
	}
	cout << setprecision(11) << sum * 4 << endl;

	return NULL;
}

//æ�ȴ�
void *Thread_sum_busy(void* rank)
{
	long my_rank = *(long*)rank;
	double factor;
	long double i;
	long double my_n = n / thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	if (my_first_i % 2 == 0)
	{
		factor = 1.0;
	}
	else
	{
		factor = -1.0;
	}
	for (long double i = my_first_i; i < my_last_i; ++i, factor = -factor)
	{
		while (flag != my_rank);
		sum += factor / (2 * i + 1);
		flag = (flag + 1) % thread_count;
	}
	return NULL;
}

//æ�ȴ��������
void *Thread_sum_partbusy(void* rank)
{
	long my_rank = *(long*)rank;
	long double my_sum = 0.0;
	double factor;
	long double my_n = n / thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	if (my_first_i % 2 == 0)
	{
		factor = 1.0;
	}
	else
	{
		factor = -1.0;
	}
	for (long double i = my_first_i; i < my_last_i; ++i, factor = -factor)
	{
		my_sum += factor / (2 * i + 1);
	}
	while (flag != my_rank);
	sum += my_sum;
	flag = (flag + 1) % thread_count;
	return NULL;
}

//�������Ĳ���
void *Thread_sum_lock(void* rank)
{
	long my_rank = *(long*)rank;
	long double my_sum = 0.0;
	double factor;
	long double my_n = n / thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	if (my_first_i % 2 == 0)
	{
		factor = 1.0;
	}
	else
	{
		factor = -1.0;
	}
	for (long double i = my_first_i; i < my_last_i; ++i, factor = -factor)
	{
		my_sum += factor / (2 * i + 1);
	}
	pthread_mutex_lock(&work_mutex);
	sum += my_sum;
	pthread_mutex_unlock(&work_mutex);

	return NULL;
}


int main(int argc, char const *argv[])
{
	clock_t start, finish;
	double totaltime;
	pthread_t thread[thread_count];
	long num[thread_count];
	for (int i = 0; i != thread_count;++i)
	{
		num[i] = i;
	}
	start = clock();
	pthread_mutex_init(&work_mutex, NULL);
	//�����̺߳���
	for (int i = 0; i != thread_count; ++i)
	{
		pthread_create(&thread[i], NULL, Thread_sum_lock, &num[i]);
	}
	//�����߳�
	for (int i = 0; i != thread_count; ++i)
	{
		pthread_join(thread[i], NULL);
	}

	finish = clock();
	totaltime = (double)(finish - start)*1000.0 / CLOCKS_PER_SEC;
	cout << "�߳�������" << thread_count << endl;
	cout << "�����" << setprecision(11) << sum * 4 << endl;
	cout << "����ʱ��: " << totaltime<<" ����" << endl;
	return 0;
}