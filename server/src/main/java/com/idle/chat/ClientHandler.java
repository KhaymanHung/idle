package com.idle.chat;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientHandler implements Runnable {
    private final Socket socket;
    private final String sessionId;
    private BufferedReader in;
    private PrintWriter out;

    public ClientHandler(Socket socket) {
        this.socket = socket;
        this.sessionId = "Client-" + socket.getPort();
    }

    @Override
    public void run() {
        try {
            in = new BufferedReader(new InputStreamReader(socket.getInputStream(), java.nio.charset.StandardCharsets.UTF_8));
            out = new PrintWriter(new java.io.OutputStreamWriter(socket.getOutputStream(), java.nio.charset.StandardCharsets.UTF_8), true);

            Server.broadcast(sessionId + " connected.");
            send("Connected to server. Waiting for data.");

            String rawMessage;
            while ((rawMessage = in.readLine()) != null) {
                if (rawMessage.trim().isEmpty()) {
                    continue;
                }

                String message = SecureComms.decrypt(rawMessage);
                System.out.println("[" + sessionId + "] " + message);
                Server.broadcast(sessionId + ": " + message);
            }
        } catch (IOException e) {
            System.out.println("Client disconnected: " + sessionId);
        } finally {
            closeSession();
            Server.removeClient(this);
            Server.broadcast(sessionId + " disconnected.");
        }
    }

    public void send(String message) {
        if (out != null) {
            out.println(SecureComms.encrypt(message));
        }
    }

    public void closeSession() {
        try {
            if (in != null) {
                in.close();
            }
            if (out != null) {
                out.close();
            }
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        } catch (IOException e) {
            System.out.println("Error closing socket: " + e.getMessage());
        }
    }
}
