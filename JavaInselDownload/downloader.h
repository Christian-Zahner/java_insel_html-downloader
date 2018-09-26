#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QtNetwork>

#include "config.h"
#include "downloadable.h"
#include "error404file.h"

class Downloader : public QObject {
    Q_OBJECT

public:
    Downloader();

    void executeDownload();

private:
    Error404File error404File_;

    Config config_;

    QNetworkAccessManager manager_;

    QVector<Downloadable*> currentDownloads_;

    QStringList fileNameList_;

    QStringList downloadedFileNames_;

    QRegExp commonFolderRegExp_;

    void downloadOtherFiles();

    void downloadChapters();

    void downloadCommonFolderContent();

    void setupRegex();

    QString saveFileName(const QUrl& url, Downloadable* downloadable);

    bool saveToDisk(Downloadable* downloadable, QIODevice* data);

    bool isHttpRedirect(QNetworkReply* reply);

    void downloadFile(const QUrl& url, bool isResource = false);

    Downloadable* getDownloadable(QNetworkReply* reply);

private slots:
    void downloadFinished(QNetworkReply* reply);

    void sslErrors(const QList<QSslError>& errors);

};

#endif // DOWNLOADER_H
