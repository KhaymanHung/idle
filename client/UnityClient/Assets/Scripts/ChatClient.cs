using System;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class ChatClient : MonoBehaviour
{
    [SerializeField] private string host = "";
    [SerializeField] private int port = 0;
    [SerializeField] private bool autoConnectOnStart = true;

    private TcpClient client;
    private NetworkStream stream;
    private readonly byte[] buffer = new byte[4096];
    private bool isConnected;

    private void Start()
    {
        if (string.IsNullOrEmpty(host))
        {
            host = UnityConfig.GetServerIp();
        }

        if (port <= 0)
        {
            port = UnityConfig.GetServerPort();
        }

        if (autoConnectOnStart)
        {
            Connect();
        }
    }

    private void OnDestroy()
    {
        Disconnect();
    }

    public void Connect()
    {
        try
        {
            client = new TcpClient();
            client.Connect(host, port);
            stream = client.GetStream();
            isConnected = true;
            Debug.Log("Connected to TCP server: " + host + ":" + port);
            stream.BeginRead(buffer, 0, buffer.Length, OnReceive, null);
            SendMessage("Unity client connected");
        }
        catch (Exception ex)
        {
            isConnected = false;
            Debug.LogError("Connection failed: " + ex.Message);
        }
    }

    public void Disconnect()
    {
        try
        {
            if (stream != null)
            {
                stream.Close();
            }

            if (client != null)
            {
                client.Close();
            }
        }
        catch (Exception ex)
        {
            Debug.LogWarning("Disconnect warning: " + ex.Message);
        }
        finally
        {
            isConnected = false;
        }
    }

    public void SendMessage(string message)
    {
        if (string.IsNullOrWhiteSpace(message) || !isConnected || client == null || stream == null)
        {
            return;
        }

        try
        {
            string encryptedMessage = SecureComms.Encrypt(message);
            byte[] data = Encoding.UTF8.GetBytes(encryptedMessage + "\n");
            stream.Write(data, 0, data.Length);
            stream.Flush();
            Debug.Log("Sent: " + message + " -> encrypted");
        }
        catch (Exception ex)
        {
            Debug.LogError("Send failed: " + ex.Message);
        }
    }

    private void OnReceive(IAsyncResult ar)
    {
        if (client == null || stream == null)
        {
            return;
        }

        try
        {
            int bytesRead = stream.EndRead(ar);
            if (bytesRead <= 0)
            {
                Debug.Log("Server disconnected.");
                isConnected = false;
                return;
            }

            string rawMessage = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
            if (!string.IsNullOrEmpty(rawMessage))
            {
                string decoded = SecureComms.Decrypt(rawMessage);
                Debug.Log("Server message: " + decoded);
            }

            stream.BeginRead(buffer, 0, buffer.Length, OnReceive, null);
        }
        catch (Exception ex)
        {
            Debug.LogError("Receive failed: " + ex.Message);
        }
    }
}
