#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QObject>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    
    bool startServer(quint16 port = 8888);
    void stopServer();
    void sendMessageToAll(const QString &message);
    void sendMessageToClient(QTcpSocket *client, const QString &message);
    int getClientCount() const { return m_clients.size(); }

signals:
    void serverStarted();
    void serverStopped();
    void clientConnected(int count);
    void clientDisconnected(int count);
    void messageReceived(const QString &message);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    QTcpServer *m_tcpServer;
    QList<QTcpSocket*> m_clients;
    
    void broadcastMessage(const QString &message);
};

#endif // SERVER_H
