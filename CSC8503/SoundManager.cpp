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

bool SoundManager::loadSound(const std::string& name, const std::string& path, bool loop, bool isBGM, float minDistance, float maxDistance) {
    if (!isInitialized) return false;

    FMOD_MODE mode = (loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT)|(isBGM ? FMOD_2D : FMOD_3D);
    FMOD::Sound* sound;
    FMOD_RESULT result = audioSystem->createSound(path.c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK) return false;
    if (!isBGM) { 
        FMOD_MODE decayMode = FMOD_3D_LINEARROLLOFF; 
        sound->setMode(decayMode);
        sound->set3DMinMaxDistance(minDistance, maxDistance);
    }
    sounds[name] = sound;
    return true;
}

void SoundManager::playSound(const std::string& name) {
    FMOD::Channel* channel = nullptr;
    if (playSoundInternal(name, &channel, false)) {
        channelLists[name].push_back(channel);
    }
}

void SoundManager::play3DSound(const std::string& name, const FMOD_VECTOR& position) {
    FMOD::Channel* channel = nullptr;
    if (playSoundInternal(name, &channel, true, &position)) {
        channelLists[name].push_back(channel);
    }
}

void SoundManager::stopSound(const std::string& name) {
    if (!isInitialized) return;

    auto search = channelLists.find(name);
    if (search != channelLists.end()) {
        for (FMOD::Channel* channel:search->second) {
            if (channel) {
                channel->stop();
            }
        }
        search->second.clear();
        channelLists.erase(search);
    }
}

void SoundManager::setListenerPosition(const FMOD_VECTOR& position, const FMOD_VECTOR& velocity, const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
    if (!isInitialized) return;
    audioSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

void SoundManager::setSoundVolume(const std::string& name, float volume) {
    if(!isInitialized) return;
    auto channelIter = channelLists.find(name);
    if (channelIter != channelLists.end()) {
        for (FMOD::Channel* channel:channelIter->second) {
            if (channel) {
                channel->setVolume(volume);
            }
        }
    }
}

void SoundManager::update3DSoundPosition(const std::string& name, const FMOD_VECTOR& newPosition) {
    if (!isInitialized) return;

    auto channelIter = channelLists.find(name);
    if (channelIter != channelLists.end()) {
        for (FMOD::Channel* channel : channelIter->second) {
            bool isPlaying = false;
            channel->isPlaying(&isPlaying);
            if (isPlaying) {
                channel->set3DAttributes(&newPosition, nullptr);
            }
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

bool SoundManager::isSoundPlaying(const std::string& name) {
    auto it = channelLists.find(name);
    if (it != channelLists.end()) {
        for (FMOD::Channel* channel : it->second) {
            bool isPlaying = false;
            channel->isPlaying(&isPlaying);
            if (isPlaying) {
                return true; 
            }
        }
    }
    return false; 
}

void SoundManager::setSoundSpeed(const std::string& name, float speedFactor) {
    if (!isInitialized) return;

    auto it = channelLists.find(name);
    if (it != channelLists.end()) {
        for (FMOD::Channel* channel : it->second) {
            if (channel) {
                float originalFrequency;
                channel->getFrequency(&originalFrequency); 
                channel->setFrequency(originalFrequency * speedFactor); 
            }
        }
    }
}