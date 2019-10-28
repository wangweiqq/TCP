#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QByteArray>
#include <QString>
#include <QThread>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(qintptr socketDescriptor,QObject *parent = 0);
    ~MyTcpSocket();
signals:
    void readData(const int,const QString &,const quint16,const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnect(const int ,const QString &,const quint16 );//断开连接的用户信息
public slots:
//    void thisReadData();//处理接收到的数据
    void sentData(const QByteArray &);//发送信号的槽
private:
    qintptr socketID;//保存id，== this->socketDescriptor()；但是this->socketDescriptor()客户端断开会被释放，
                        //断开信号用this->socketDescriptor()得不到正确值
};

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();
signals:
//    void connectClient(const int , const QString & ,const quint16 );//发送新用户连接信息
//    void readData(const int,const QString &, quint16, const QByteArray &);//发送获得用户发过来的数据
//    void sockDisConnect(const int ,const QString &,const quint16 );//断开连接的用户信息
    void sentData(const QByteArray &);//向scoket发送消息
public slots:
//    void setData(const QByteArray & data, const int  handle);//想用户发送消息
    void readDataSlot(const int, const QString &, const quint16,const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnectSlot(int handle, QString ip, quint16 prot);//断开连接的用户信息
public:
    void clear();
protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程
private:
    QMap<int,QTcpSocket *> tcpClient;
};

#endif // MYTCPSERVER_H
