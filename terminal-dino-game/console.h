/*
 * console.h — 讓恐龍遊戲同時能在 Windows 與 Linux / macOS 終端機執行的相容層
 *
 * 遊戲只用到 4 個與平台有關的功能：暫停 Sleep、清除畫面、kbhit（有沒有按鍵）、getch（讀一個鍵）。
 *   - Windows：直接使用 <windows.h> 與 <conio.h> 的原生函式（原本的寫法）
 *   - Linux / macOS：用 termios 把終端機切成「不等 Enter、不回顯」模式來實作 kbhit / getch，
 *                    Sleep 用 usleep，清除畫面用 ANSI 跳脫碼
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef _WIN32

#include <windows.h>
#include <conio.h>
#include <stdlib.h>

static inline void clear_screen(void) { system("cls"); }

#else /* Linux / macOS */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/* Windows 的 Sleep 以毫秒為單位，usleep 以微秒為單位 */
#define Sleep(ms) usleep((useconds_t)(ms) * 1000)

/* ANSI：游標移到左上角並清除整個畫面 */
static inline void clear_screen(void) {
    printf("\033[H\033[2J");
    fflush(stdout);
}

/* 讀一個鍵，不需要按 Enter、也不顯示在畫面上 */
static inline int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);          /* 關閉行緩衝與回顯 */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);   /* 還原終端機設定 */
    return ch;
}

/* 檢查是否有按鍵（不會卡住）：暫時切成非阻塞模式偷看一個字元，有的話再放回去 */
static inline int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);   /* 放回去，讓接下來的 getch 讀得到 */
        return 1;
    }
    clearerr(stdin);         /* 非阻塞讀不到時 stdin 會被標記錯誤，要清掉 */
    return 0;
}

#endif /* _WIN32 */

#endif /* CONSOLE_H */
