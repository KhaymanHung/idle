package com.idle.chat;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

public class ServerConfig {
    private static final String CONFIG_FILE_NAME = "config.ini";
    private static final Map<String, String> values = new HashMap<>();

    static {
        load();
    }

    private ServerConfig() {
    }

    public static void load() {
        values.clear();

        Path configPath = Paths.get(CONFIG_FILE_NAME);
        if (!Files.exists(configPath)) {
            setDefaultValues();
            return;
        }

        try (BufferedReader reader = Files.newBufferedReader(configPath, StandardCharsets.UTF_8)) {
            String line;
            while ((line = reader.readLine()) != null) {
                String trimmed = line.trim();
                if (trimmed.isEmpty() || trimmed.startsWith("#") || trimmed.startsWith(";")) {
                    continue;
                }

                int equalsIndex = trimmed.indexOf('=');
                if (equalsIndex <= 0) {
                    continue;
                }

                String key = trimmed.substring(0, equalsIndex).trim();
                String value = trimmed.substring(equalsIndex + 1).trim();
                values.put(key.toLowerCase(), value);
            }
        } catch (IOException e) {
            System.err.println("Failed to read " + CONFIG_FILE_NAME + ": " + e.getMessage());
            setDefaultValues();
        }
    }

    private static void setDefaultValues() {
        values.put("servername", "IdleServer");
        values.put("serverport", "9000");
        values.put("dbip", "127.0.0.1");
        values.put("dbport", "3306");
        values.put("dbusername", "root");
        values.put("dbpassword", "123456");
        values.put("clientmaxconnects", "10");
    }

    public static String getString(String key, String defaultValue) {
        return values.getOrDefault(key.toLowerCase(), defaultValue);
    }

    public static int getInt(String key, int defaultValue) {
        String value = values.get(key.toLowerCase());
        if (value == null) {
            return defaultValue;
        }
        try {
            return Integer.parseInt(value.trim());
        } catch (NumberFormatException e) {
            return defaultValue;
        }
    }

    public static String getServerName() {
        return getString("servername", "IdleServer");
    }

    public static int getServerPort() {
        return getInt("serverport", 9000);
    }

    public static String getDbIp() {
        return getString("dbip", "127.0.0.1");
    }

    public static int getDbPort() {
        return getInt("dbport", 3306);
    }

    public static String getDbUsername() {
        return getString("dbusername", "root");
    }

    public static String getDbPassword() {
        return getString("dbpassword", "123456");
    }

    public static int getClientMaxConnects() {
        return getInt("clientmaxconnects", 10);
    }
}
