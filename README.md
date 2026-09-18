# TCP/IP Server-Client 專案

這個專案已從 Visual Studio/MSBuild 專案轉成 Qt 5 + CMake 設定，可在 Windows Terminal 與 Linux 環境中使用相同原始碼進行編譯。

## 專案結構

```
idle/
├── CMakeLists.txt              # 根專案設定
├── CMakePresets.json           # Windows / Linux 編譯預設
├── build_qt.ps1                # Windows 編譯腳本
├── README.md                   # 專案說明
├── client/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── client.h/cpp
│   ├── clientwindow.h/cpp/ui
│   └── ...
├── server/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── server.h/cpp
│   ├── serverwindow.h/cpp/ui
│   └── ...
└── Qt5Settings.props           # Legacy VC 設定（保留僅作參考，不再為主要建置流程）
```

## 功能

- **Server**: 多客戶端連線、訊息廣播、連線監控
- **Client**: 連線 Server、收發訊息

## 環境需求

- CMake 3.16+
- Qt 5.x (推薦 5.15.2)
- Windows: Visual Studio 2022 + Qt 5.15.2 msvc2019_64
- Linux: Qt 5 開發套件（例如 `qtbase5-dev`、`qttools5-dev`）

## Windows 編譯

已驗證可用，使用以下命令即可在 Windows Terminal 編譯：

### 方式 1：CMake 預設

```powershell
cmake --preset windows-qt5
cmake --build --preset windows-qt5 --config Release
```

### 方式 2：手動建置（已驗證）

```powershell
cmake -S . -B build/windows -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="D:/Qt/5.15.2/msvc2019_64"
cmake --build build/windows --config Release --parallel
```

### 方式 3：使用腳本

```powershell
./build_qt.ps1
```

編譯完成後，執行檔會在以下位置：

- Server: `build/windows/server/Release/idle_server.exe`
- Client: `build/windows/client/Release/idle_client.exe`

## Linux 編譯

```bash
cmake --preset linux-qt5
cmake --build --preset linux-qt5
```

若系統 Qt 5 安裝路徑不同，可改用：

```bash
cmake -S . -B build/linux -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt5;/usr/lib/x86_64-linux-gnu"
cmake --build build/linux
```

Linux 編譯完成後，執行檔通常會在：

- `build/linux/server/idle_server`
- `build/linux/client/idle_client`

## 執行

- 啟動 `idle_server`
- 啟動 `idle_client`
- Server 設定埠號（預設 8888）
- Client 輸入主機 IP 和埠號進行連線

## 技術說明

- **Server**: `QTcpServer` 多客戶端管理
- **Client**: `QTcpSocket` 連線通訊
- **MOC**: 自動處理 Qt signals/slots
- **UIC**: 自動生成 UI 類別
- **Build Tool**: CMake + Qt 5，脫離 Visual Studio 專案檔依賴

## 常見問題

**Q: Windows 架構不匹配怎麼辦？**  
A: 使用 `msvc2019_64` 這個 Qt 路徑，避免 32-bit/64-bit 混用。

**Q: Linux 找不到 Qt5？**  
A: 確認已安裝 `qtbase5-dev` 等套件，並設定正確的 `CMAKE_PREFIX_PATH`。
