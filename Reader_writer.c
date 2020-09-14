#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t mutex;
sem_t wrt;
int readCount=0;

void *Reader(void *arg);
void *Writer(void *arg);

int main()
{
	int i=0,j=0,NumberofReaderThread,NumberofWriterThread;
	sem_init(&mutex,0,1);
	sem_init(&wrt,0,1);
	pthread_t Readers_thr[100],Writer_thr[100];
	printf("\nEnter number of Readers thread(MAX 10)");
	scanf("%d",&NumberofReaderThread);
	printf("\nEnter number of Writers thread(MAX 10)");
	scanf("%d",&NumberofWriterThread);
	for(i=0,j=0;i<NumberofReaderThread&&j<NumberofWriterThread;i++,j++)
	{
		pthread_create(&Writer_thr[i],NULL,Writer,(void *)i);
		pthread_create(&Readers_thr[j],NULL,Reader,(void *)j);
	}
	for(;j<NumberofReaderThread;j++)
	{
		pthread_create(&Writer_thr[j],NULL,Writer,(void *)j);
	}
	for(;i<NumberofWriterThread;i++)
	{
		pthread_create(&Writer_thr[i],NULL,Writer,(void *)i);
	}
	for(i=0;i<NumberofWriterThread;i++)
	{
		pthread_join(Writer_thr[i],NULL);
	}

	for(j=0;j<NumberofReaderThread;j++)
	{
		pthread_join(Readers_thr[j],NULL);
	}
	sem_destroy(&wrt);
	sem_destroy(&mutex);
	printf("\n");
	return 0;
}

void * Writer(void *arg)
{
	sleep(1);
	int temp=(int)arg;
	printf("\nWriter %d is trying to enter into database for modifying the data",temp);
	sem_wait(&wrt);
	printf("\nWriter %d is writting into the database",temp);
	printf("\nWriter %d is leaving the database");
	sem_post(&wrt);
}

void *Reader(void *arg)
{
	sleep(10);
	int temp=(int)arg;
	printf("\nReader %d is trying to enter into the Database for reading the data",temp);
	sem_wait(&mutex);
	readCount++;
	if(readCount==1)
	{
		sem_wait(&wrt);
		printf("\nReader %d is reading the database",temp);
	}
	sem_post(&mutex);
	sem_wait(&mutex);
	readCount--;
	if(readCount==0)
	{
		printf("\nReader %d is leaving the database",temp);
		sem_post(&wrt);
	}
	sem_post(&mutex);
}
