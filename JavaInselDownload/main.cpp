#include <QtCore/QCoreApplication>

#include <iostream>

#include "downloader.h"

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    
    std::cout << "Java ist auch eine Insel HTML downloader - 1.0.0\n";
    Downloader downloader;
    downloader.executeDownload();

    return a.exec();
}
