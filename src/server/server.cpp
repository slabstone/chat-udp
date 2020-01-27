#include "server.h"

#include <QtCore/QDataStream>
#include <QtCore/QIODevice>
#include <QtCore/QtDebug>  // qDebug
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>

Server::Server()
{

}

QString Server::address() const
{
    return address_;
}

void Server::setAddress(const QString& address)
{
    if (checkAddress(address))
    {
        address_ = address;
    }
}

quint16 Server::port() const
{
    return port_;
}

void Server::setPort(quint16 port)
{
    if (port >= 1024 && port <= 49151)
    {
        port_ = port;
    }
}

bool Server::start(const QString& address, quint16 port)
{
    setAddress(address);
    setPort(port);
    return start();
}

bool Server::start()
{
    qDebug() << "starting server on" << address_ << port_;
    QObject::connect(&socket,
        SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        SIGNAL(socketStateChanged(QAbstractSocket::SocketState)));

    socket.bind(QHostAddress(address_), port_, QUdpSocket::DontShareAddress);

    return QObject::connect(&socket,
        SIGNAL(readyRead()),
        SLOT(onReadyRead()));
}

void Server::stop()
{
    socket.close();
}

QStringList Server::clients() const
{
    QStringList list;
    for (const ClientAddr& client : clients_)
    {
        list << client.address;
    }
    return list;
}

void Server::processPendingDatagram()
{
    // check datagram size
    const qint64 pendingDatagramSize = socket.pendingDatagramSize();
    if (pendingDatagramSize == -1)
    {
        qDebug() << "pendingDatagramSize error";
        return;
    }
    else if (pendingDatagramSize == 0)
    {
        qDebug() << "pending datagram is empty";
        socket.read(nullptr, 0);
        return;
    }

    // read datagram and check result
    QByteArray datagram(pendingDatagramSize, '\0');
    QHostAddress fromAddress;
    quint16 fromPort = 0U;
    const qint64 readResult = socket.readDatagram(
        datagram.data(), pendingDatagramSize, &fromAddress, &fromPort);
    if (readResult == -1)
    {
        qDebug() << "read error";
        return;
    }
    else if (readResult != pendingDatagramSize)
    {
        qDebug() << "size mismatch";
        return;
    }

    QDataStream stream(&datagram, QIODevice::ReadOnly);
    Message message;
    stream >> message;

    parseMessage(message, fromAddress, fromPort);
}

void Server::parseMessage(const Message& message,
    const QHostAddress& fromAddress, quint16 fromPort)
{
    // process datagram according to message type
    switch (message.type)
    {
    case MessageType::Hello:
        qDebug() << "received Hello from" << fromAddress;
        addClient(fromAddress.toString(), fromPort);
        break;

    case MessageType::Bye:
        qDebug() << "received Bye from" << fromAddress;
        removeClient(fromAddress.toString());
        break;

    case MessageType::Service:
        processServiceMessage();
        break;

    case MessageType::Text:
        qDebug() << "received Text from" << fromAddress << message.sender << message.message;
        forwardMessage(message);
        break;

    default:
        Q_ASSERT(false);
        break;
    }
}

void Server::addClient(const QString& address, quint16 port)
{
    qDebug() << "adding client" << address << port;
    clients_.append({ address, port });
}

void Server::removeClient(const QString& address)
{
    qDebug() << "removing client" << address;
    for (int i = 0; i < clients_.size(); i++)
    {
        const ClientAddr addr(clients_.at(i));
        if (addr.address == address)
        {
            clients_.removeAt(i);
        }
    }
}

void Server::processServiceMessage()
{
    // NYI
}

void Server::forwardMessage(const Message& message)
{
    for (const ClientAddr& client : clients_)
    {
        qDebug() << "forwarding message to" << client.address;
        QByteArray datagram;
        QDataStream stream(&datagram, QIODevice::WriteOnly);
        stream << message;

        socket.writeDatagram(datagram, QHostAddress(client.address), client.port);
    }
}

bool Server::checkAddress(const QString& address)
{
    return QHostAddress(address).protocol() == QAbstractSocket::IPv4Protocol;
}

Q_SLOT void Server::onReadyRead()
{
    qDebug() << "ready read";
    while (socket.hasPendingDatagrams())
    {
        processPendingDatagram();
    }
}
