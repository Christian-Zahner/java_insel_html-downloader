#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config : public QObject {
    Q_OBJECT

public:
    bool serialize();

    const QString getFileName() const;

    QString getGeneralHtml() const;

    QString getIndexHtml() const;

    QString getIndexHtml2() const;

    QString getA001Html() const;

    QString getFileExt() const;

    QString getOutputDir() const;

    QString getDataDir() const;

    QStringList getDirectories() const;

private:
    const QString fileName_ = "config.ini";

    QString generalHtml_;

    QString indexHtml_;

    QString indexHtml2_;

    QString a001Html_;

    QString fileExt_;

    QString outputDir_;

    QString dataDir_;

    QStringList directories_;

};

#endif // CONFIG_H
