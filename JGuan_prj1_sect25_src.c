//Filename: JGuan_prj1_sect25_src.c
//Jiaheng Guan
//Compile command:gcc JGuan_prj1_sect25_src.c -o JGuan_prj1_sect25_src.exe

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define MAXLINE 128

int main (int argc, const char *argv[]){

    int f1, f2;                     
    int n1, n2;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    char buf[MAXLINE];

//the pipe is created and its file descriptor returned 

if (argc != 3){                //checking if the numbers of the arguments are correct; if not, return 1.
printf("Wrong number of command line arguments\n");
// exit(1);
return 1;
}

if ((f1 = open(argv[1], O_RDONLY, 0)) == -1){             //checking if we can open the source file; if not, return 2.
printf("Can't open %s \n", argv[1]);
return 2;
}

if ((f2 = creat(argv[2], 0644)) == -1){                  //checking if we can create the output file for the copy contents; if not, return 3.
printf("Can't create %s \n", argv[2]);
return 3;
}

if (pipe(fd) < 0){                                      //checking if we can create the pipe for the parent; if not, return 1.
    fprintf (stderr, "pipe failed");
    return 1;
 }
 
 //parent is producer and child is consumer
 pid = fork();
 
 if (pid < 0){                                          //checking if we can call fork(); if not, return 1;
     fprintf (stderr, "Fork failed");
     return 1;
 }
 else if (pid == 0 ){         //child reads from the pipe 
     close (fd[1]);           //write end of pipe
     n1 = read (fd[0] , line , MAXLINE);          //read the contents and store it into n1
     write (f2, line, n1);                        //write the contents to the output file, which is creating from the previous part
     write (STDOUT_FILENO, line, n1);             //print out the contents at the end
 }
else{
 //parents write to pipe 
     close (fd[0]);  //read end of pipe
     while ((n2 = read(f1, buf, MAXLINE)) > 0){      //read the contents from the input file
         if (write(fd[1], buf, n2) != n2){           //try to write the contents to the pipe; if can not do it, return 4.
             printf("Can't write file" );
             close(f1);
             close(fd[1]);
             return 4;
         }
    }
}

close(f1);
close(f2);


return 0;
}