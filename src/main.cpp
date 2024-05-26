#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>

#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>

#include "backend.h"
#include "applicationconfig.h"

void cleanDataDir();
void copyTestDataFilesForAndroid(QString, QString);

static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void logToFile(const QString message)
{
    QFile file("logs.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << message << endl;
    }
}

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString message = QString("%1: %2").arg(
        QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss:zzz "),
        QString(localMsg.constData())
        );
    logToFile(message);

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, message);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qInstallMessageHandler(messageOutput);

    QApplication app(argc, argv);

    qmlRegisterType<Backend>("intondemo.backend", 1, 0, "Backend");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

#ifdef ANDROID
    copyTestDataFilesForAndroid(
        ApplicationConfig::TestsPath,
        ApplicationConfig::GetFullTestsPath()
    );
#endif

    auto result = QApplication::exec();

#ifdef ANDROID
    cleanDataDir();
#endif

    return result;
}

#ifdef ANDROID
#include <QtAndroid>

void copyTestDataFilesForAndroid(QString srcPath, QString dstPath)
{
    ApplicationConfig::requestAndroidExternalStoragePermissions();

    qDebug() << "copyTestDataFilesForAndroid";

    const QString src = "assets:/" + srcPath;
    qDebug() << "copyTestDataFilesForAndroid src: " << src;

    qDebug() << "copyTestDataFilesForAndroid dst: " << dstPath;

    QDir srcDir(src);
    if (!srcDir.exists())
    {
        qDebug() << "copyTestDataFilesForAndroid srcDir: Not Exists";
        return;
    }

    foreach (QString f, srcDir.entryList(QDir::Files)) {
        qDebug() << "copyTestDataFilesForAndroid QFile::copy f: " << f;
        auto copyRes = QFile::copy(src + QDir::separator() + f, dstPath + QDir::separator() + f);
        qDebug() << "copyTestDataFilesForAndroid QFile::copy copyRes: " << copyRes;
    }
}
#else
void copyTestDataFilesForAndroid(QString subPath, QString dstPath)
{
    qDebug() << "copyTestDataFilesForAndroid - Skip";
}
#endif

void cleanDataDir()
{
    QDir dataDir(ApplicationConfig::GetFullDataPath());
    QStringList allFiles = dataDir.entryList(ApplicationConfig::WaveFileFilter, QDir::NoDotAndDotDot | QDir::Files);

    foreach(auto file, allFiles)
    {
        QDir dir;
        dir.remove(dataDir.absoluteFilePath(file));
    }
}
