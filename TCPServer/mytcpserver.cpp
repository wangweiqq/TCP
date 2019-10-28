#pragma execution_character_set("utf-8")
#include "mytcpserver.h"

MyTcpSocket::MyTcpSocket(qintptr socketDescriptor, QObject *parent) :
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    //转换系统信号到我们需要发送的数据、、直接用lambda表达式了，qdebug是输出线程信息
//    connect(this,&MyTcpSocket::readyRead,this,&myTcpSocket::thisReadData); //连接到收到数据的处理函数
    connect(this,&MyTcpSocket::readyRead, //转换收到的信息，发送信号
            [this](){
                qDebug() <<"myTcpSocket::myTcpSocket lambda readData thread is:" << QThread::currentThreadId();                 emit readData(socketID,this->peerAddress().toString(),this->peerPort() ,this->readAll());//发送用户发过来的数据
            });
    connect(this,&MyTcpSocket::disconnected, //断开连接的信号转换
            [this](){
                qDebug() <<"myTcpSocket::myTcpSocket lambda sockDisConnect thread is:" << QThread::currentThreadId();                 emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort());//发送断开连接的用户信息
            });

    qDebug() << this->socketDescriptor() << " " << this->peerAddress().toString()
                << " " << this->peerPort() << "myTcpSocket::myTcpSocket thread is " <<QThread::currentThreadId();
}
MyTcpSocket::~MyTcpSocket(){
    //QObject::disconnect(this);
}
void MyTcpSocket::sentData(const QByteArray &data)
{
//    //如果是服务器判断好，直接调用write会出现跨线程的现象，所以服务器用广播，每个连接判断是否是自己要发送的信息。
//    if(id == socketID)//判断是否是此socket的信息
//    {
        qDebug() << "myTcpSocket::sentData" << QThread::currentThreadId();
        write(data);
//    }
}
//////////////////////////////////////////////////////
MyTcpServer::MyTcpServer()
{

}
void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    MyTcpSocket * tcpTemp = new MyTcpSocket(socketDescriptor);
    connect(this,SIGNAL(sentData(const QByteArray &)),tcpTemp,SLOT(sentData(const QByteArray &)));
    connect(tcpTemp,&MyTcpSocket::sockDisConnect,this,&MyTcpServer::sockDisConnectSlot);//断开连接的处理，从列表移除，并释放断开的Tcpsocket
	connect(tcpTemp, &MyTcpSocket::readData, this, &MyTcpServer::readDataSlot);//接受到数据
	tcpClient.insert(socketDescriptor,tcpTemp);
//    QThread * thread = new QThread(tcpTemp);//把线程的父类设为连接的，防止内存泄漏
//    //可以信号连接信号的，我要捕捉线程ID就独立出来函数了，使用中还是直接连接信号效率应该有优势
    

//    connect(this,&MyTcpServer::sentData,tcpTemp,&MyTcpSocket::sentData);//发送数据
//    connect(tcpTemp,&MyTcpSocket::disconnected,thread,&QThread::quit);//断开连接时线程退出
//    tcpTemp->moveToThread(thread);//把tcp类移动到新的线程
//    thread->start();//线程开始运行

//    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中
//    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();
//    //发送连接信号
//    emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());
}
//void MyTcpServer::setData(const QByteArray &data, const int handle)
//{
//    emit sentData(data,handle);
//}

void MyTcpServer::sockDisConnectSlot(int handle, QString ip, quint16 prot)
{
    qDebug() <<"MyTcpServer::sockDisConnectSlot thread is:" << QThread::currentThreadId();
    MyTcpSocket * tcp = dynamic_cast<MyTcpSocket *>( tcpClient.value(handle));
    tcpClient.remove(handle);//连接管理中移除断开连接的socket
    tcp->deleteLater();//释放断开连接的资源、、子对象线程也会释放
    qDebug()<<"MyTcpServer::sockDisConnectSlot";
//    emit sockDisConnect(handle,ip,prot);
}
void MyTcpServer::clear(){
    for(auto it = tcpClient.begin();it != tcpClient.end();){
        MyTcpSocket * tcp = dynamic_cast<MyTcpSocket*>(it.value());
        QString str = "关闭";
        tcp->sentData(str.toLocal8Bit());
        tcp->disconnectFromHost();
        tcp->deleteLater();
    }
    tcpClient.clear();
    qDebug()<<"MyTcpServer::clear";
}
void MyTcpServer::readDataSlot(const int id, const QString & ip, const quint16 port,const QByteArray & data)//发送获得用户发过来的数据
{
    qDebug()<<"id:"<<id<<",ip:"<<ip<<",port:"<<port<<",data:"<<data;
}
