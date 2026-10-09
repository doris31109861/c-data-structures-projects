#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num = 0;
    printf("輸入一個正整數: ");
    scanf("%d",&num);
    if (num <= 0) {
        printf("輸入一個正整數");
        return 1;
    }
    pid_t pid = fork();
    /*
    parent -> PID
	child -> 0
	error -> -1
    */
    if (pid < 0) { //-1 -> error
        printf(stderr, "Fork 錯誤\n");
        return 1;
    } else if (pid == 0) { //0 -> in child
        printf("%d ", num );
        while (num  != 1) {
            if (num  % 2 == 0) {
                num  = num  / 2;
            } else {
                num  = 3 * num + 1;
            }
            printf("%d ", num );
        }
        printf("\n");
    } else { // in parent
        wait(NULL);
    }
    return 0;
}

