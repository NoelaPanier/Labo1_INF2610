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
    registerProc(getpid(),getppid(),0,0);
    if (fork() == 0) {// 1.1 
        registerProc(getpid(),getppid(),1,1);

        if(fork()==0){ // 2.1
            registerProc(getpid(),getppid(),2,1);
            _exit(0);
        }
        while(wait(NULL)>0);
        _exit(0); 
    }

    if (fork() == 0) {// 1.2
        registerProc(getpid(),getppid(),1,2);

        if(fork()==0){ // 2.2
            registerProc(getpid(),getppid(),2,2);
            _exit(0);
        }
        while(wait(NULL)>0);
        _exit(0); 
    }

    if (fork() == 0) {// 1.3
        registerProc(getpid(),getppid(),1,3);

        if(fork()==0){ // 2.3
            registerProc(getpid(),getppid(),2,3);
            _exit(0); 
        }
        if(fork()==0){ // 2.4
            registerProc(getpid(),getppid(),2,4);
            _exit(0); 
        }
        if(fork()==0){ // 2.5
            registerProc(getpid(),getppid(),2,5);
            _exit(0); 
        }
        if(fork()==0){ // 2.6
            registerProc(getpid(),getppid(),2,6);
            _exit(0); 
        }
        while(wait(NULL)>0);
        _exit(0); 
    }
    while(wait(NULL)>0);
    printProcRegistrations();
    execl("/bin/ls", "ls","-l", NULL);
}

