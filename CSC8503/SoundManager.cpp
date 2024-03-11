#include"SoundManager.h"

using namespace NCL;
using namespace CSC8503;

SoundManager::SoundManager() : audioSystem(nullptr), isInitialized(false) {}

SoundManager::~SoundManager() {
    if (isInitialized) {
        for (auto& sound : sounds) {
            sound.second->release();
        }
        audioSystem->close();
        audioSystem->release();
    }
}

bool SoundManager::init() {
    FMOD_RESULT result;
    result = FMOD::System_Create(&audioSystem);
    if (result != FMOD_OK) return false;

    result = audioSystem->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) return false;

    isInitialized = true;
    return true;
}

void SoundManager::update() {
    if (isInitialized) {
        audioSystem->update();
    }
}

bool SoundManager::loadSound(const std::string& name, const std::string& path, bool loop, bool isBGM) {
    if (!isInitialized) return false;

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    mode |= isBGM ? FMOD_2D : FMOD_3D;
    FMOD::Sound* sound;
    FMOD_RESULT result = audioSystem->createSound(path.c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK) return false;

    sounds[name] = sound;
    return true;
}

void SoundManager::playSound(const std::string& name) {
    if (!isInitialized) return;

    auto search = sounds.find(name);
    if (search != sounds.end()) {
        FMOD::Channel* channel = channels[name];
        auto channelIter = channels.find(name);
        bool isPlaying = false;
        if (channelIter != channels.end()) {
            channel = channelIter->second;
            if (channel) {
                channel->isPlaying(&isPlaying);
            }
        }
        if (!isPlaying) {
            FMOD_RESULT result = audioSystem->playSound(search->second, nullptr, false, &channel);
            if (result == FMOD_OK && channel) {
                channels[name] = channel;
            }
            else {
                std::cerr << "Failed to play sound: " << name << std::endl;
            }
        }
    }
}

void SoundManager::stopSound(const std::string& name) {
    if (!isInitialized) return;

    auto search = channels.find(name);
    if (search != channels.end()) {
        search->second->stop(); 
        channels.erase(search); 
    }
}

void SoundManager::play3DSound(const std::string& name, const FMOD_VECTOR& position) {
    if (!isInitialized) return;
    auto search = sounds.find(name);
    if (search != sounds.end()) {
        FMOD::Channel* channel = nullptr;
        auto channelIter = channels.find(name);
        FMOD_RESULT result = audioSystem->playSound(search->second, nullptr, true, &channel);
        if (result != FMOD_OK) {
            return;
        }
        if (channel) {
            channel->set3DAttributes(&position, nullptr);
            channel->setPaused(false);
            channels[name] = channel;
        }
    }
}

void SoundManager::setListenerPosition(const FMOD_VECTOR& position, const FMOD_VECTOR& velocity, const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
    if (!isInitialized) return;
    audioSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

void SoundManager::setSoundVolume(const std::string& name, float volume) {
    if(!isInitialized) return;
    auto channelIter = channels.find(name);
    if (channelIter != channels.end()) {
        FMOD::Channel* channel = channelIter->second;
        channel->setVolume(volume);
    }
}

void SoundManager::update3DSoundPosition(const std::string& name, const FMOD_VECTOR& newPosition) {
    if (!isInitialized) return;

    auto channelIter = channels.find(name);
    if (channelIter != channels.end()) {
        FMOD::Channel* channel = channelIter->second;
        if (channel) {
            channel->set3DAttributes(&newPosition, nullptr);
        }
    }
}

void SoundManager::loadAllBGM(const std::map<std::string, std::string>& bgmPaths) {
    for (const auto& pair : bgmPaths) {
        const std::string& name = pair.first;
        const std::string& path = pair.second;
        loadSound(name, path, true, true);
    }
}
