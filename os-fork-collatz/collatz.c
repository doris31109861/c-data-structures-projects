/*
 * collatz.c — 作業系統：用 fork() 建立子行程計算 Collatz 數列
 *
 * 子行程（fork 回傳 0）從輸入的正整數 n 開始，偶數除以 2、奇數乘 3 加 1，印到 1 為止；
 * 父行程呼叫 wait() 等子行程結束，避免子行程變成殭屍行程。只能在 Linux / macOS 編譯。
 */
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

