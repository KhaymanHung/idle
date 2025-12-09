#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void onConnectClicked();
    void onSendClicked();
    void onClientConnected();
    void onClientDisconnected();
    void onMessageReceived(const QString &message);
    void onClientError(const QString &error);

private:
    Ui::ClientWindow *ui;
    Client *m_client;
    bool m_isConnected;
    
    void appendMessage(const QString &message);
    void updateStatus(bool connected);
};

#endif // CLIENTWINDOW_H
