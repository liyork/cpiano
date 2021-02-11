#include <errno.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <assert.h>  
#include <semaphore.h> 

sem_t * CreateSemaphore( const char * inName, const int inStartingCount )  
{  
    sem_t * semaphore = sem_open( inName, O_CREAT, 0644, inStartingCount );  
      
    //  
    if( semaphore == SEM_FAILED )  
    {  
        switch( errno )  
        {  
            case EEXIST:  
                printf( "Semaphore with name '%s' already exists.\n", inName );  
                break;  
                  
            default:  
                printf( "Unhandled error: %d.\n", errno );  
                break;  
        }  
        return SEM_FAILED;  
    }  
      
    //  
    return semaphore;  
}  
  
  
//  
void DestroySemaphore( sem_t * inSemaphore )  
{  
    int retErr = sem_close( inSemaphore );  
      
    //  
    if( retErr == -1 )  
    {  
        //  
        switch( errno )  
        {  
            case EINVAL:  
                printf( "inSemaphore is not a valid sem_t object." );  
                break;  
                  
            default:  
                printf( "Unhandled error: %d.\n", errno );  
                break;  
        }  
          
        //  
    }  
      
    //  
}  
  
//  
void SignalSemaphore( sem_t * inSemaphore )  
{  
    sem_post( inSemaphore );  
}  
  
//  
void WaitSemaphore( sem_t * inSemaphore )  
{  
    sem_wait( inSemaphore );  
}  
  

//int main(int argc, char *argv[]){
//  sem_t *s = CreateSemaphore("a", 1);
//  WaitSemaphore(s);
//  printf("wait..");
//  SignalSemaphore(s);
//  WaitSemaphore(s);
//  printf("wait..i2");
//  SignalSemaphore(s);
//  printf("finish");
//  return 0;
//}

// gcc -o conhash conhash.c -Wall
