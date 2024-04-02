#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int N_ITER;

int result;

void *count (void *arg){
    int i=0;

    for(i=0; i< N_ITER; i++){
        result = result + 1;
    }
    return (void*)0;
}

int main (int argc, char **argv)
{

  int i ;
  int nb_threads = 0 ;
  pthread_t *tids;

  result=0;

  if (argc != 3){
    fprintf(stderr, "usage : %s N_ITER nb_threads\n", argv[0]) ;
    exit (-1) ;
  }

  N_ITER = atoi (argv[1]) ;
  nb_threads = atoi (argv[2]) ;
  tids = malloc (nb_threads*sizeof(pthread_t)) ;
  
  for (i = 0 ; i < nb_threads; i++){
      if(pthread_create (&tids[i], NULL, count, NULL) != 0){
          fprintf(stderr,"Failed to create thread number %d\n",i);
      }
  }

  /* Wait until every thread ended */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;

  printf("the result of the computation is %d\n",result);
  
  return EXIT_SUCCESS;
}
