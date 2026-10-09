# C Data Structures & Systems Projects

> 資料結構、程式設計、作業系統課程作業｜逢甲大學資訊工程學系｜C

[中文](#中文) | [English](#english)

---

## 中文

用 C 語言把資料結構與作業系統觀念實際做出來的作品集。

| 專案 | 觀念 | 說明 |
|---|---|---|
| [`marketplace-bst/`](marketplace-bst) | 二元搜尋樹、Heap Sort、檔案 I/O | 拍賣平台：每項商品是 BST 的一個節點並記錄所有賣家；支援新增、查詢、購買（用 Heap 取最低價）、刪除與排序報表，輸出 Buy／Search／Sort／Log 表 |
| [`bank-teller-simulation/`](bank-teller-simulation) | Linked-list 佇列、事件模擬 | 模擬多個櫃台的銀行，客人選最短的隊伍排隊，輸出每位客人的櫃台與離開時間 |
| [`tree-traversal/`](tree-traversal) | 二元樹、遞迴 | 由 PreOrder 與 InOrder 重建二元樹並輸出 PostOrder |
| [`terminal-dino-game/`](terminal-dino-game) | 遊戲迴圈、陣列 | 終端機版 Chrome 小恐龍：3 層樓、障礙物、隨分數加速、最高分紀錄、「蟲洞」技能 |
| [`os-fork-collatz/`](os-fork-collatz) | `fork()`、`wait()` | 子行程印出 Collatz 數列，父行程等待子行程結束 |

### 編譯與執行

```bash
cd marketplace-bst && gcc sell.c -o sell && ./sell          # 輸入 input.txt
cd bank-teller-simulation && gcc bank.c -o bank && ./bank   # 輸入 input1.tst 或 input2.tst
cd tree-traversal && gcc tree.c -o tree && ./tree           # 範例序列寫在程式中（Input.txt 為題目提供的另一組輸入）
cd os-fork-collatz && gcc collatz.c -o collatz && ./collatz # Linux
```

> `terminal-dino-game` 透過 `console.h` 相容層，在 Windows 使用原本的 `<windows.h>`／`<conio.h>`，在 Linux／macOS 改用 termios 與 ANSI 跳脫碼，兩邊都能玩：`gcc dino.c -o dino && ./dino`。

所有作業都可在 Linux / Windows 編譯；每次 push 會由 GitHub Actions 在 Linux 上編譯執行，`sell` 另以 Valgrind 檢查記憶體洩漏（`.github/workflows/build.yml`）。

### 學到的東西

- 依需求選擇資料結構（BST 查詢、Heap 排序、佇列模擬）
- 在 C 中用指標與動態記憶體管理串列結構
- Linux 上的行程建立與同步

---

## English

A collection of C programs that put core data structures and OS concepts into practice.

| Project | Concepts | Description |
|---|---|---|
| `marketplace-bst/` | BST, heap sort, file I/O | A marketplace where each product is a BST node holding its sellers; supports insert, search, buy (lowest price via heap), delete and sorted reports |
| `bank-teller-simulation/` | Linked-list queues, simulation | Customers join the shortest of several teller queues; outputs each customer's window and departure time |
| `tree-traversal/` | Binary tree, recursion | Rebuilds a tree from PreOrder + InOrder and prints PostOrder |
| `terminal-dino-game/` | Game loop, arrays | Console Dino game with 3 floors, obstacles, speed-up, high score and a "wormhole" skill (Windows, Linux and macOS via `console.h`) |
| `os-fork-collatz/` | `fork()`, `wait()` | A child process prints the Collatz sequence while the parent waits |

### What I learned

- Choosing a data structure for the workload (BST for lookup, heap for ordering, queues for simulation)
- Dynamic memory management with pointers and linked structures in C
- Process creation and synchronization on Linux
