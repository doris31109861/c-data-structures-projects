# Changelog

## 2026-10-09 — 修正恐龍 ASCII 圖案的跳脫字元、CI 測試步驟

- **內容**：`dino.c` 恐龍圖案中的 `"\)"` 改為 `"\\)"`（原本是無效的跳脫字元，反斜線印不出來，gcc 會警告）；`console.h` 的 Linux 分支補上 `<stdlib.h>`（`rand` 原本由 `windows.h` 間接引入）；CI 的 smoke run 改成 `|| code=$?`，因為 GitHub Actions 的 bash 有 `-e`，逾時的 124 會直接讓步驟失敗。
- **說明**：前一個 commit（01085fe）的訊息已提到圖案修正，但當時修改腳本中途出錯，實際只包含 CI 與 `<stdlib.h>` 的修改；圖案修正在這個 commit。
- **測試**：Windows gcc `-Wall` 編譯無警告；CI 在 Linux 全部步驟通過（含恐龍 3 秒 smoke run）。

## 2026-10-09 — 恐龍遊戲支援 Linux／macOS

- **內容**：新增 `terminal-dino-game/console.h`：Windows 維持 `<windows.h>`／`<conio.h>`；Linux／macOS 以 termios 實作 `kbhit`／`getch`（不等 Enter、不回顯），`Sleep` 對應 `usleep`，清畫面改用 ANSI 跳脫碼。`dino.c` 改 include `console.h`，`system("cls")` 改為 `clear_screen()`，遊戲邏輯不變。CI 在 Linux 編譯並執行 3 秒確認進入遊戲迴圈。
- **原因**：原本只能在 Windows 執行。
- **測試**：Windows gcc 編譯通過；Linux 編譯與執行由 CI 驗證。實際鍵盤遊玩未在 Linux 測試。

## 2026-10-09 — sell.c：跨平台、釋放記憶體、修正刪除節點錯誤

- **內容**：
  - 以自己實作的 `str_icmp()` 取代 Windows 專用的 `strcmpi`，Linux 也能編譯；`search()` 原本用區分大小寫的 `strcmp`，與 BST 建樹用的順序不一致，一併改成 `str_icmp`。
  - 商品賣完被刪除時 `free` 節點與賣家陣列；程式結束前以 `free_tree()` 後序釋放整棵樹並 `fclose` 所有檔案。
  - 修正刪除「有兩個子節點」的商品時的錯誤：原本找前驅節點的迴圈寫成 `(*temp) = (*temp)->rchild`，會改掉樹上的指標、讓中間的節點遺失，且前驅的左子樹被直接設成 NULL；改成移動指標 `temp = &(*temp)->rchild` 並把前驅的左子樹接回。
  - 輸入檔名緩衝區由 10 格放大到 256 並限制 `scanf` 長度，避免溢位。
  - 新增 GitHub Actions：在 Linux 編譯 4 個作業並執行，`sell` 以 Valgrind 檢查記憶體洩漏，另加入刪除節點的回歸測試 `tests/sell_delete_two_children.txt`。
- **原因**：原程式在 Linux 編不過、沒有任何 `free`，而且刪除邏輯有會遺失資料的錯誤。
- **測試**：Windows gcc 編譯後以 `input.txt` 執行，5 個輸出檔與修改前完全相同；自製測資（插入 M D X F G 後買走 M）修改前排序只剩 `G X`，修改後為 `D F G X`；Linux 上 CI 編譯執行通過，Valgrind：All heap blocks were freed — no leaks are possible、0 errors。

## 2026-10-09 — 修正編譯錯誤與 README 說明

- **內容**：`dino.c` 的 `void` 函式中 `return 0` 改為 `return`、`sleep(1)` 改為 Windows API `Sleep(1000)`；`collatz.c` 的 `printf(stderr, ...)` 改為 `fprintf(stderr, ...)`；README 更正 `tree.c` 的說明（序列寫在程式中，並非讀取 Input.txt）。
- **原因**：新版 GCC（14）把這兩種寫法視為錯誤，`dino.c` 原本無法編譯；`printf(stderr, ...)` 會把 FILE 指標當成格式字串，是錯誤用法。
- **測試**：`dino.c` 以 gcc 14（Windows）語法檢查由 2 個錯誤變為 0；`collatz.c` 需 Linux，本機未編譯。

## 2026-10-09 — 補上檔案開頭說明註解

- **內容**：`tree.c`、`collatz.c`、`bank.c`、`sell.c`、`dino.c` 開頭各加 3–5 行說明：用途、使用的資料結構／系統呼叫、輸入輸出。
- **原因**：讓第一次看程式的人快速知道每個檔案在做什麼。
- **測試**：只有加註解。`tree.c` 以 gcc 編譯執行，輸出 `CBEHGIFDA` 與註解中的範例一致；`bank.c`、`sell.c`、`dino.c` 以 gcc（Windows）語法檢查通過；`collatz.c` 需 Linux（`sys/wait.h`），本機未編譯。
