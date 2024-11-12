#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    // Creation des tubes nommes
    mkfifo("tube", 0600);  
    mkfifo("tube2", 0600);  

    if (fork() == 0) { 
        int fd = open("tube", O_WRONLY);  
        int file = open("In.txt", O_RDONLY); 

        // Redirection stdin/stdout
        dup2(file, 0);  
        dup2(fd, 1);  

        close(fd);
        close(file);

        execl("/bin/rev", "rev", NULL); 
    }

    if (fork() == 0) { 
        int fd = open("tube", O_RDONLY);  
        int fd2 = open("tube2", O_WRONLY);  

        // Redirection stdin/stdout
        dup2(fd, 0); 
        dup2(fd2, 1);  

        close(fd);
        close(fd2);

        execl("/bin/rev", "rev", NULL);  
    }

    if (fork() == 0) {
        int fd2 = open("tube2", O_RDONLY); 

        // Redirection stdin 
        dup2(fd2, 0);  

        close(fd2);

        execl("/usr/bin/diff", "diff", "-", "In.txt", "-s", NULL);  
    }

    int pid;
    while ((pid = wait(NULL)) > 0) {}

    return 0;
}
