#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 1024
void do_command(char *input)
{
    char *arg[10];
    int i = 0;
    arg[i] = strtok(input, " ");
    while( arg[i] != NULL)
    {
        arg[++i] = strtok(NULL," ");
    }
    if(strcmp(arg[0], "cd"))
    {
        if(arg[1] == NULL || strcmp(arg[1], ".") )
        {
            printf("Asd\n");
        }
    }
    else if (strcmp(arg[0], "pwd"))
    {

    }
    else
    {

    }
}
int main() {
    
    while (1) 
    {
        printf("%s@%s:%s$ ",getenv("USER"),getenv("HOSTNAME"),getenv("PWD"));
        char input[MAX];
        fgets(input, MAX, stdin);
        do_command(input);
        break;
    }

}