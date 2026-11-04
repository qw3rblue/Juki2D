#include <Juki2D/Audio.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
	float volumeValue(float value) {
		if (!std::isfinite(value)) {
			throw std::invalid_argument("Volume must be finite");
		}

		return std::clamp(value, 0.f, 100.f);
	}
}

void Juki2D::Audio::update() {
	std::erase_if(sounds, [](const auto& sound) {
		return sound.sound->getStatus() == sf::SoundSource::Status::Stopped;
	});
}

void Juki2D::Audio::playSound(const std::string& path, float volume) {
	volume = volumeValue(volume);
	update();

	auto it = buffers.find(path);

	if (it == buffers.end()) {
		auto buffer = std::make_unique<sf::SoundBuffer>();

		if (!buffer->loadFromFile(path)) {
			throw std::runtime_error("Failed to load sound: " + path);
		}

		it = buffers.emplace(path, std::move(buffer)).first;
	}

	auto sound = std::make_unique<sf::Sound>(*it->second);

	sound->setVolume(volume * soundVolume / 100.f);
	sound->play();

	sounds.push_back({ std::move(sound), volume });
}

void Juki2D::Audio::playMusic(const std::string& path, bool loop) {
	auto next = std::make_unique<sf::Music>();
	if (!next->openFromFile(path)) {
		throw std::runtime_error("Failed to load music: " + path);
	}
	next->setLooping(loop);
	next->setVolume(musicVolume);
	music = std::move(next);
	music->play();
}

void Juki2D::Audio::pauseMusic() {
	if (music) {
		music->pause();
	}
}

void Juki2D::Audio::resumeMusic() {
	if (music && music->getStatus() == sf::SoundSource::Status::Paused) {
		music->play();
	}
}

void Juki2D::Audio::stopMusic() {
	if (music) {
		music->stop();
	}
}

void Juki2D::Audio::setMusicVolume(float value) {
	musicVolume = volumeValue(value);
	if (music) {
		music->setVolume(musicVolume);
	}
}

void Juki2D::Audio::stopSounds() {
	sounds.clear();
}

void Juki2D::Audio::setSoundVolume(float value) {
	soundVolume = volumeValue(value);

	for (auto& voice : sounds) {
		voice.sound->setVolume(voice.volume * soundVolume / 100.f);
	}
}
