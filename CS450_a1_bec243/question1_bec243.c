#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct arguments
   {
      int generate;
      int *times_ran;
      int *correct_runs;
      int *index;
      int *array;
      pthread_mutex_t lock;
      pthread_cond_t array_full;
   };

void *do_work(void *arg)
   {
      struct arguments *args = (struct arguments *) arg;
      int generate = args->generate;
      int *times_ran = args->times_ran;
      int *correct_runs = args->correct_runs;
      int *array_index = args->index;
      int *array = args->array;
      pthread_mutex_t lock = args->lock;
      pthread_cond_t array_full = args->array_full;
      
      while(*correct_runs < 10)
         {
            pthread_mutex_lock(&lock);
            printf("My id: %d\n", generate);
            array[*array_index] = generate;
            (*array_index)++;
            pthread_mutex_unlock(&lock);
            
            usleep(500000);
            
            // array full
            if(*array_index == 3)
               {
                  pthread_mutex_lock(&lock);
                  
                  if(array[0] == 1 && array[1] == 2 && array[2] == 3)
                     {
                        (*correct_runs)++;
                        printf("123\n");
                     }
                  
                  array[0] = array[1] = array[2] = 0;
                  *array_index = 0;
                  (*times_ran)++;
                  
                  pthread_cond_signal(&array_full);
                  pthread_mutex_unlock(&lock);
               }
            // array not yet full
            else
               {
                  pthread_mutex_lock(&lock);
                  while(*array_index != 3)
                     {
                        pthread_cond_wait(&array_full, &lock);
                     }
                  pthread_mutex_unlock(&lock);
               }
         }
      
      
      return NULL;
   }


int main()
   {
      int index;
      pthread_mutex_t lock;
      pthread_cond_t array_full;
      int array_index = 0;
      int times_ran = 0;
      int correct_runs = 0;
      int store_array[3] = {};
      pthread_t thread_array[3];
      struct arguments *arg_array[3];
      
      pthread_mutex_init(&lock, NULL);
      pthread_cond_init(&array_full, NULL);
      
      for(index = 0; index < 3; index++)
         {
            arg_array[index] = 
                  (struct arguments*) calloc(1, sizeof(struct arguments));
                  
            arg_array[index]->generate = index + 1;
            arg_array[index]->times_ran = &times_ran;
            arg_array[index]->correct_runs = &correct_runs;
            arg_array[index]->index = &array_index;
            arg_array[index]->array = store_array;
            arg_array[index]->lock = lock;
            arg_array[index]->array_full = array_full;
            
            
            pthread_create(&thread_array[index], NULL, 
                  do_work, (void *)arg_array[index]);
         }
         
      for(index = 0; index < 3; index++)
         {
            pthread_join(thread_array[index], NULL);
         }
      
      printf("Total sequences generated: %d\n", times_ran);
      printf("Number of correct sequences: %d\n", correct_runs);
      
      return 0;
   }







