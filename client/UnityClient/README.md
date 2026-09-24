# Unity Client

這個資料夾是 Unity 端專用範例。

## 資料結構

```text
UnityClient/
└── Assets/
    └── Scripts/
        └── ChatClient.cs
```

## 使用方式

1. 在 Unity 中建立新專案，或直接把 `Assets` 匯入。
2. 把 `ChatClient.cs` 附加到任一個 GameObject。
3. 在 Inspector 設定 `host`、`port`。
4. 執行後，腳本會自動連接到 Java Server。

## 範例

```csharp
host = "127.0.0.1";
port = 9000;
```
