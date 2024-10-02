/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Processes - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include "./libprocesslab/libprocesslab.h"


void question1()
{
    // TODO
    int total = 0;
    int value = 1;
    int status;
    registerProc(getpid(),getppid(),0,0);
    if (fork() == 0) {// 1.1 
        registerProc(getpid(),getppid(),1,1);
 
        if(fork()==0){ // 2.1
            registerProc(getpid(),getppid(),2,1);
            _exit(value);
        }
        while(wait(&status)>0)
        {
            total += WEXITSTATUS(status);
        }
        _exit(total+1); 
   
    }

    if (fork() == 0) {// 1.2
        registerProc(getpid(),getppid(),1,2);

        if(fork()==0){ // 2.2
            registerProc(getpid(),getppid(),2,2);
            _exit(value);
        }
         while(wait(&status)>0)
        {
            total += WEXITSTATUS(status);
        }
        _exit(total+1); 
    }

    if (fork() == 0) {// 1.3
        registerProc(getpid(),getppid(),1,3);

        if(fork()==0){ // 2.3
            registerProc(getpid(),getppid(),2,3);
            _exit(value); 
        }
        if(fork()==0){ // 2.4
            registerProc(getpid(),getppid(),2,4);
            _exit(value);
        }
        if(fork()==0){ // 2.5
            registerProc(getpid(),getppid(),2,5);
            _exit(value);
        }
        if(fork()==0){ // 2.6
            registerProc(getpid(),getppid(),2,6);
            _exit(value);
        }
         while(wait(&status)>0)
        {
            total += WEXITSTATUS(status);
        }
        _exit(total+1); 
    }
    while(wait(&status)>0)
    {
        total += WEXITSTATUS(status);
    }
    printf("Total des enfants directs et indirects du processus 0:  %d  \n", total);
    printProcRegistrations();
    execl("/bin/ls", "ls","-l", NULL);
}

