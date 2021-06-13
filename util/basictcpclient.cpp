#include "basictcpclient.h"
#include "../util/console.h"

BasicTcpClient::BasicTcpClient(QObject *parent) : QTcpSocket(parent)
  , m_lineEnding("\n")
  , m_port(0)
{
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(&m_reconnectTimer, SIGNAL(timeout()), this, SLOT(tryReconnect()));
    m_reconnectTimer.setInterval(5000);
    m_reconnectTimer.setSingleShot(false);
}

void BasicTcpClient::startConnection() {
    close();

    m_reconnectTimer.start();
    if(m_host.isEmpty() || !m_port) {
        m_networkError = QString("Host or port not set - can't connect to %1:%2").arg(m_host).arg(m_port);
        emit networkErrorChanged(m_networkError);
        INFO << "host or port not set - can't connect" << m_host << m_port;
        return;
    }
    abort();
    tryReconnect();
}

void BasicTcpClient::writeLine(QString line)
{
    write(QString(line + m_lineEnding).toUtf8());
    flush();
    INFO << "wrote" << line;
}

QString BasicTcpClient::lineEnding() const
{
    return m_lineEnding;
}

QString BasicTcpClient::hostName() const
{
    return m_host;
}

int BasicTcpClient::port() const
{
    return m_port;
}

bool BasicTcpClient::connected() const
{
    return state() == ConnectedState;
}

QString BasicTcpClient::networkError() const
{
    return m_networkError;
}

void BasicTcpClient::setLineEnding(QString lineEnding)
{
    if (m_lineEnding == lineEnding)
        return;

    m_lineEnding = lineEnding;
    emit lineEndingChanged(lineEnding);
}

void BasicTcpClient::setHostName(QString hostName)
{
    if (m_host == hostName)
        return;

    m_host = hostName;
    emit connectionChanged();
}

void BasicTcpClient::setPort(int port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit connectionChanged();
}

void BasicTcpClient::tryReconnect() {
    if(m_host.isEmpty() || !m_port) {
        m_networkError = "Please set host and port to connect";
        emit networkErrorChanged(m_networkError);
        return;
    } else {
        if(state() != QAbstractSocket::UnconnectedState) {
            disconnectFromHost();
            waitForDisconnected(500);
        }
        connectToHost(m_host, static_cast<quint16>(m_port));
    }
}

void BasicTcpClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    if(connected()) {
        m_reconnectTimer.stop();
        m_networkError = "";
        emit networkErrorChanged(m_networkError);
    }
    emit connectedChanged(state == QTcpSocket::ConnectedState);
}

void BasicTcpClient::socketError(QAbstractSocket::SocketError err) {
    Q_UNUSED(err)
    INFO << "Socket error:" << errorString();
    m_networkError = errorString() + " : " + m_host + ":" + QString::number(m_port);
    emit networkErrorChanged(m_networkError);
    m_reconnectTimer.start();
}

void BasicTcpClient::readClient() {
    while(canReadLine()) {
        QByteArray lineBA = readLine();
        QString line = QString(lineBA).trimmed();
        // qDebug() << line;
        emit receivedLine(line);
    }
}
