// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char final_str[] = "gobison.org";
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%99s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        close(fd1[1]);

        char received_str[300];
        read(fd2[0], received_str, sizeof(received_str));
        close(fd2[0]);

        strcat(received_str, final_str);

        printf("Output : %s\n", received_str);
        
  
        // Wait for child to print the concatenated string 
        wait(NULL); 
       
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        // Read a string using first pipe 
        char concat_str[200]; 
        read(fd1[0], concat_str, sizeof(concat_str));
        close(fd1[0]); 

        strcat(concat_str, fixed_str);
  
        // Concatenate a fixed string with it 
          // string ends with '\0' 
  

        char second_input[100];

        printf("Input : ");
        scanf("%99s", second_input);

        strcat(concat_str, second_input);

        write(fd2[1], concat_str, strlen(concat_str) +1);
        
        // Close both reading ends 
        close(fd2[1]); 

  
        exit(0); 
    } 
} 