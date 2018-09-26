#ifndef ERROR404FILE_H
#define ERROR404FILE_H

#include <QObject>

class Error404File : public QObject {
    Q_OBJECT

public:
    bool serializeSourceCode();

    bool equals(const QString& sourceCode);

private:
    QString sourceCode_;

};

#endif // ERROR404FILE_H
