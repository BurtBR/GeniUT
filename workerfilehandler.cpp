#include "workerfilehandler.h"

WorkerFileHandler::WorkerFileHandler(QObject *parent) : QObject{parent}{

}

void WorkerFileHandler::GetScoreFile(){

    QVector<uint> score = {0,0,0,0,0};
    QString scorefilename = "Scores.geut", filecontent;
    QStringList scorelist;
    QFile *fp = nullptr;
    QTextStream *in = nullptr;
    bool ok;

    if(QFileInfo::exists(scorefilename)){
        try{
            fp = new QFile(scorefilename);
        }catch(...){
            emit FileHandlingError("Unable to allocate memory for File pointer");
            emit FileHandlingFinished();
            return;
        }

        if(!fp->open(QIODevice::ReadOnly | QIODevice::Text)){
            delete fp;
            emit FileHandlingError("Unable to read file");
            emit FileHandlingFinished();
            return;
        }

        try{
            in = new QTextStream(fp);
        }catch(...){
            fp->close();
            delete fp;
            emit FileHandlingError("Unable to allocate memory for Text Stream");
            emit FileHandlingFinished();
            return;
        }

        filecontent = in->readAll();
        fp->close();
        delete in;
        delete fp;

        scorelist = filecontent.split(",");

        if(scorelist.size() != 5){
            emit FileHandlingError("Corrupted score file");
            emit FileHandlingFinished();
            return;
        }

        for(int i=0; i<score.size() ;i++){
            score[i] = scorelist[i].toUInt(&ok);
            if(!ok){
                emit FileHandlingError("Corrupted score file");
                emit FileHandlingFinished();
                return;
            }
        }
    }

    emit ScoreFile(score);
    emit FileHandlingFinished();
}

void WorkerFileHandler::SaveMusicFile(QString filename, QString music, int clock){
    if(music[music.size()-1]!=',')
        music.append(',');

    if(!Sounds::ValidateMusicStr(music)){
        emit InvalidMusicStr();
        emit FileHandlingFinished();
        return;
    }

    QFile *fp = nullptr;
    QTextStream *out = nullptr;

    try{
        fp = new QFile(filename);
    }catch(...){
        emit FileHandlingError("Unable to allocate memory for File Pointer");
        emit FileHandlingFinished();
        return;
    }

    if(!fp->open(QIODevice::WriteOnly | QIODevice::Text)){
        delete fp;
        emit FileHandlingError("Unable to open File");
        emit FileHandlingFinished();
        return;
    }

    try{
        out = new QTextStream(fp);
    }catch(...){
        fp->close();
        delete fp;
        emit FileHandlingError("Unable to allocate memory for Text Stream");
        emit FileHandlingFinished();
        return;
    }

    music.prepend(QString::number(clock) + ",");

    *out << music;

    fp->close();
    delete fp;
    delete out;

    emit FileHandlingFinished();
}
