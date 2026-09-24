# TCP 通訊驗證專案

這個專案的目標不是聊天室，也不做任何 UI。

目標是驗證：

- 單一 Java Server 可以接收多個 Client 連線
- 多個 Unity Client 可以透過 TCP 傳送與接收訊息
- Server 直接輸出到終端機，方便觀察連線與資料流
- 不做 2、3 這種聊天室擴充功能，只保留核心通訊驗證

## 專案結構

```text
idle/
├── README.md
├── server/
│   ├── pom.xml
│   ├── start-server.bat
│   └── src/
│       └── main/java/com/idle/chat/
│           ├── Server.java
│           └── ClientHandler.java
├── client/
│   └── UnityClient/
│       ├── README.md
│       └── Assets/
│           └── Scripts/
│               └── ChatClient.cs
├── scripts/
│   └── README.md
└── .gitignore
```

## 伺服器端

- 語言：Java
- 通訊方式：TCP Socket
- 執行模式：終端機直接顯示
- 功能：接受多個 client、顯示連線狀態、轉送訊息

這個環境已確認 `java` 和 `javac` 可直接使用，因此不需要 Maven。

### 正常執行

```powershell
cd server
$files = Get-ChildItem -Path .\src\main\java\com\idle\chat -Filter *.java | Select-Object -ExpandProperty FullName
javac -d out $files
java -cp out com.idle.chat.Server
```

也可直接執行：

```powershell
cd server
.\start-server.bat
```

### 背景執行

如果你想讓伺服器在背景執行，不要佔住目前的終端機視窗，可使用：

```powershell
cd server
Start-Process powershell -ArgumentList "-NoExit","-Command","cd '$PWD'; java -cp out com.idle.chat.Server"
```

或直接在同一個 PowerShell 內啟動另一個背景工作：

```powershell
cd server
Start-Job -ScriptBlock { Set-Location "D:\work\idle\server"; java -cp out com.idle.chat.Server }
```

如果你想在背景執行時保留輸出日誌，可以這樣做：

```powershell
cd server
$log = "server.log"
Start-Process powershell -ArgumentList "-NoExit","-Command","cd '$PWD'; java -cp out com.idle.chat.Server 2>&1 | Tee-Object -FilePath '$log'"
```

### 背景執行時停止程式

背景執行測試完成後，請務必關閉伺服器，避免 port 9000 仍被佔用，影響後續測試或重新啟動。

最簡單方式：

```powershell
cd server
.\stop-server.bat
```

如果要手動確認，再用：

```powershell
netstat -ano | findstr :9000
```

找到 PID 後停止：

```powershell
taskkill /PID <PID> /F
```

例如：

```powershell
taskkill /PID 12345 /F
```

如果你想先確認目前是否有 Java Server 在跑：

```powershell
Get-CimInstance Win32_Process | Where-Object { $_.Name -match "java" }
```

預設 Port：9000

## Unity Client

- 語言：C#
- 目標：用來連線 Java Server
- 角色：簡單的測試端，不做 UI

使用方式：

1. 打開 Unity
2. 將 `client/UnityClient` 匯入專案
3. 把 `Assets/Scripts/ChatClient.cs` 掛到任意 GameObject
4. 設定 `host` 和 `port`
5. 執行後即可連接到 Java Server
6. 使用 `SendMessage("test")` 送出測試訊息

## 驗證重點

這個專案的核心驗證目標是：

- 1 個 server + 多個 client 同時連線
- 訊息可由 client 傳到 server
- server 可在終端機顯示收到的訊息
- server 可廣播給其他連線中的 client
- 不需要額外聊天室功能、會員系統、UI、房間管理

## 連線範例

```text
Server: 127.0.0.1:9000
```

Client 端可直接設定為：

```csharp
host = "127.0.0.1";
port = 9000;
```

## 備註

這是最小化的單 server 多 client 通訊驗證專案，目的是驗證 TCP 架構與多連線行為，而不是實作聊天介面。