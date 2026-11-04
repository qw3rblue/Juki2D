#pragma once
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Juki2D {
class Audio {
public:
    void playSound(const std::string& path, float volume = 100.f);
    void playMusic(const std::string& path, bool loop = true);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);
    void stopSounds();
    void update();
private:
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> buffers;
    struct Voice {
        std::unique_ptr<sf::Sound> sound;
        float volume;
    };
    std::vector<Voice> sounds;
    float soundVolume = 100.f;
    std::unique_ptr<sf::Music> music;

    float musicVolume = 100.f;
};
}
