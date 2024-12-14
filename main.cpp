#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载qss文件
    // 路径前面的:/代表绝对位置
    QFile qss(":/style/styleSheet.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug("Open failed");
    }

    QString fileName = "config.ini";
    // 获取可执行文件的目录
    QString app_path = QCoreApplication::applicationDirPath();
    // QDir::separator()为路径的分隔符，会自动区分windows和linux环境
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    MainWindow w;
    w.show();
    return a.exec();
}
