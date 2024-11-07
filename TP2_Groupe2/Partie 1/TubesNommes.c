#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    //mkfifo("tube", 0600);  // Create a named pipe (FIFO)
    //mkfifo("tube2", 0600);  // Create a second named pipe (FIFO)

    //if (fork() == 0) { // First child (rev < In.txt)
        //int fd = open("tube", O_WRONLY);  // Open the pipe for writing
        //int file = open("In.txt", O_RDONLY);  // Open In.txt for reading
        //dup2(file, 0);  // Redirect stdin from In.txt
        //dup2(fd, 1);    // Redirect stdout to the pipe
        //close(fd);
        //close(file);
        //execl("/bin/rev", "rev", NULL);  // Execute the rev command
        //_exit(1);  // In case execl fails
    //}

   // if (fork() == 0) { // Second child (rev)
        //int fd = open("tube", O_RDONLY);  // Open the pipe for reading
        //int fd2 = open("tube2", O_WRONLY);  // Open the second pipe for writing
        //dup2(fd, 0);  // Redirect stdin to read from the pipe
       // dup2(fd2, 1);    // Redirect stdout to the second pipe
       // close(fd);
        //close(fd2);
        //execl("/bin/rev", "rev", NULL);  // Execute the rev command again
       // _exit(1);  // In case execl fails
    //}

    //if (fork() == 0) { // Third child (diff - In.txt -s)
        //int fd2 = open("tube2", O_RDONLY);  // Open the second pipe for reading the final output
        //dup2(fd2, 0);  // Redirect stdin to read from the second pipe
        //close(fd2);
        //execl("/usr/bin/diff", "diff", "-", "In.txt", "-s", NULL);  // Compare output with In.txt
       // _exit(1);  // In case execl fails
   // }

   // int pid;
   // while ((pid = wait(NULL)) > 0) {}

   // return 0;
}
