#include "config.h"

#include <QSettings>

bool Config::serialize() {
    QSettings settings("config.ini", QSettings::Format::IniFormat);

    settings.beginGroup("Url");
    generalUrl_ = settings.value("General", "").toString();
    settings.endGroup();

    settings.beginGroup("Dir");
    outputDir_ = settings.value("Output", "").toString();
    dataDir_ = settings.value("Data", "").toString();
    directories_ = settings.value("Directories").toStringList();
    settings.endGroup();

    return !generalUrl_.isEmpty()
        && !outputDir_.isEmpty()
        && !dataDir_.isEmpty()
        && !directories_.isEmpty();
}

QString Config::getGeneralUrl() const
{
    return generalUrl_;
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
