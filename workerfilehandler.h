#ifndef WORKERFILEHANDLER_H
#define WORKERFILEHANDLER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>

#include "sounds.h"

class WorkerFileHandler : public QObject{
    Q_OBJECT
public:
    WorkerFileHandler(QObject *parent = nullptr);

public slots:
    void GetScoreFile();
    void SaveMusicFile(QString filename, QString music, int clock);

signals:
    void FileHandlingError(QString);
    void ScoreFile(QVector<uint>);
    void FileHandlingFinished();
    void InvalidMusicStr();
};

#endif // WORKERFILEHANDLER_H
