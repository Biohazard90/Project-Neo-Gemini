#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "isimulated.h"
#include <QString>
#include <QHash>


namespace sf
{
    class SoundBuffer;
    class Sound;
    class Music;
}

class AudioManager : public ISimulated
{
    static AudioManager instance;
    AudioManager();
public:

    static AudioManager *GetInstance();

    void Init();
    void Shutdown();

    void SetMusicEnabled(bool enabled);

    virtual void OnSimulate(float frametime);

    void PrecacheSound(const char *path);
    void PlaySoundSample(const char *path, float volume = 1.0f);

    void PlayMusic(const char *path, float volume = 1.0f, bool looped = true);
    void StopMusic();

private:

    void UpdateSounds(float frametime);
    void UpdateMusic(float frametime);

    bool shutdown;

    struct music_settings_t
    {
        float volume;
        std::string path;
        bool looped;
    };

    struct cross_fade_t
    {
        sf::Music *music;
        float fade;
        float volume;
        bool fadeout;
    };

    struct sound_cache_t
    {
        sf::Sound *sound;
        sf::SoundBuffer *buffer;
        std::string name;
    };

    sound_cache_t *GetSoundForName(const char *path);
    QList<sound_cache_t*> sounds;
    QHash<QString, sf::SoundBuffer*> precache;

    music_settings_t lastMusicSettings;
    QList<cross_fade_t*> music;

    bool musicEnabled;
};

#endif // AUDIOMANAGER_H
