package com.idle.chat;

import java.nio.charset.StandardCharsets;
import java.util.Base64;

public class SecureComms {
    private static final int KEY_LENGTH = 512;
    private static final byte[] ENCRYPT_KEY = createKey("IDLE_ENCRYPT_KEY_FOR_TCP_COMMUNICATION_0");
    private static final byte[] DECRYPT_KEY = createKey("IDLE_DECRYPT_KEY_FOR_TCP_COMMUNICATION_1");
    private static final String ENCRYPTED_PREFIX = "ENC:";

    private SecureComms() {
    }

    private static byte[] createKey(String seed) {
        byte[] seedBytes = seed.getBytes(StandardCharsets.UTF_8);
        byte[] key = new byte[KEY_LENGTH];
        for (int i = 0; i < KEY_LENGTH; i++) {
            key[i] = seedBytes[i % seedBytes.length];
        }
        return key;
    }

    public static String encrypt(String plainText) {
        if (plainText == null) {
            return "";
        }

        byte[] textBytes = plainText.getBytes(StandardCharsets.UTF_8);
        byte[] encrypted = new byte[textBytes.length];

        for (int i = 0; i < textBytes.length; i++) {
            encrypted[i] = (byte) (textBytes[i] ^ ENCRYPT_KEY[i % ENCRYPT_KEY.length]);
        }

        return ENCRYPTED_PREFIX + Base64.getEncoder().encodeToString(encrypted);
    }

    public static String decrypt(String cipherText) {
        if (cipherText == null || cipherText.isEmpty()) {
            return "";
        }

        String normalized = cipherText.trim();
        if (normalized.startsWith(ENCRYPTED_PREFIX)) {
            normalized = normalized.substring(ENCRYPTED_PREFIX.length());
        }

        try {
            byte[] encrypted = Base64.getDecoder().decode(normalized);
            byte[] textBytes = new byte[encrypted.length];

            for (int i = 0; i < encrypted.length; i++) {
                textBytes[i] = (byte) (encrypted[i] ^ DECRYPT_KEY[i % DECRYPT_KEY.length]);
            }

            return new String(textBytes, StandardCharsets.UTF_8);
        } catch (IllegalArgumentException e) {
            return cipherText;
        }
    }

    public static byte[] getEncryptKeyBytes() {
        byte[] copy = new byte[ENCRYPT_KEY.length];
        System.arraycopy(ENCRYPT_KEY, 0, copy, 0, ENCRYPT_KEY.length);
        return copy;
    }

    public static byte[] getDecryptKeyBytes() {
        byte[] copy = new byte[DECRYPT_KEY.length];
        System.arraycopy(DECRYPT_KEY, 0, copy, 0, DECRYPT_KEY.length);
        return copy;
    }
}
