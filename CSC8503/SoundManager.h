#pragma once
#include<fmod.h>
#include <fmod.hpp>
#include<string>
#include<map>

namespace NCL {
	namespace CSC8503 {
        class SoundManager {
        public:
            SoundManager();
            ~SoundManager();

            bool init(); 
            void update(); 

            bool isSoundPlaying(const std::string& name);
            bool loadSound(const std::string& name, const std::string& path, bool loop = false, bool isBGM = false, float minDistance = 1.0, float maxDistance = 70.0f);
            void play3DSound(const std::string& name, const FMOD_VECTOR& position);
            void setListenerPosition(const FMOD_VECTOR& position, const FMOD_VECTOR& velocity, const FMOD_VECTOR& forward, const FMOD_VECTOR& up);
            void playSound(const std::string& name);
            void stopSound(const std::string& name); 
            void setSoundVolume(const std::string& name, float volume);
            void update3DSoundPosition(const std::string& name, const FMOD_VECTOR& newPosition);
            void loadAllBGM(const std::map<std::string, std::string>& bgmPaths);
            void setSoundSpeed(const std::string& name, float speedFactor);
            void ResumeAllSounds();
            void PauseAllSounds();
        private:
            FMOD::System* audioSystem;
            std::map<std::string, FMOD::Sound*> sounds;
            std::map<std::string, std::vector<FMOD::Channel*>> channelLists;
            bool isInitialized;
            bool playSoundInternal(const std::string& name, FMOD::Channel** channel, bool is3D, const FMOD_VECTOR* position = nullptr) {
                if (!isInitialized) return false;
                auto search = sounds.find(name);
                if (search == sounds.end()) {
                    std::cerr << "Sound not found: " << name << std::endl;
                    return false;
                }
                FMOD_RESULT result = audioSystem->playSound(search->second, nullptr, true, channel);
                if (result != FMOD_OK || !(*channel)) {
                    std::cerr << "Failed to play sound: " << name << std::endl;
                    return false;
                }
                if (is3D && position) {
                    (*channel)->set3DAttributes(position, nullptr);
                }
                (*channel)->setPaused(false);
                return true;
            }
        };
	}
}