#ifndef LOGGER_H
#define LOGGER_H
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>

class logger {
    public:
        logger();
        ~logger();
        void WriteToLog(QString line);
        void WriteManifestName(QString manifest);
        QString GetManifestName();
    private:
        QFile *log;
        QString timeFormat;
};

#endif // LOGGER_H
