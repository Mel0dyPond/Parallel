#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct arguments
   {
      int *counter;
      int *times_ran;
      pthread_mutex_t lock;
      pthread_cond_t full;
      pthread_cond_t empty;
   };

void *increment_work(void *arg)
   {
      struct arguments *args = (struct arguments *)arg;
      int *counter = args->counter;
      int *times_ran = args->times_ran;
      pthread_mutex_t lock = args->lock;
      pthread_cond_t full = args->full;
      pthread_cond_t empty = args->empty;
      
      int index = 0;
      
      while(*times_ran < 50)
         {
            pthread_mutex_lock(&lock);
            while(*counter == 10)
               {
                  pthread_cond_wait(&full, &lock);
               }
            
            for(index = 0; index < 10 && *times_ran < 50; index++ )
               {
                  (*counter)++;
                  printf("Count is now (inc fn): %d\n", *counter);
                  (*times_ran)++;
               }
            
            pthread_cond_signal(&empty);
            pthread_mutex_unlock(&lock);
            
            usleep(500000);
         }
      
      return NULL;
   }

void *decrement_work(void *arg)
   {
      struct arguments *args = (struct arguments *)arg;
      int *counter = args->counter;
      int *times_ran = args->times_ran;
      pthread_mutex_t lock = args->lock;
      pthread_cond_t full = args->full;
      pthread_cond_t empty = args->empty;
      
      int index = 0;
      
      while(*times_ran < 50)
         {
            pthread_mutex_lock(&lock);
            while(*counter == 0)
               {
                  pthread_cond_wait(&empty, &lock);
               }
            
            for(index = 0; index < 10 && *times_ran < 50; index++ )
               {
                  (*counter)--;
                  printf("Count is now (dec fn): %d\n", *counter);
                  (*times_ran)++;
               }
            
            pthread_cond_signal(&full);
            pthread_mutex_unlock(&lock);
            
            usleep(500000);
         }
      
      return NULL;
   }


int main()
   {
      int index = 0;
      int counter = 0;
      int times_ran = 0;
      pthread_t thread_1, thread_2;
      pthread_mutex_t lock;
      pthread_cond_t full;
      pthread_cond_t empty;
      struct arguments *arg1, *arg2;
      
      pthread_mutex_init(&lock, NULL);
      pthread_cond_init(&full, NULL);
      pthread_cond_init(&empty, NULL);
      
      arg1 = (struct arguments *) 
         calloc(1, sizeof(struct arguments));
      arg1->counter = &counter;
      arg1->lock = lock;
      arg1->full = full;
      arg1->empty = empty;
      arg1->times_ran = &times_ran;
      
      arg2 = (struct arguments *) 
         calloc(1, sizeof(struct arguments));
      arg2->counter = &counter;
      arg2->lock = lock;
      arg1->full = full;
      arg1->empty = empty;
      arg2->times_ran = &times_ran;
      
      
      pthread_create(&thread_1, NULL, 
         increment_work, (void *)arg1);
         
      pthread_create(&thread_2, NULL, 
         decrement_work, (void *)arg2);
         
         
      pthread_join(thread_1, NULL);
      pthread_join(thread_2, NULL);
      
      free(arg1);
      free(arg2);
      
      return 0;
   }





