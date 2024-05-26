#include "backend.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>

#include "qml/qmlfileinfo.h"
#include "applicationconfig.h"

QVariantList Backend::getWaveFilesList(QString subpath)
{
    const QString path = ApplicationConfig::GetFullPatternsPath(subpath);
    qDebug() << "Backend::getWaveFilesList path: " << path;
    QDir dataDir(path);
    QStringList allFiles = dataDir.entryList(ApplicationConfig::WaveFileFilter, QDir::NoDotAndDotDot | QDir::Files);
    qDebug() << "Backend::getWaveFilesList allFiles: " << allFiles;

    QVariantList fileList;

    foreach(auto file, allFiles)
    {
        QmlFileInfo info(dataDir.absoluteFilePath(file));
        qDebug() << "Backend::getWaveFilesList: " << info.getName() << " : " << info.getPath();

        fileList.append(QVariant::fromValue(info));
    }

    return fileList;
}

QVariantList Backend::getTestsWaveFilesList()
{
    const QString path = ApplicationConfig::GetFullTestsPath();
    qDebug() << "Backend::getTestsWaveFilesList path: " << path;
    QDir dataDir(path);
    QStringList allFiles = dataDir.entryList(ApplicationConfig::WaveFileFilter, QDir::NoDotAndDotDot | QDir::Files);
    qDebug() << "Backend::getTestsWaveFilesList allFiles: " << allFiles;

    QVariantList fileList;

    foreach(auto file, allFiles)
    {
        QmlFileInfo info(dataDir.absoluteFilePath(file));
        qDebug() << "Backend::getTestsWaveFilesList: " << info.getName() << " : " << info.getPath();

        fileList.append(QVariant::fromValue(info));
    }

    return fileList;
}

void Backend::copyToFiles(QString filePath, QString name) {
    qDebug() << "Backend::copyToFiles filePath: " << filePath;
    qDebug() << "Backend::copyToFiles name: " << name;
    const QString path = ApplicationConfig::GetFullTestsPath();
    qDebug() << "Backend::copyToFiles path: " << path;

    QDir dir(path);

    QFile::copy(filePath, dir.absoluteFilePath(name + ".wav"));
}

void Backend::copyToTemplates(QString filePath, QString subPath, QString name) {
    qDebug() << "Backend::copyToTemplates filePath: " << filePath;
    qDebug() << "Backend::copyToTemplates subPath: " << subPath;
    qDebug() << "Backend::copyToTemplates name: " << name;
    const QString path = ApplicationConfig::GetFullPatternsPath(subPath);
    qDebug() << "Backend::copyToTemplates path: " << path;

    QDir dir(path);

    auto copyRes = QFile::copy(filePath, dir.absoluteFilePath(name + ".wav"));
    qDebug() << "Backend::copyToTemplates copyRes: " << copyRes;
}

void Backend::deleteWaveFile(QString path)
{
    qDebug() << "Backend::deleteWaveFile: " << path;
    QFile file (path.toLocal8Bit());
    file.remove();
}

QString Backend::openFileDialog()
{
    const QString path = ApplicationConfig::GetFullTestsPath();
#ifdef ANDROID
    qDebug() << "Backend::openFileDialog path: " << path;
    QString fileName = "";
    ApplicationConfig::requestAndroidExternalStoragePermissions();
    auto fileUrl = QFileDialog::getOpenFileUrl(nullptr,
                                               tr("Open File"),
                                               path,
                                               tr("Wave (*.*)"));
    qDebug() << "Backend::openFileDialog fileUrl: " << fileUrl;
    fileName = fileUrl.toString();
#else
    qDebug() << "Backend::openFileDialog path: " << path;
    auto fileName = QFileDialog::getOpenFileName(nullptr,
        tr("Open File"),
        path,
        tr("Wave (*.wav)"));
#endif
    qDebug() << "Backend::openFileDialog: " << fileName;

    return fileName;
}
