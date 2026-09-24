using System;
using System.Text;

public static class SecureComms
{
    private const int KeyLength = 512;
    private const string EncryptedPrefix = "ENC:";
    private static readonly byte[] EncryptKey = CreateKey("IDLE_ENCRYPT_KEY_FOR_TCP_COMMUNICATION_0");
    private static readonly byte[] DecryptKey = CreateKey("IDLE_DECRYPT_KEY_FOR_TCP_COMMUNICATION_1");

    private static byte[] CreateKey(string seed)
    {
        byte[] seedBytes = Encoding.UTF8.GetBytes(seed);
        byte[] key = new byte[KeyLength];

        for (int i = 0; i < KeyLength; i++)
        {
            key[i] = seedBytes[i % seedBytes.Length];
        }

        return key;
    }

    public static string Encrypt(string plainText)
    {
        if (string.IsNullOrEmpty(plainText))
        {
            return string.Empty;
        }

        byte[] textBytes = Encoding.UTF8.GetBytes(plainText);
        byte[] encrypted = new byte[textBytes.Length];

        for (int i = 0; i < textBytes.Length; i++)
        {
            encrypted[i] = (byte)(textBytes[i] ^ EncryptKey[i % EncryptKey.Length]);
        }

        return EncryptedPrefix + Convert.ToBase64String(encrypted);
    }

    public static string Decrypt(string cipherText)
    {
        if (string.IsNullOrWhiteSpace(cipherText))
        {
            return string.Empty;
        }

        string normalized = cipherText.Trim();
        if (normalized.StartsWith(EncryptedPrefix, StringComparison.Ordinal))
        {
            normalized = normalized.Substring(EncryptedPrefix.Length);
        }

        try
        {
            byte[] encrypted = Convert.FromBase64String(normalized);
            byte[] textBytes = new byte[encrypted.Length];

            for (int i = 0; i < encrypted.Length; i++)
            {
                textBytes[i] = (byte)(encrypted[i] ^ DecryptKey[i % DecryptKey.Length]);
            }

            return Encoding.UTF8.GetString(textBytes);
        }
        catch (Exception)
        {
            return cipherText;
        }
    }
}
