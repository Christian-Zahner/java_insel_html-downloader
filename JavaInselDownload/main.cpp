#include <QtCore/QCoreApplication>

#include "downloader.h"

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);

    Downloader downloader;
    downloader.executeDownload();

    return a.exec();
}
