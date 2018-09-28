#include "downloader.h"

#include <iostream>

Downloader::Downloader()
{
    connect(&manager_, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}
void Downloader::executeDownload()
{
    /* Serialize download information */
    /* error404File_.serializeSourceCode(); */
    if (!config_.serialize()) {
        std::cerr << "Could not read file " << qPrintable(config_.getFileName()) << ".\nAborting the download.\n";
    }

    setupRegex();

    /* Download chapters */
    downloadOtherFiles();
    downloadChapters();
}

void Downloader::downloadOtherFiles()
{
    downloadFile(config_.getGeneralHtml() + config_.getIndexHtml() + config_.getFileExt());
    downloadFile(config_.getGeneralHtml() + config_.getIndexHtml2() + config_.getFileExt());
    downloadFile(config_.getGeneralHtml() + config_.getA001Html() + config_.getFileExt());
}

void Downloader::downloadChapters()
{
    std::cout << "Downloading chapters ...\n";
    const auto url = config_.getGeneralHtml();

    for (uchar i = 0; i < 50; ++i) {
        for (uchar j = 0; j < 50; ++j) {
            auto size = currentDownloads_.size();
            QString fileName = QString::number(i).rightJustified(2, '0') + "_";
            fileName += QString::number(j).rightJustified(3, '0');
            downloadFile(url + fileName + config_.getFileExt());
        }
    }
    std::cout << "Downloading resources ...";
}

void Downloader::downloadCommonFolderContent()
{
    while (fileNameList_.size() > 0) {
        QFile file(fileNameList_.first());
        if (!file.open(QIODevice::ReadOnly)) {
            continue;
        }

        QString fileContent;
        while (!file.atEnd()) {
            fileContent += file.readLine();
        }

        QStringList regexResults;
        for (auto position = 0; (position = commonFolderRegExp_.indexIn(fileContent, position)) != -1;) {
            const auto match = commonFolderRegExp_.cap(0);
            if (match.size() != 0 && !regexResults.contains(match) && !downloadedFileNames_.contains(match)) {
                regexResults << match;
                downloadFile(config_.getGeneralHtml() + match, true);
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

    const auto penultimateIndex = directories.size() - 1;
    for (auto i = 0; i < directories.size(); ++i) {
        regExp += '(' + directories[i] + "[a-zA-Z0-9/\\.\\_-=]{1,})";
        if (i < penultimateIndex) {
            regExp += '|';
        }
    }
    commonFolderRegExp_ = QRegExp(regExp);
}

QString Downloader::saveFileName(const QUrl& url, Downloadable* downloadable)
{
    auto basename = QFileInfo(url.path()).fileName();

    if (!downloadable->outputDirectory.isNull() && !QDir().exists(downloadable->outputDirectory)) {
        if (!QDir().mkpath(downloadable->outputDirectory)) {
            std::cerr << "Failed to create directory " << qPrintable(downloadable->outputDirectory) << "\n";
            return QString();
        }
    }

    if (basename.isEmpty()) {
        basename = "download";
    }

    if (QFile::exists(downloadable->outputDirectory + basename)) {
        // already exists, don't overwrite
        basename += '.';
        auto i = 0;
        for (; QFile::exists(downloadable->outputDirectory + basename + QString::number(i)); ++i);
        basename += QString::number(i);
    }

    return downloadable->outputDirectory + basename;
}

bool Downloader::saveToDisk(Downloadable* downloadable, QIODevice* data)
{
    if (downloadable == nullptr) {
        return false;
    }

    auto fileName = config_.getOutputDir() + (downloadable->outputDirectory.isEmpty() ? "" : downloadable->outputDirectory + "/");

    QDir dir(fileName);
    if (!dir.exists()) {
        if (!dir.mkpath(fileName)) {
            return false;
        }
    }

    fileName += downloadable->outputFileName;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
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
    auto* reply = manager_.get(request);
    auto* downloadable = new Downloadable(reply);

    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));

    auto urlString = url.toString();
    downloadable->outputFileName = urlString.split('/').last();

    if (!urlString.contains("http:") || (downloadable->isResource = isResource)) {
        urlString.remove(config_.getGeneralHtml());
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

void Downloader::sslErrors(const QList<QSslError>& sslErrors)
{
    for (const auto &error : sslErrors) {
        std::cerr << "SSL error: " << qPrintable(error.errorString()) << '\n';
    }
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    const auto url = reply->url();
    auto* downloadable = getDownloadable(reply);

    if (reply->error()) {
        downloadedFileNames_.removeAll(url.fileName());
    }
    else if (isHttpRedirect(reply)) {
        std::cerr << "Request was redirected.\n";
    }
    else {
        saveToDisk(downloadable, reply);
    }

    currentDownloads_.removeAll(downloadable);
    delete downloadable;

    reply->deleteLater();

    if (currentDownloads_.size() == 0) {
        std::cout << "\nDownload successful.\n";
    }
}