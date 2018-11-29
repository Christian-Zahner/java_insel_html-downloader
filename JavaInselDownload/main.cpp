#include <QtCore/QCoreApplication>
#include <iostream>

#include "downloader.h"

int main(int argc, char** argv)
{
    QCoreApplication application(argc, argv);

    std::cout << "Java ist auch eine Insel HTML downloader - 1.0.2\n";

    Downloader downloader;
    downloader.executeDownload();

    return application.exec();
}
