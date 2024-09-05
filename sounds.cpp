#include "sounds.h"
#include <QRegularExpression>

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
    QUrl("qrc:/Sounds/UnableToSave.wav"),
    QUrl("qrc:/Sounds/InvalidMusicText.wav")
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

QVector<Sounds::Sound> Sounds::GetMusicFromString(QString str, bool &ok){

    QVector<Sound> music;
    ok = false;

    if(str.size() <= 1)
        return music;

    if(str[str.size()-1] == ',')
        str.removeLast();

    QStringList list = str.split(",");
    uint8_t octave, pos;

    for(int i=0; i<list.size() ;i++){
        switch(list[i].size()){
        case 1:
            if(list[i][0] == '_')
                music.append(Sounds::Sound::silence);
            else
                return music;
            break;
        case 2:
            octave = list[i][0].toLatin1() - '0';
            if(octave < 2 || octave > 6)
                return music;
            switch(list[i][1].toLatin1()){
            case 'C':
                pos = 0;
                break;
            case 'D':
                pos = 2;
                break;
            case 'E':
                pos = 4;
                break;
            case 'F':
                pos = 5;
                break;
            case 'G':
                pos = 7;
                break;
            case 'A':
                pos = 9;
                break;
            case 'B':
                pos = 11;
                break;
            default:
                return music;
                break;
            }
            music.append(Sounds::GetTone(octave,pos));
            break;
        case 3:
            if(list[i][2] != '#')
                return music;
            octave = list[i][0].toLatin1() - '0';
            if(octave < 2 || octave > 6)
                return music;
            switch(list[i][1].toLatin1()){
            case 'C':
                pos = 1;
                break;
            case 'D':
                pos = 3;
                break;
            case 'F':
                pos = 6;
                break;
            case 'G':
                pos = 8;
                break;
            case 'A':
                pos = 10;
                break;
            default:
                return music;
                break;
            }
            music.append(Sounds::GetTone(octave,pos));
            break;
        default:
            return music;
            break;
        }
    }

    ok = true;
    return music;
}

bool Sounds::GetOctavePosFromTone(Sound s, uint8_t &octave, uint8_t &pos){

    if(s > Sound::silence)
        return false;
    else if(s == Sound::silence){
        pos = 13;
        return true;
    }

    octave = (((uint)s)/12)+2;
    pos = ((uint)s)%12;

    return true;
}

bool Sounds::ValidateMusicStr(QString str){
    if(str.size() < 2)
        return false;

    enum unitedstates{ // Also known as MERRRCA
        begin,
        tone,
        sharp,
        comma
    };

    unitedstates state = begin;

    for(int i=0; i<str.size() ;i++){

        switch(str[i].toLatin1()){
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            if(state != begin)
                return false;
            state = tone;
            break;
        case 'C':
        case 'D':
        case 'F':
        case 'G':
        case 'A':
            if(state != tone)
                return false;
            state = sharp;
            break;
        case 'E':
        case 'B':
            if(state != tone)
                return false;
            state = comma;
            break;
        case '#':
            if(state != sharp)
                return false;
            state = comma;
            break;
        case '_':
            if(state!=begin)
                return false;
            state = comma;
            break;
        case ',':
            if(state!=comma && state!=sharp)
                return false;
            state = begin;
            break;
        default:
            return false;
        }
    }

    if(state != begin)
        return false;

    return true;
}
