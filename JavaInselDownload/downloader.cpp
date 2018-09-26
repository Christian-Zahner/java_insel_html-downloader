#include "downloader.h"

#include <iostream>

Downloader::Downloader()
{
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}
void Downloader::executeDownload()
{
    /* Serialize download information */
    error404File_.serializeSourceCode();
    config_.serialize();

    setupRegex();

    /* Download chapters */
    downloadOtherFiles();
    downloadChapters();
}

void Downloader::downloadOtherFiles()
{
    downloadFile(config_.getGeneralUrl() + "index.html");
    downloadFile(config_.getGeneralUrl() + "stichwortverzeichnis.html");
    downloadFile(config_.getGeneralUrl() + "A_001.html");
}

void Downloader::downloadChapters()
{
    const auto url = config_.getGeneralUrl();

    for (uchar i = 0; i < 50; ++i) {
        for (uchar j = 0; j < 50; ++j) {
            auto size = currentDownloads_.size();
            QString fileName = QString::number(i).rightJustified(2, '0') + "_";
            fileName += QString::number(j).rightJustified(3, '0');
            downloadFile(url + fileName + ".html");
        }
    }
}

void Downloader::downloadCommonFolderContent()
{
    while (fileNameList_.size() > 0) {
        QStringList regexResults;
        int position = 0;

        QFile file(fileNameList_.first());

        if (!file.open(QIODevice::ReadOnly)) {
            continue;
        }

        QString fileContent;
        while (!file.atEnd()) {
            fileContent += file.readLine();
        }

        while ((position = commonFolderRegExp_.indexIn(fileContent, position)) != -1) {
            const auto match = commonFolderRegExp_.cap(0);
            if (match.size() > 0 && !regexResults.contains(match) && !downloadedFileNames_.contains(match)) {
                regexResults << match;
                downloadFile(config_.getGeneralUrl() + match, true);
            }
            position += commonFolderRegExp_.matchedLength();

        }

        fileNameList_.removeFirst();
    }
}

void Downloader::setupRegex()
{
    const auto directories = config_.getDirectories();
    QString regExp;

    for (auto i = 0; i < directories.size(); ++i) {
        regExp += "(" + directories[i] + "[a-zA-Z0-9/\\.\\_-=]{1,})";
        if (i < directories.size() - 1) {
            regExp += "|";
        }
    }
    commonFolderRegExp_ = QRegExp(regExp);
}

QString Downloader::saveFileName(const QUrl& url, Downloadable* downloadable)
{
    QString basename = QFileInfo(url.path()).fileName();

    if (!downloadable->outputDirectory.isNull() && !QDir().exists(downloadable->outputDirectory)) {
        if (!QDir().mkpath(downloadable->outputDirectory)) {
            std::cerr << "Failed to create directory " << downloadable->outputDirectory.toStdString() << "\n";
            return "";
        }
    }

    if (basename.isEmpty()) {
        basename = "download";
    }

    if (QFile::exists(downloadable->outputDirectory + basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(downloadable->outputDirectory + basename + QString::number(i))) {
            ++i;
        }
        basename += QString::number(i);
    }

    return downloadable->outputDirectory + basename;
}

bool Downloader::saveToDisk(Downloadable* downloadable, QIODevice* data)
{
    auto fileName = config_.getOutputDir() + (downloadable->outputDirectory.isEmpty() ? "" : downloadable->outputDirectory + "/");

    QDir dir;
    if (!dir.exists(fileName)) {
        dir.mkpath(fileName);
    }

    fileName += downloadable->outputFileName;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n", qPrintable(file.fileName()), qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();
    fileNameList_.append(file.fileName());
    downloadCommonFolderContent();
    return true;
}


bool Downloader::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
        || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void Downloader::downloadFile(const QUrl& url, bool isResource)
{
    QNetworkRequest request(url);
    QNetworkReply* reply = manager_.get(request);
    auto* downloadable = new Downloadable(reply);

#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif 

    auto urlString = url.toString();
    downloadable->outputFileName = urlString.split('/').last();

    if (!urlString.contains("http:") || (downloadable->isResource = isResource)) {
        urlString.remove(config_.getGeneralUrl());
        downloadable->outputDirectory = urlString.left(urlString.size() - downloadable->outputFileName.size());
    }

    downloadedFileNames_.append((isResource ? downloadable->outputDirectory + downloadable->outputFileName : url.fileName()));
    currentDownloads_.append(downloadable);
}

Downloadable* Downloader::getDownloadable(QNetworkReply* reply)
{
    for (auto it = currentDownloads_.begin(); it != currentDownloads_.end(); ++it) {
        if ((*it)->networkReply == reply) {
            return *it;
        }
    }
    return nullptr;
}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors) {
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
    }
#endif
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    auto* downloadable = getDownloadable(reply);

    if (reply->error()) {
        downloadedFileNames_.removeAll(url.fileName());
    }
    else if (isHttpRedirect(reply)) {
        fputs("Request was redirected.\n", stderr);
    }
    else {
        //auto fileName = saveFileName(url, downloadable);
        if (saveToDisk(downloadable, reply)) {
            std::cout << "File " << url.toEncoded().constData() << " downloaded successfully to " << url.fileName().toStdString() << "\n";
        }
    }

    currentDownloads_.removeAll(downloadable);
    delete downloadable;

    reply->deleteLater();

    if (currentDownloads_.size() == 0) {
        std::cout << "Download successful.";
    }
}