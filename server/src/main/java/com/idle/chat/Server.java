package com.idle.chat;

import java.io.IOException;
import java.net.BindException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class Server {
    private static final List<ClientHandler> clients = new CopyOnWriteArrayList<>();

    public static void main(String[] args) {
        ServerConfig.load();

        String serverName = ServerConfig.getServerName();
        int port = ServerConfig.getServerPort();
        String dbIp = ServerConfig.getDbIp();
        int dbPort = ServerConfig.getDbPort();
        String dbUsername = ServerConfig.getDbUsername();
        String dbPassword = ServerConfig.getDbPassword();
        int maxConnections = ServerConfig.getClientMaxConnects();

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("========================================");
            System.out.println("Server name: " + serverName);
            System.out.println("Server port " + port);
            System.out.println("DB IP: " + dbIp);
            System.out.println("DB Port: " + dbPort);
            // System.out.println("DB Username: " + dbUsername);
            // System.out.println("DB Password: " + dbPassword);
            System.out.println("Client Max Connects: " + maxConnections);
            System.out.println("Waiting for client connections...");
            System.out.println("========================================");

            while (true) {
                Socket socket = serverSocket.accept();
                ClientHandler handler = new ClientHandler(socket);
                clients.add(handler);
                Thread thread = new Thread(handler, "ClientThread-" + socket.getPort());
                thread.start();
                System.out.println("Connected client: " + socket.getRemoteSocketAddress());
            }
        } catch (BindException e) {
            System.err.println("Port " + port + " is already in use. Please close the process using it or change server port in config.ini.");
            System.err.println("Server startup aborted.");
        } catch (IOException e) {
            System.err.println("Failed to start server on port " + port + ": " + e.getMessage());
        }
    }

    public static void broadcast(String message) {
        System.out.println(message);

        for (ClientHandler client : clients) {
            client.send(message);
        }
    }

    public static void removeClient(ClientHandler client) {
        clients.remove(client);
    }
}
