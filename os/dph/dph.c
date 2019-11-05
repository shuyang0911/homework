#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int state[5];
int phil_num[5]={0,1,2,3,4};//为了方便创建各个哲学家的线程函数

#define thinking 0
#define hungry 1
#define eating 2
#define left (philo_number+4)%5
#define right (philo_number+1)%5

void* philo(void* num);
void pickup_forks(int);
void return_forks(int);
void test(int);
int main();

sem_t mutex;//mutex主要是为了保证其wait和post之间的操作不会让两个ph同时执行，达到类似于原语的效果,这个信号量与主流程关系不大
sem_t S[5];//5个信号量，分别代表5个fork
int main()
{
    int i;
    pthread_t thread_id[5];//声明线程ID数组
    sem_init(&mutex, 0, 1);//mutex在一个进程的线程间进行共享，初始化为1

    for (i = 0; i<5; i++)//初始化S数组
        sem_init(&S[i], 0, 0);//初始化S[i]的值为零
    
    for (i = 0; i<5; i++)
    {
        pthread_create(&thread_id[i], NULL, philo, &phil_num[i]);//创建哲学家线程函数
        printf("Philosopher %d is born\n", i + 1);
    }
 
    for (i = 0; i<5; i++)
        pthread_join(thread_id[i], NULL);//等待thread_id[i]线程结束

}

void* philo(void *num)
{
	while(1)
	{
		int *i = (int*)num;
        sleep(rand()% 5);
		pickup_forks(*i);
        sleep(0);
		return_forks(*i);
	}

	return 0;
}

void pickup_forks(int philo_number)
{
	sem_wait(&mutex);
	state[philo_number] = hungry;
    printf("philosopher %d want to eat\n",philo_number+1);
    test(philo_number);
    sem_post(&mutex);

    sem_wait(&S[philo_number]);//拿掉左右的fork
}

void test(int philo_number)
{   //只要这个ph正在hungry且左右的ph都不在hungry（即左右fork都空闲）
    if (state[philo_number] = hungry && state[left] != eating && state[right] != eating)
    {
        state[philo_number] = eating;
        printf("Philosopher %d is eating\n", philo_number+1);
        sleep(2);//吃两秒
        sem_post(&S[philo_number]);//吃完了
    }

}

void return_forks(int philo_number)
{
    sem_wait(&mutex);
    state[philo_number] = thinking;
    printf("Philosopher %d stop eating and begin to think\n",philo_number+1);
    test(left);//询问左右是否要用到他的fork
    test(right);
    sem_post(&mutex);
}
