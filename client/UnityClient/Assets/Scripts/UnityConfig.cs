using System;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public static class UnityConfig
{
    private static readonly Dictionary<string, string> values = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

    static UnityConfig()
    {
        Load();
    }

    public static void Load()
    {
        values.Clear();

        string[] candidatePaths =
        {
            Path.Combine(Application.dataPath, "Config", "config.ini"),
            Path.Combine(Application.dataPath, "config.ini"),
            Path.Combine(Application.streamingAssetsPath, "config.ini")
        };

        foreach (string path in candidatePaths)
        {
            if (File.Exists(path))
            {
                ReadFile(path);
                return;
            }
        }

        values["serverip"] = "127.0.0.1";
        values["serverport"] = "9000";
    }

    private static void ReadFile(string path)
    {
        try
        {
            string[] lines = File.ReadAllLines(path);
            foreach (string rawLine in lines)
            {
                string line = rawLine.Trim();
                if (string.IsNullOrEmpty(line) || line.StartsWith("#") || line.StartsWith(";"))
                {
                    continue;
                }

                int equalsIndex = line.IndexOf('=');
                if (equalsIndex <= 0)
                {
                    continue;
                }

                string key = line.Substring(0, equalsIndex).Trim();
                string value = line.Substring(equalsIndex + 1).Trim();
                values[key] = value;
            }
        }
        catch (Exception ex)
        {
            Debug.LogWarning("Failed to read config.ini: " + ex.Message);
            values["serverip"] = "127.0.0.1";
            values["serverport"] = "9000";
        }
    }

    public static string GetString(string key, string defaultValue)
    {
        if (values.TryGetValue(key, out string value))
        {
            return value;
        }

        return defaultValue;
    }

    public static int GetInt(string key, int defaultValue)
    {
        if (values.TryGetValue(key, out string value) && int.TryParse(value, out int parsed))
        {
            return parsed;
        }

        return defaultValue;
    }

    public static string GetServerIp()
    {
        return GetString("serverip", "127.0.0.1");
    }

    public static int GetServerPort()
    {
        return GetInt("serverport", 9000);
    }
}
