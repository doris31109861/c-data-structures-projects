# Changelog

## 2026-10-09 — 補上檔案開頭說明註解

- **內容**：`tree.c`、`collatz.c`、`bank.c`、`sell.c`、`dino.c` 開頭各加 3–5 行說明：用途、使用的資料結構／系統呼叫、輸入輸出。
- **原因**：讓第一次看程式的人快速知道每個檔案在做什麼。
- **測試**：只有加註解。`tree.c` 以 gcc 編譯執行，輸出 `CBEHGIFDA` 與註解中的範例一致；`bank.c`、`sell.c`、`dino.c` 以 gcc（Windows）語法檢查通過；`collatz.c` 需 Linux（`sys/wait.h`），本機未編譯。
