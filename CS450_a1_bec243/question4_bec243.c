#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct arguments
   {
      int *counter;
      int *times_ran;
      pthread_mutex_t lock;
      pthread_cond_t condition;
   };

void *increment_work(void *arg)
   {
      struct arguments *args = (struct arguments *)arg;
      int *counter = args->counter;
      int *times_ran = args->times_ran;
      pthread_mutex_t lock = args->lock;
      pthread_cond_t condition = args->condition;
      
      int index = 0;
      
      while(*times_ran < 50)
         {
            pthread_mutex_lock(&lock);
            while(*counter == 10)
               {
                  pthread_cond_wait(&condition, &lock);
               }
            
            for(index = 0; index < 10; index++ )
               {
                  (*counter)++;
                  printf("Count is now (inc fn): %d\n", *counter);
                  (*times_ran)++;
               }
            
            pthread_cond_signal(&condition);
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
      pthread_cond_t condition = args->condition;
      
      int index = 0;
      
      while(*times_ran < 50)
         {
            usleep(500000);
            
            pthread_mutex_lock(&lock);
            while(*counter == 0)
               {
                  pthread_cond_wait(&condition, &lock);
               }
            
            for(index = 0; index < 10; index++ )
               {
                  (*counter)--;
                  printf("Count is now (dec fn): %d\n", *counter);
                  (*times_ran)++;
               }
            
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&lock);
            
            
         }
      
      return NULL;
   }


int main()
   {
      int index = 0;
      int counter = 0;
      int times_ran = 0;
      pthread_t thread_1, thread_2;
      pthread_mutex_t lock_inc, lock_dec;
      pthread_cond_t condition;
      struct arguments *arg1, *arg2;
      
      pthread_mutex_init(&lock_inc, NULL);
      pthread_mutex_init(&lock_dec, NULL);
      pthread_cond_init(&condition, NULL);
      
      arg1 = (struct arguments *) 
         calloc(1, sizeof(struct arguments));
      arg1->counter = &counter;
      arg1->lock = lock_inc;
      arg1->condition = condition;
      arg1->times_ran = &times_ran;
      
      arg2 = (struct arguments *) 
         calloc(1, sizeof(struct arguments));
      arg2->counter = &counter;
      arg2->lock = lock_dec;
      arg2->condition = condition;
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





