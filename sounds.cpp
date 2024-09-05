#include "sounds.h"

const QVector<QString> Sounds::_tonestr = {
    "2C", "2C#", "2D", "2D#", "2E", "2F", "2F#", "2G", "2G#", "2A", "2A#", "2B",
    "3C", "3C#", "3D", "3D#", "3E", "3F", "3F#", "3G", "3G#", "3A", "3A#", "3B",
    "4C", "4C#", "4D", "4D#", "4E", "4F", "4F#", "4G", "4G#", "4A", "4A#", "4B",
    "5C", "5C#", "5D", "5D#", "5E", "5F", "5F#", "5G", "5G#", "5A", "5A#", "5B",
    "6C", "6C#", "6D", "6D#", "6E", "6F", "6F#", "6G", "6G#", "6A", "6A#", "6B",
    "_"
};

const QVector<QUrl> Sounds::_soundPaths = {
    QUrl("qrc:/Sounds/2C.wav"),
    QUrl("qrc:/Sounds/2Cs.wav"),
    QUrl("qrc:/Sounds/2D.wav"),
    QUrl("qrc:/Sounds/2Ds.wav"),
    QUrl("qrc:/Sounds/2E.wav"),
    QUrl("qrc:/Sounds/2F.wav"),
    QUrl("qrc:/Sounds/2Fs.wav"),
    QUrl("qrc:/Sounds/2G.wav"),
    QUrl("qrc:/Sounds/2Gs.wav"),
    QUrl("qrc:/Sounds/2A.wav"),
    QUrl("qrc:/Sounds/2As.wav"),
    QUrl("qrc:/Sounds/2B.wav"),
    QUrl("qrc:/Sounds/3C.wav"),
    QUrl("qrc:/Sounds/3Cs.wav"),
    QUrl("qrc:/Sounds/3D.wav"),
    QUrl("qrc:/Sounds/3Ds.wav"),
    QUrl("qrc:/Sounds/3E.wav"),
    QUrl("qrc:/Sounds/3F.wav"),
    QUrl("qrc:/Sounds/3Fs.wav"),
    QUrl("qrc:/Sounds/3G.wav"),
    QUrl("qrc:/Sounds/3Gs.wav"),
    QUrl("qrc:/Sounds/3A.wav"),
    QUrl("qrc:/Sounds/3As.wav"),
    QUrl("qrc:/Sounds/3B.wav"),
    QUrl("qrc:/Sounds/4C.wav"),
    QUrl("qrc:/Sounds/4Cs.wav"),
    QUrl("qrc:/Sounds/4D.wav"),
    QUrl("qrc:/Sounds/4Ds.wav"),
    QUrl("qrc:/Sounds/4E.wav"),
    QUrl("qrc:/Sounds/4F.wav"),
    QUrl("qrc:/Sounds/4Fs.wav"),
    QUrl("qrc:/Sounds/4G.wav"),
    QUrl("qrc:/Sounds/4Gs.wav"),
    QUrl("qrc:/Sounds/4A.wav"),
    QUrl("qrc:/Sounds/4As.wav"),
    QUrl("qrc:/Sounds/4B.wav"),
    QUrl("qrc:/Sounds/5C.wav"),
    QUrl("qrc:/Sounds/5Cs.wav"),
    QUrl("qrc:/Sounds/5D.wav"),
    QUrl("qrc:/Sounds/5Ds.wav"),
    QUrl("qrc:/Sounds/5E.wav"),
    QUrl("qrc:/Sounds/5F.wav"),
    QUrl("qrc:/Sounds/5Fs.wav"),
    QUrl("qrc:/Sounds/5G.wav"),
    QUrl("qrc:/Sounds/5Gs.wav"),
    QUrl("qrc:/Sounds/5A.wav"),
    QUrl("qrc:/Sounds/5As.wav"),
    QUrl("qrc:/Sounds/5B.wav"),
    QUrl("qrc:/Sounds/6C.wav"),
    QUrl("qrc:/Sounds/6Cs.wav"),
    QUrl("qrc:/Sounds/6D.wav"),
    QUrl("qrc:/Sounds/6Ds.wav"),
    QUrl("qrc:/Sounds/6E.wav"),
    QUrl("qrc:/Sounds/6F.wav"),
    QUrl("qrc:/Sounds/6Fs.wav"),
    QUrl("qrc:/Sounds/6G.wav"),
    QUrl("qrc:/Sounds/6Gs.wav"),
    QUrl("qrc:/Sounds/6A.wav"),
    QUrl("qrc:/Sounds/6As.wav"),
    QUrl("qrc:/Sounds/6B.wav"),
    QUrl(""),
    QUrl("qrc:/Sounds/Welcome.wav"),
    QUrl("qrc:/Sounds/ChooseGamemode.wav"),
    QUrl("qrc:/Sounds/Player1Wins.wav"),
    QUrl("qrc:/Sounds/Player2Wins.wav"),
    QUrl("qrc:/Sounds/NewDeviceRecord.wav"),
    QUrl("qrc:/Sounds/NewDailyRecord.wav"),
    QUrl("qrc:/Sounds/NoMusicPractice.wav"),
    QUrl("qrc:/Sounds/NoMusicPlay.wav"),
    QUrl("qrc:/Sounds/YouMissed.wav"),
    QUrl("qrc:/Sounds/UnableToSave.wav")
};

Sounds::Sounds(){}

QUrl Sounds::GetSoundPath(Sound s){
    return _soundPaths[(qsizetype)s];
}

Sounds::Sound Sounds::GetTone(uint8_t octave, uint8_t pos){
    return (Sounds::Sound)(((octave-2)*12)+pos);
}

QString Sounds::GetToneString(Sound s){
    if(s <= Sounds::Sound::silence)
        return _tonestr[(qsizetype)s];

    return QString();
}
