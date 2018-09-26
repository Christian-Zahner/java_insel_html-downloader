#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config : public QObject {
    Q_OBJECT

public:
    bool serialize();

    QString getGeneralUrl() const;

    QString getOutputDir() const;

    QString getDataDir() const;

    QStringList getDirectories() const;

private:
    QString generalUrl_;

    QString outputDir_;

    QString dataDir_;

    QStringList directories_;

};

#endif // CONFIG_H
