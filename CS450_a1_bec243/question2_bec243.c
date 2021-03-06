#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>


struct arguments
   {
      int generate;
      int *times_ran;
      int *correct_runs;
      int *index;
      int *array; 
      bool *other_finished;
      pthread_mutex_t lock;
   };



void *do_work(void *arg)
   {
      struct arguments *args = (struct arguments *)arg;
      int id = args->generate;
      int *times_ran = args->times_ran;
      int *correct_runs = args->correct_runs;
      int *array = args->array;
      int *index = args->index;
      bool *other_finished = args->other_finished;
      pthread_mutex_t lock = args->lock;
      
      while(*correct_runs < 10 && *other_finished == false)
         {
            pthread_mutex_lock(&lock);
            if(*index < 3)
               {
                  printf("My id: %d\n", id);
                  array[*index] = id;
                  (*index)++;
               }
            pthread_mutex_unlock(&lock);
            
            usleep(500000);
            
            
            pthread_mutex_lock(&lock);
            if(*index == 3)
               {
                  if(array[0] == 1 && array[1] == 2 && array[2] == 3)
                     {
                        printf("123\n");
                        (*correct_runs)++;
                     }
                  (*times_ran)++;
                  array[0] = array[1] = array[2] = 0;
                  *index = 0;
               }
               
            
            pthread_mutex_unlock(&lock);
            
         }
         
      if(*other_finished == false)
         {
            pthread_mutex_lock(&lock);
            *other_finished = true;
            printf("Team 1 Won!\n");
            pthread_mutex_unlock(&lock);
         }   
      
      return NULL;
   }
   
void *do_work2(void *arg)
   {
      struct arguments *args = (struct arguments *)arg;
      int id = args->generate;
      int *times_ran = args->times_ran;
      int *correct_runs = args->correct_runs;
      int *array = args->array;
      int *index = args->index;
      bool *other_finished = args->other_finished;
      pthread_mutex_t lock = args->lock;
      
      while(*correct_runs < 10 && *other_finished == false)
         {
            pthread_mutex_lock(&lock);
            if(*index < 3)
               {
                  printf("My id: %d\n", id);
                  array[*index] = id;
                  (*index)++;
               }
            pthread_mutex_unlock(&lock);
            
            usleep(500000);
            
            pthread_mutex_lock(&lock);
            if(*index == 3)
               {
                  if(array[0] == 4 && array[1] == 5 && array[2] == 6)
                     {
                        printf("456\n");
                        (*correct_runs)++;
                     }
                  (*times_ran)++;
                  array[0] = array[1] = array[2] = 0;
                  *index = 0;
               }
               
            pthread_mutex_unlock(&lock);
            
            
         }
         
      if(*other_finished == false)
         {
            pthread_mutex_lock(&lock);
            *other_finished = true;
            printf("Team 2 Won!\n");
            pthread_mutex_unlock(&lock);
         }
         
      return NULL;
   }


int main()
   {
      int array_index1 = 0;
      int array1[3] = {};
      int times_ran1 = 0;
      int correct_runs1 = 0;
      pthread_mutex_t lock1;
      pthread_mutex_init(&lock1, NULL);
      
      int array_index2 = 0;
      int array2[3] = {};
      int times_ran2 = 0;
      int correct_runs2 = 0;
      pthread_mutex_t lock2;
      pthread_mutex_init(&lock2, NULL);
      
      bool other_finished = false;
      
      int index = 0;
      pthread_t thread_array[6];
      struct arguments *arg[6];
      
      for(index = 0; index < 3; index++)
         {
            arg[index] = 
               (struct arguments *) calloc(1, sizeof( struct arguments ));
               
            arg[index]->generate = index + 1;
            arg[index]->times_ran = &times_ran1;
            arg[index]->correct_runs = &correct_runs1;
            arg[index]->index = &array_index1;
            arg[index]->array = array1;
            arg[index]->other_finished = &other_finished;
            arg[index]->lock = lock1;
         }
         
      for(index = 3; index < 6; index++)
         {
            arg[index] = 
               (struct arguments *) calloc(1, sizeof( struct arguments ));
               
            arg[index]->generate = index + 1;
            arg[index]->times_ran = &times_ran2;
            arg[index]->correct_runs = &correct_runs2;
            arg[index]->index = &array_index2;
            arg[index]->array = array2;
            arg[index]->other_finished = &other_finished;
            arg[index]->lock = lock2;
         }
         
      for(index = 0; index < 3; index++)
         {
            pthread_create(&thread_array[index], 
                  NULL, do_work, (void *)arg[index]);
                     
            pthread_create(&thread_array[index + 3], 
                  NULL, do_work2, (void *)arg[index + 3]);
         }
         
      for(index = 0; index < 6; index++)
         {
            pthread_join(thread_array[index], NULL);
            free(arg[index]);
         }
      
      
      printf("Total sequences generated team1: %d\n", times_ran1);
      printf("Number of correct sequences team1: %d\n", correct_runs1);
      printf("Total sequences generated team2: %d\n", times_ran2);
      printf("Number of correct sequences team2: %d\n", correct_runs2);
      
      
      return 0;
   }









