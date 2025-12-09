# TCP/IP Server-Client 專案

Qt5 + VC++ TCP/IP 通訊專案，含圖形化 Server 和 Client 應用程式。

## 專案結構

```
idle/
├── idle.sln                    # VS 方案檔
├── Qt5Settings.props           # Qt 路徑配置
├── server/                     # Server 專案
│   ├── Server.vcxproj
│   ├── main.cpp
│   ├── server.h/cpp            # TCP Server 邏輯
│   └── serverwindow.h/cpp/ui   # GUI 介面
└── client/                     # Client 專案
    ├── Client.vcxproj
    ├── main.cpp
    ├── client.h/cpp            # TCP Client 邏輯
    └── clientwindow.h/cpp/ui   # GUI 介面
```

## 功能

- **Server**: 多客戶端連線、訊息廣播、連線監控
- **Client**: 連線 Server、收發訊息

## 環境需求

- Visual Studio 2022 (MSVC v143)
- Qt 5.15.2 MSVC 2019 32-bit: `D:\Qt\5.15.2\msvc2019`

## 快速開始

## 快速開始

### 編譯與執行

1. 開啟 `idle.sln`
2. 選擇 **Win32** 平台（**Debug** 或 **Release**）
3. 按 `F5` 編譯並執行
4. Qt DLL 自動複製到 `bin\Debug\` 或 `bin\Release\`

### 使用

**Server**: 設定埠號（預設 8888）→ 啟動伺服器  
**Client**: 輸入主機 IP 和埠號 → 連線

## 修改介面

使用 Qt Designer：`D:\Qt\5.15.2\msvc2019\bin\designer.exe`  
編輯 `.ui` 檔案後重新編譯。

## 技術說明

- **Server**: `QTcpServer` 多客戶端管理
- **Client**: `QTcpSocket` 連線通訊
- **MOC**: 自動處理 Qt signals/slots
- **UIC**: 自動生成 UI 類別
- **Post-Build**: 自動複製必要 Qt DLL（僅 Release 版本）

## 常見問題

**Q: Qt 路徑不同怎麼辦？**  
A: 編輯 `Qt5Settings.props` 修改 `<QtDir>` 路徑。

**Q: 找不到 Qt5Core.dll？**  
A: 重新編譯，Post-Build 會自動複製 DLL。
