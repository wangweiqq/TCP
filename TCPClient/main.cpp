#include "dialog.h"
#include <QApplication>
#include <QProcess>
#include <QFileInfo>
bool CheckAppStatus(const QString &appName)
{
	QProcess process;
	//执行tasklist程序
	process.start("tasklist", QStringList() << "/FI" << "imagename eq " + appName);
	//阻塞5秒等待tasklist程序执行完成，超过五秒则直接返回
	process.waitForFinished(5000);
	//把tasklist程序读取到的进程信息输出到字符串中
	QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput());
	qDebug() << outputStr;
	if (outputStr.contains(appName))
	{
		//用完记得把process关闭了，否则如果重新调用这个函数可以会失败
		process.close();
		return true;
	}
	else
	{
		process.close();
		return false;
	}
	return false;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QString filepath = "D:\\Test\\TCP\\debug\\TCPServer.exe";
	QFileInfo fileinfo(filepath);
	QString name = fileinfo.fileName();
	bool bl = CheckAppStatus(name);
	if (!bl) {
		bool bl = QProcess::startDetached(filepath);
		qDebug() << "Process = " << bl;
	}
    Dialog w;
    w.show();

    return a.exec();
}
