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
    void SetScoreFile(QVector<uint> score);
    void SaveMusicFile(QString filename, QString music, int clock);
    void OpenMusicFile(QString filename);

signals:
    void FileHandlingError(QString);
    void ScoreFile(QVector<uint>);
    void FileHandlingFinished();
    void InvalidMusicStr();
    void FileMusic(QString filename, QString music, int clock, QVector<Sounds::Sound> musicvector);
};

#endif // WORKERFILEHANDLER_H
