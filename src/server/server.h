#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QtGlobal>  // quint16
#include <QtNetwork/QUdpSocket>

#include "common/protocol.h"  // Message

class Server : public QObject
{
    Q_OBJECT

public:
    Server();

    // get and set server network params
    QString address() const;
    void setAddress(const QString& address);
    quint16 port() const;
    void setPort(quint16 port);

    bool start(const QString& address, quint16 port);
    bool start();
    void stop();

    QStringList clients() const;

private:
    void processPendingDatagram();
    void parseMessage(const Message& datagram,
        const QHostAddress& fromAddress, quint16 fromPort);

    // add client
    void addClient(const QString& address, quint16 port);

    // remove client
    void removeClient(const QString& address);

    //
    void processServiceMessage();

    void forwardMessage(const Message& message);

    // validate input address
    bool checkAddress(const QString& address);

    Q_SIGNAL void socketStateChanged(QAbstractSocket::SocketState socketState);

    // datagram processing
    Q_SLOT void onReadyRead();

    // network params
    QString address_;
    quint16 port_ = 0U;

    // network socket
    QUdpSocket socket;

    struct ClientAddr
    {
        QString address;
        quint16 port;
    };

    QList<ClientAddr> clients_;
};

#endif  // SERVER_H
