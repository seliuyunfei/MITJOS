#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <errno.h>  
#include <iostream>
using namespace std;
#define BUFFER_SIZE 10//缓冲区大小为10
int buffer[BUFFER_SIZE];
int num=0;
sem_t customedNum;
sem_t mutex,empty,full;//三个信号量，互斥信号量mutex，技术信号量empty和full
int x,y;//生产者和消费者在buffer中下标
int index=-1;
void push(int x)  
{  
  
    if(index<10)  
        buffer[++index] = x;  
    else  
        printf("缓冲区不足!\n");  
  
} 
int pop()  
{  
    int s=-1;  
  
    if(index>=0)  
    {
        s = buffer[index--];
    }
    else  
         printf("缓冲区已空!\n");
  
    return s;  
}  
void *Produce(void *arg)//生产者函数
{       int* produceNum = (int *)arg;
	int j;
	j=0;	
	do
	{
		if(num>=50)
		{
		   pthread_exit((void*)"thread all done");
		}
                sem_wait(&empty);//buffer有空余部分，可以生产，并减一
		sem_wait(&mutex);//形成互斥访问，只能一个线程生产
                push(j);
                cout <<" Producer" << *produceNum<<":生产"<< endl;
		
		j++;
		sem_post(&mutex);//取消互斥
		sem_post(&full);//生成完毕，增加一个可以消费量。
                sleep(1);
	}while (j!=60);
}
void *Customer(void *arg)//消费者函数
{
	int* customerNum = (int *)arg;
        int j,x;
	j=0;	
	do
	{
		if(num>=50)
		{
		   pthread_exit((void*)"thread all done");
		}
                sem_wait(&full);//可以消费的量减一
                sem_wait(&customedNum);
		sem_wait(&mutex);//互斥访问，只能一个线程消费
                x=pop();
		if(x!=-1)
                {
                    cout <<" Consumer"<< *customerNum<<":消费" << endl;  
                }
		j++;
                num++;
		sem_post(&mutex);//取消互斥，其他线程可以消费
		sem_post(&empty);//空余量加一
                sleep(1);
	}while (j!=60);
}

int main()
{

	//semaphore	
	sem_init(&mutex,1,1);//初始化互斥信号量mutex为1
	sem_init(&empty,0,BUFFER_SIZE);//初始化计数信号量empty为BUFFER_SIZE
	sem_init(&full,0,0);//初始化计数信号量full为0
        sem_init(&customedNum,0,50);
	//multipthread
	pthread_t customer[2],produce[3];
	int produce1 = 1,produce2 = 2,produce3 = 3,customer1 = 1,customer2 = 2,pt[5]; 
        pt[2]=pthread_create(&produce[0],NULL,Produce,&produce1);  	
	pt[3]=pthread_create(&produce[1],NULL,Produce,&produce2);  
        pt[4]=pthread_create(&produce[2],NULL,Produce,&produce3);   
        pt[0]=pthread_create(&customer[0],NULL,Customer,&customer1);  
        pt[1]=pthread_create(&customer[1],NULL,Customer,&customer2);  

        //让每个线程在主线程main执行前全部执行完毕。
	pthread_join(customer[0],NULL);
        pthread_join(customer[1],NULL);
        pthread_join(produce[0],NULL);
        pthread_join(produce[1],NULL);
        pthread_join(produce[3],NULL);
	return 0;
}


