#ifndef SOUNDS_H
#define SOUNDS_H

#include <QVector>
#include <QUrl>

class Sounds{
public:
    enum class Sound{
        _begin=-1,
        C2, C2s, D2, D2s, E2, F2, F2s, G2, G2s, A2, A2s, B2,
        C3, C3s, D3, D3s, E3, F3, F3s, G3, G3s, A3, A3s, B3,
        C4, C4s, D4, D4s, E4, F4, F4s, G4, G4s, A4, A4s, B4,
        C5, C5s, D5, D5s, E5, F5, F5s, G5, G5s, A5, A5s, B5,
        C6, C6s, D6, D6s, E6, F6, F6s, G6, G6s, A6, A6s, B6,
        silence,
        welcome,
        choosegamemode,
        player1wins,
        player2wins,
        newdevicerecord,
        newdailyrecord,
        nomusicpractice,
        nomusicplay,
        youmissed,
        unabletosave,
        invalidmusictext,
        filehandlingfail,
        musiccomplete,
        _end
    };

private:
    static const QVector<QString> _tonestr;
    static const QVector<QUrl> _soundPaths;

public:
    Sounds();
    static QUrl GetSoundPath(Sound s);
    static Sound GetTone(uint8_t octave, uint8_t pos);
    static uint8_t GetOctave(Sound s);
    static QString GetToneString(Sound s);
    static QVector<Sound> GetMusicFromString(QString str, bool &ok);
    static bool GetOctavePosFromTone(Sound s, uint8_t &octave, uint8_t &pos);
    static bool ValidateMusicStr(QString str);
};

#endif // SOUNDS_H
