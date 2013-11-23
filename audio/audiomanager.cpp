
#include "base.h"

#include <QFile>
#include <QDir>

#include "SFML/Audio.hpp"

AudioManager AudioManager::instance;
AudioManager *AudioManager::GetInstance()
{
    return &instance;
}

AudioManager::AudioManager()
{
    shutdown = false;
    lastMusicSettings.volume = 0.0f;
    musicEnabled = true;
}

void AudioManager::Init()
{
    SimulationList::GetInstance()->AddSimulationObject(this);
}

void AudioManager::Shutdown()
{
    shutdown = true;

    DESTROY_QHASH(QString, sf::SoundBuffer*, precache);

    FOREACH_QLIST(sounds, sound_cache_t*, c)
    {
        c->sound->stop();
        delete c->sound;
        delete c->buffer;
        delete c;
    }
    FOREACH_QLIST_END;

    FOREACH_QLIST(music, cross_fade_t*, c)
    {
        c->music->stop();
        delete c->music;
        delete c;
    }
    FOREACH_QLIST_END;

    sounds.clear();
    precache.clear();

    SimulationList::GetInstance()->RemoveSimulationObject(this);
}

void AudioManager::SetMusicEnabled(bool enabled)
{
    if (musicEnabled == enabled)
        return;

    musicEnabled = enabled;

    if (enabled && lastMusicSettings.volume > 0.0f)
    {
        PlayMusic(lastMusicSettings.path.c_str(),
                  lastMusicSettings.volume,
                  lastMusicSettings.looped);
    }
    else if (!enabled)
    {
        StopMusic();
    }
}

void AudioManager::OnSimulate(float frametime)
{
    UpdateSounds(frametime);

    UpdateMusic(frametime);
}

void AudioManager::PlaySoundSample(const char *path, float volume)
{
    if (shutdown)
        return;

    sound_cache_t *sound = GetSoundForName(path);
    if (sound == NULL)
        return;

    sound->sound->setVolume(volume * 100);
    sound->sound->setPitch(GAME_SPEED_SCALE);
    sound->sound->play();
    sounds.push_back(sound);
}

AudioManager::sound_cache_t *AudioManager::GetSoundForName(const char *path)
{
    sf::SoundBuffer *buffer = GET_QHASH(precache, path);

    if (buffer == NULL)
    {
        DBGWARNING("!! Sound not precached:" << path);

        PrecacheSound(path);
        buffer = GET_QHASH(precache, path);

        if (buffer == NULL)
        {
            DBGWARNING("!! Can't load:" << path);
            return NULL;
        }
    }

    buffer = new sf::SoundBuffer(*buffer);

    sf::Sound *sound = new sf::Sound(*buffer);

    sound_cache_t *e = new sound_cache_t;
    e->sound = sound;
    e->name = path;
    e->buffer = buffer;
    return e;
}

void AudioManager::PrecacheSound(const char *path)
{
    if (precache.contains(QString(path)))
    {
        DBGWARNING("!! Sound already precached:" <<path);
        return;
    }

    sf::SoundBuffer *buffer = new sf::SoundBuffer();

    QString fullPath(PATH_SOUND_ROOT);
    fullPath += path;

    if (!buffer->loadFromFile(OSLocalPath(fullPath).toStdString()))
    {
        DBGWARNING("!! Failed to precache: " << path);
        delete buffer;
        return;
    }

    precache.insert(path, buffer);
}

void AudioManager::PlayMusic(const char *path, float volume, bool looped)
{
    lastMusicSettings.volume = volume;
    lastMusicSettings.path = path;
    lastMusicSettings.looped = looped;

    if (!musicEnabled)
        return;

    QString fullPath(PATH_SOUND_ROOT);
    fullPath += "music/";

    if (path != NULL)
        fullPath += path;

    cross_fade_t *fade = new cross_fade_t;

    fade->volume = volume;
    fade->fade = 0.0f;
    fade->fadeout = false;

    fade->music = new sf::Music();
    fade->music->setVolume(0.0f);
    fade->music->setPitch(GAME_SPEED_SCALE);
    fade->music->setLoop(looped);

    if (path == NULL || *path == 0
            || !fade->music->openFromFile(OSLocalPath(fullPath).toStdString()))
    {
        FOREACH_QLIST(music, cross_fade_t*, c)
        {
            c->fadeout = true;
        }
        FOREACH_QLIST_END;

        DBGWARNING("!! Error playing music:" << path);
        delete fade->music;
        delete fade;

        lastMusicSettings.volume = 0.0f;
        return;
    }
    fade->music->play();

    music.push_front(fade);
}

void AudioManager::StopMusic()
{
    FOREACH_QLIST(music, cross_fade_t*, c)
    {
        c->fadeout = true;
    }
    FOREACH_QLIST_END;
}

void AudioManager::UpdateSounds(float frametime)
{
    FOREACH_QLIST(sounds, sound_cache_t*, c)
    {

        if (c->sound->getStatus() == sf::Sound::Stopped)
        {
            delete c->sound;
            delete c->buffer;
            delete c;
            sounds.removeOne(c);
        }
    }
    FOREACH_QLIST_END;
}

void AudioManager::UpdateMusic(float frametime)
{
    bool fadeout = false;

    FOREACH_QLIST(music, cross_fade_t*, c)
    {
        if (fadeout)
            c->fadeout = true;

        float target = c->fadeout ? 0.0f : 1.0f;

        if (c->fade != target)
        {
            c->fade = qapproachlinear(target, c->fade, frametime * 1.5f);

            c->music->setVolume(c->fade * c->volume * 100);
        }
        else if (c->fadeout && c->fade <= 0.0f)
        {
            c->music->stop();
            music.removeOne(c);
            delete c->music;
            delete c;
        }

        fadeout = true;
    }
    FOREACH_QLIST_END;
}
