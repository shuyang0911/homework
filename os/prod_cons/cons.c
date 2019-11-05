#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

#include "buffer.h"



int cons_num[3]={0,1,2};//为了方便创建各个消费者的线程函数

sem_t* mutex;//mutex主要是为了保证其wait和post之间的操作不会让两个ph同时执行，达到类似于原语的效果,这个信号量与主流程关系不大
sem_t* full;//信号量 
sem_t* empty;


int main();
void *cons(void *num);
void consume(int,buffer*);
int remove_item(int*,buffer*);



int main()
{   
    int fd,buffer_fd,locate_fd;
    fd=open("data",O_CREAT|O_RDWR,0666); 
    buffer_fd= shm_open("/shm_buffer", O_CREAT | O_RDWR, 0644);// 
      
    
    ftruncate(fd,1024); 
    ftruncate(buffer_fd,sizeof(buffer));//   
    mutex=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0); 
    full=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);    
    empty=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    p =(buffer*) mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, buffer_fd, 0);//

	int i;
	pthread_t thread_id[3];//声明线程ID数组

	for (i = 0; i<3; i++)
    {
        pthread_create(&thread_id[i], NULL, cons, &cons_num[i]);//创建生产者线程函数
        printf("Consumer %d is born\n", i + 1);
    }

    for (i = 0; i<3; i++)
        pthread_join(thread_id[i], NULL);//等待thread_id[i]线程结束
}

void* cons(void *num)
{
	while(1)
	{
		int *i = (int*)num;

        sleep(1);
		consume(*i,p);
        sleep(1);
        sleep(0);

	}

	return 0;
}

void consume(int cons_number,buffer*p)
{   
    sem_post(empty);
	sem_wait(full);

    sem_wait(mutex);

    int tree;

    remove_item(&tree,p);
    printf("consumer %d consume %d\n",cons_number+1,tree);
    sem_post(mutex);

}

int remove_item(int* item,buffer*p)
{   

	(*item) = p->array[p->locate];
	p->locate-=1;

	return 0;
}

