#include "dialog.h"
#include <QApplication>
#include <QProcess>
#include <QFileInfo>
bool CheckAppStatus(const QString &appName)
{
	QProcess process;
	//ִ��tasklist����
	process.start("tasklist", QStringList() << "/FI" << "imagename eq " + appName);
	//����5��ȴ�tasklist����ִ����ɣ�����������ֱ�ӷ���
	process.waitForFinished(5000);
	//��tasklist�����ȡ���Ľ�����Ϣ������ַ�����
	QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput());
	qDebug() << outputStr;
	if (outputStr.contains(appName))
	{
		//����ǵð�process�ر��ˣ�����������µ�������������Ի�ʧ��
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
