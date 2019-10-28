#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    server = new MyTcpServer();
    server->listen(QHostAddress::Any,8888);
}

Dialog::~Dialog()
{
    delete ui;
}
