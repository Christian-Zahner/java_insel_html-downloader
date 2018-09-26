#ifndef DOWNLOADABLE_H
#define DOWNLOADABLE_H

#include <QObject>
#include <QNetworkReply>

struct Downloadable {
    Downloadable() :
        networkReply(nullptr)
    {}

    Downloadable(QNetworkReply* networkReply) :
        networkReply(networkReply)
    {}

    QNetworkReply* networkReply;

    QString outputDirectory;

    QString outputFileName;

    bool isResource;

};

#endif // DOWNLOADABLE_H
