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


int prod_num[3]={0,1,2};//为了方便创建各个生产者的线程函数

sem_t* mutex;//mutex主要是为了保证其wait和post之间的操作不会让两个ph同时执行，达到类似于原语的效果,这个信号量与主流程关系不大
sem_t* full;//信号量 
sem_t* empty;


int main();
void *prod(void *num);
void produce(int,buffer*);
int insert_item(int,buffer*);



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
    p->locate=0;//


    int i;
	pthread_t thread_id[3];//声明线程ID数组
	sem_init(mutex, 1, 1);//mutex在两个进程的线程间进行共享，初始化为1
	sem_init(full, 1, 0);//多进程 中间参数设为1，可以使这个信号量跨进程
	sem_init(empty, 1, 20);


	for (i = 0; i<3; i++)
    {
        pthread_create(&thread_id[i], NULL, prod, &prod_num[i]);//创建生产者线程函数
        printf("Producer %d is born\n", i + 1);
    }

    for (i = 0; i<3; i++)
        pthread_join(thread_id[i], NULL);//等待thread_id[i]线程结束
}

void* prod(void *num)
{
	while(1)
	{
		int *i = (int*)num;
        sleep(1);
		produce(*i,p);
        sleep(1);
        sleep(0);
	}

	return 0;
}

void produce(int prod_number,buffer* p)
{   
    sem_wait(mutex);
    int seed=rand()%26;

    insert_item(seed,p);

    printf("producer %d produce %d\n",prod_number+1,seed);
    sem_post(mutex);

    sem_wait(empty);//放在前面，避免满了之后producer还产生一次数据
    sem_post(full);

}



int insert_item(int item,buffer *p)
{   
    p->locate+=1;
	p->array[p->locate]=item;
 
	return 0;
}


