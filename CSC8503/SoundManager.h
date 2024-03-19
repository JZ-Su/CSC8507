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

            bool loadSound(const std::string& name, const std::string& path, bool loop = false, bool isBGM = false, float minDistance = 0.5, float maxDistance = 70.0f);
            void play3DSound(const std::string& name, const FMOD_VECTOR& position);
            void setListenerPosition(const FMOD_VECTOR& position, const FMOD_VECTOR& velocity, const FMOD_VECTOR& forward, const FMOD_VECTOR& up);
            void playSound(const std::string& name);
            void stopSound(const std::string& name); 
            void setSoundVolume(const std::string& name, float volume);
            void update3DSoundPosition(const std::string& name, const FMOD_VECTOR& newPosition);
            void loadAllBGM(const std::map<std::string, std::string>& bgmPaths);

        private:
            FMOD::System* audioSystem;
            std::map<std::string, FMOD::Sound*> sounds;
            std::map<std::string, FMOD::Channel*> channels;

            bool isInitialized;
        };
	}
}