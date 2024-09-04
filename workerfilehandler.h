#ifndef WORKERFILEHANDLER_H
#define WORKERFILEHANDLER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>

class WorkerFileHandler : public QObject{
    Q_OBJECT
public:
    WorkerFileHandler(QObject *parent = nullptr);

public slots:
    void GetScoreFile();

signals:
    void FileHandlingError(QString);
    void ScoreFile(QVector<uint>);
    void FileHandlingFinished();
};

#endif // WORKERFILEHANDLER_H
