#pragma execution_character_set("utf-8")
#include "dialog.h"
#include "ui_dialog.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadData()));
	on_pushButton_clicked();
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::onConnected(){
    qDebug()<<"client onConnected";
}
void Dialog::onDisconnected(){
    qDebug()<<"client onDisconnected";
}
void Dialog::onReadData(){
     QByteArray data = socket->readAll();
     qDebug()<<"read : "<<data;
}
void Dialog::on_pushButton_clicked(){
    QHostAddress add("127.0.0.1");
    socket->connectToHost(add,8888);
}
void Dialog::on_pushButton_2_clicked(){
    socket->disconnectFromHost();
}
void Dialog::on_pushButton_3_clicked(){
    QString str = ui->lineEdit->text();
    QByteArray data = str.toLocal8Bit();
    socket->write(data);
}
