#include "config.h"

#include <iostream>

#include <QFile>
#include <QSettings>

bool Config::serialize() {
    if (!QFile::exists(fileName_)) {
        std::cerr << "File " << qPrintable(fileName_) << " does not exist.\nAborting the download.\n";
    }

    QSettings settings(fileName_, QSettings::Format::IniFormat);

    settings.beginGroup("Html");
    generalHtml_ = settings.value("General", "").toString();
    indexHtml_ = settings.value("Index", "").toString();
    indexHtml2_ = settings.value("Index2", "").toString();
    a001Html_ = settings.value("A_001", "").toString();
    fileExt_ = settings.value("FileExt", "").toString();
    settings.endGroup();

    settings.beginGroup("Dir");
    outputDir_ = settings.value("Output", "").toString();
    dataDir_ = settings.value("Data", "").toString();
    directories_ = settings.value("Directories").toStringList();
    settings.endGroup();

    return !generalHtml_.isEmpty()
        && !indexHtml_.isEmpty()
        && !indexHtml2_.isEmpty()
        && !fileExt_.isEmpty()
        && !outputDir_.isEmpty()
        && !dataDir_.isEmpty()
        && !directories_.isEmpty();
}

const QString Config::getFileName() const
{
    return fileName_;
}

QString Config::getGeneralHtml() const
{
    return generalHtml_;
}

QString Config::getIndexHtml() const
{
    return indexHtml_;
}

QString Config::getIndexHtml2() const
{
    return indexHtml2_;
}

QString Config::getA001Html() const
{
    return a001Html_;
}

QString Config::getFileExt() const
{
    return fileExt_;
}

QString Config::getOutputDir() const
{
    return outputDir_;
}

QString Config::getDataDir() const
{
    return dataDir_;
}

QStringList Config::getDirectories() const
{
    return directories_;
}
