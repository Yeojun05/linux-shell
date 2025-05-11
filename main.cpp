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
    char dirc[MAX];

    arg[i] = strtok(input, " ");
    while( arg[i] != NULL)
    {
        arg[++i] = strtok(NULL," ");
    }

    if(strcmp(arg[0], "exit")==0)
    {
        exit(0);
    }


    if(strcmp(arg[0], "cd")==0)
    {
        if(arg[1] == NULL || strcmp(arg[1], "~") == 0)
        {
            arg[1] = getenv("HOME");
        }

        if(chdir(arg[1]) != 0) 
        {
            perror("디렉토리 변경 실패 ");
        }        
    }
    else if (strcmp(arg[0], "pwd")==0)
    {
        if (getcwd(dirc, sizeof(dirc)) != NULL) printf("%s\n",dirc);
        else perror("fail");
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0) {
            execvp(arg[0], arg);
            perror("명령 실행 실패");
            exit(1);
        } else {
            wait(NULL); // 부모
        }

    }
}
int main() {
    char dirc[MAX];
    while (1) 
    {
        //getcwd(dirc,sizeof(dirc));
        printf("%s@%s:%s$ ",getenv("USER"),getenv("HOSTNAME"), getcwd(dirc, sizeof(dirc)));

        char input[MAX];
        fgets(input, MAX, stdin);
        input[strcspn(input, "\n")] = 0; // 입력 해결

        do_command(input);
    }

}