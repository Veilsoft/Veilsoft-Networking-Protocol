using System;
using System.Text;
using System.Net.Sockets;
using UnityEngine;
using UnityEngine.UI;

public class TCPClient : MonoBehaviour
{
    public string serverIP = "35.182.205.105";
    public int serverPort = 7777;

    private TcpClient client;
    private NetworkStream stream;
    private byte[] buffer = new byte[1024];

    public InputField inputField;
    public Text chatLog;

    void Start()
    {
        ConnectToServer();
    }

    void ConnectToServer()
    {
        try
        {
            client = new TcpClient(serverIP, serverPort);
            stream = client.GetStream();
            Debug.Log("Connected to server");
            chatLog.text += "Connected to server\n";
        }
        catch (Exception e)
        {
            Debug.LogError("Connection failed: " + e.Message);
        }
    }

    public void OnSendButtonClicked() {
        string message = inputField.text.Trim();
        if (string.IsNullOrEmpty(message)) {
            Debug.LogWarning("Cannot send an empty message!");
            return;
        }

        SendMessageToServer(message);
    }

    void SendMessageToServer(string message)
    {
        if (client == null || !client.Connected)
        {
            Debug.LogError("Not connected to server");
            return;
        }

        byte[] data = Encoding.ASCII.GetBytes(message);
        stream.Write(data, 0, data.Length);

        int bytesRead = stream.Read(buffer, 0, buffer.Length);
        string response = Encoding.ASCII.GetString(buffer, 0, bytesRead);
        Debug.Log("Server Response: " + response);
        chatLog.text += "Server: " + response + "\n";

        inputField.text = "";
    }

    void OnApplicationQuit()
    {
        stream.Close();
        client.Close();
    }
}
