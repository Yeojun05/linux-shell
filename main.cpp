#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 1024

typedef struct {
    char *token;
    int index;
} TokenInfo;

TokenInfo tokens[100];

int split(char *input)
{

    int tcnt = 0;
    const char *delimiters[] = {"||", "&&", ";"};
    int delimiter_count = 3;

    const char *base = input;     // 원본 
    const char *start = input;    // 미는거거 

    while (*start) {
        const char *earliest = NULL;
        int earl_index = -1;

        // 모든 구분자에 대해 찾고, 가장 빨리 등장하는 구분자를 선택   -> 가장 먼저 나오는거 (나오면 저장하고 자르고 while문 진행방식식)
        for (int i = 0; i < delimiter_count; i++) {
            const char *pos = strstr(start, delimiters[i]);
            if (pos != NULL) {
                if (earliest == NULL || pos < earliest) {
                    earliest = pos;
                    earl_index = i;
                }
            }
        }

        if (earliest == NULL) {  
            // 더 이상 구분자가 없으면 나머지 문자열을 토큰으로 저장
            if (*start) {
                tokens[tcnt].token = strdup(start);
                tokens[tcnt].index = start - base;
                tcnt++;
            }
            break;  //while 탈출
        } else {
            // 구분자 앞에 있는 문자열 (토큰)을 저장 (빈 문자열이 아닐 때)
            if (earliest > start) {
                tokens[tcnt].token = strndup(start, earliest - start);
                tokens[tcnt].index = start - base;
                tcnt++;
            }
            // 구분자 -> 토큰으로 저장
            tokens[tcnt].token = strdup(delimiters[earl_index]);
            tokens[tcnt].index = earliest - base;
            tcnt++;
            
            // start를 구분자 바로 뒤로 이동   -> start 밀기
            start = earliest + strlen(delimiters[earl_index]); 
        }
    }

    printf(" 분리된 문자열 및 인덱스:\n");
    for (int i = 0; i < tcnt; i++) {
        printf("[%d] %s\n", tokens[i].index, tokens[i].token);
        free(tokens[i].token);
    }
    printf(" %d\n",tcnt);

    return (tcnt+1)/2;
}
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

        int input_num = split(input);

        for (int i = 0; i < input_num; i++)
        {
            
        }
        //do_command(input);
    }

}