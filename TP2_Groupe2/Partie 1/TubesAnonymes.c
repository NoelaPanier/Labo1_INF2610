#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    // Creation des tubes anonymes
    int fd1[2];
    int fd2[2];
    pipe(fd1);  
    pipe(fd2); 

    if(fork() == 0){
        // Fermer les fd non utilises
        close(fd2[0]); 
        close(fd2[1]);
        close(fd1[0]);
       
        int file = open("In.txt", O_RDONLY);
      
        // Redirection stdin/stdout
        dup2(file, 0);  
        dup2(fd1[1], 1); 

        close(fd1[1]);
        close(file);

        execlp("rev", "rev", NULL);  
    }

  
    if(fork() == 0) {
        // Fermer les fd non utilises
        close(fd1[1]); 
        close(fd2[0]);  

        // Redirection stdin/stdout
        dup2(fd1[0], 0); 
        dup2(fd2[1], 1); 

        close(fd1[0]);
        close(fd2[1]);

        execlp("rev", "rev", NULL);  
    }

  
    if(fork() == 0){
        // Fermer les fd non utilises
        close(fd1[0]); 
        close(fd1[1]);
        close(fd2[1]); 

        // Redirection stdin/sdout 
        dup2(fd2[0], 0);

        close(fd2[0]);

        execlp("diff", "diff", "-", "In.txt", "-s", NULL);  
    }

    // Fermer tous les fd
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    // Attendre la fin de tous les processus fils
    while (wait(NULL) > 0);

    return 0;
}
