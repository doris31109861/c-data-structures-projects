# Changelog

## 2026-10-09 — 修正編譯錯誤與 README 說明

- **內容**：`dino.c` 的 `void` 函式中 `return 0` 改為 `return`、`sleep(1)` 改為 Windows API `Sleep(1000)`；`collatz.c` 的 `printf(stderr, ...)` 改為 `fprintf(stderr, ...)`；README 更正 `tree.c` 的說明（序列寫在程式中，並非讀取 Input.txt）。
- **原因**：新版 GCC（14）把這兩種寫法視為錯誤，`dino.c` 原本無法編譯；`printf(stderr, ...)` 會把 FILE 指標當成格式字串，是錯誤用法。
- **測試**：`dino.c` 以 gcc 14（Windows）語法檢查由 2 個錯誤變為 0；`collatz.c` 需 Linux，本機未編譯。

## 2026-10-09 — 補上檔案開頭說明註解

- **內容**：`tree.c`、`collatz.c`、`bank.c`、`sell.c`、`dino.c` 開頭各加 3–5 行說明：用途、使用的資料結構／系統呼叫、輸入輸出。
- **原因**：讓第一次看程式的人快速知道每個檔案在做什麼。
- **測試**：只有加註解。`tree.c` 以 gcc 編譯執行，輸出 `CBEHGIFDA` 與註解中的範例一致；`bank.c`、`sell.c`、`dino.c` 以 gcc（Windows）語法檢查通過；`collatz.c` 需 Linux（`sys/wait.h`），本機未編譯。
