#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


int total = 0;;


struct arguments
   {
      int id;
      int *last_added;
   };


void *do_work(void *arg)
   {
      struct arguments *args = (struct arguments *) arg;
      int id = args->id;
      int *last_added = args->last_added;
      
      while(total < 990)
         {
            if(id == (*last_added) + 1)
               {
                  if(total < 990)
                     {
                        total += id;
                        *last_added = id;
                        printf("my num: %d, total: %d\n", id, total);
                        
                        if(*last_added == 9)
                           {
                              *last_added = -1;
                           }
                     }
                  
               }
         }
      
      return NULL;
   }


int main()
   {
      int index = 0;
      int last_added = -1;
      pthread_t thread_array[10];
      struct arguments *arg_array[10];
      
      for(index = 0; index < 10; index++)
         {
            arg_array[index] = (struct arguments *) 
                  calloc(1, sizeof(struct arguments));
            arg_array[index]->id = index;
            arg_array[index]->last_added = &last_added;
                  
            pthread_create(&thread_array[index], 
                  NULL, do_work, (void *)arg_array[index]);
         }
      
      for(index = 0; index < 10; index++)  
         {
            pthread_join(thread_array[index], NULL);
            free(arg_array[index]);
         }    
      
      printf("Total: %d\n", total);
      
      return 0;
   }








