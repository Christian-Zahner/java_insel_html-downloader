#include "error404file.h"

#include <iostream>
#include <qfile.h>

bool Error404File::serializeSourceCode()
{
    QFile file("404.html");

    if (!file.exists()) {
        std::cerr << "File " << file.fileName().toStdString() << "does not exist!\n";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Could not open file " << file.fileName().toStdString() << "!\n";
        return false;
    }

    if (file.size() == 0) {
        std::cerr << "File is empty. Aborting operation.\n";
        return false;
    }

    QString result;
    for (; !file.atEnd(); result += file.readLine());
    file.close();
    return result.size() > 0;
}

bool Error404File::equals(const QString& sourceCode)
{
    return sourceCode_ == sourceCode;
}
