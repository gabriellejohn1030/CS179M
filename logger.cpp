#include "logger.h"

logger::logger() {
    QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    int year = QDate::currentDate().year();
    QString fileName =  "/" + QString::number(year) + "LOG.txt";
    log = new QFile(location + fileName);
    if (!log->open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "Failed to open file: " << fileName;
    } else {
        qDebug() << "File " << log->fileName() << " opened";
    }
}

logger::~logger() {
    log->close();
    delete(log);
    log = nullptr;
}

void logger::WriteToLog(QString line) {
    QString as = QDateTime::currentDateTime().toString("MM-dd-yyyy: HH:mm:ss: ");
    log->write(as.toUtf8() + line.toUtf8() + "\n");
    qDebug() << "Log to file: " << as << line;
}

void logger::WriteManifestName(QString manifest) {
    QString fileName = "manifest_name.txt";
    QFile manFile(fileName);
    if (!manFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file " << manFile.fileName();
    } else {
        manFile.write(manifest.toUtf8());
        qDebug() << "File " << manFile.fileName() << " opened";
        manFile.close();
        qDebug() << "File " << manFile.fileName() << " closed";
    }
}

// test later to see if the name of the manifest file is saved
// to a file if the pc turns off while using the application
QString logger::GetManifestName() {
    QString fileName = "manifest_name.txt";
    QFile manFile(fileName);
    if (!manFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file " << manFile.fileName();
        return "";
    }
    QTextStream in(&manFile);
    QString manifestName = in.readLine();
    manFile.close();
    return manifestName;
}
