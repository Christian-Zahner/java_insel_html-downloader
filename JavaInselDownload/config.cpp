#include "config.h"

#include <iostream>

#include <QFile>
#include <QSettings>

bool Config::serialize() {
    if (!QFile::exists(fileName_)) {
        std::cerr << "File " << qPrintable(fileName_) << " does not exist.\nAborting the download.\n";
        return false;
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

    auto result = true;
    QString missingValues;
    if (generalHtml_.isEmpty()) {
        missingValues += " General";
        result = false;
    }
    if (indexHtml_.isEmpty()) {
        missingValues += " Index";
        result = false;
    }
    if (indexHtml2_.isEmpty()) {
        missingValues += " Index2";
        result = false;
    }
    if (a001Html_.isEmpty()) {
        missingValues += " A_001";
        result = false;
    }
    if (fileExt_.isEmpty()) {
        missingValues += " FileExt";
        result = false;
    }
    if (outputDir_.isEmpty()) {
        missingValues += " Output";
        result = false;
    }
    if (dataDir_.isEmpty()) {
        missingValues += " Data";
        result = false;
    }
    if (directories_.isEmpty()) {
        missingValues += " Directories";
        result = false;
    }
    if (!result) {
        std::cerr << "Could not read values " << missingValues.toStdString() << " from " 
            << qPrintable(fileName_) << '\n';
    }
    return result;
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
