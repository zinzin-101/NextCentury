#pragma once

#include <SDL_mixer.h>
#include <string>
#include <map>
#include <vector>


/// @brief This class provides an interface to play a sound effect
class SoundEffect {
public:
	friend class AudioEngine;

	/// @brief Plays the sound effect, but only if it's not already playing.
	/// @param loop The number of times to repeat (0 = play once, 1 = play twice, etc.)
	void play(int loop = 0);

private:
	Mix_Chunk* m_chunk = nullptr;
	int        m_channel = -1;
	// m_channel holds the channel index returned by the last Mix_PlayChannel call.
	// If m_channel >= 0 and Mix_Playing(m_channel) != 0, we won’t play again.
};

/// @brief This class provides an interface to play a music
class Music {
public:
	friend class AudioEngine;

	/// @brief This function plays the music
	/// @param loop is the number of times to repeat the music
	void play(int loop = 0);

	/// @brief This function pauses the music
	void pause();
	
	/// @brief This function stops the music
	void stop();

	/// @brief This function resumes the paused music
	void resume();
private:
	Mix_Music* m_music = nullptr;
};


/// @brief this struct contains the data required for the sound effect
struct SoundEffectObj {
	std::string fileName;
	SoundEffect soundEffect;
};

/// @brief this struct contains the data required for the music
struct MusicObj {
	std::string fileName;
	Music music;
};


/// @brief This class is the driver for playing audio file
class AudioEngine {
public:
	AudioEngine();
	~AudioEngine();

	/// @brief This function initializes the AudioEngine
	/// @param effectFolderPath is the folder path containing the sound effects
	/// @param musicFolderPath is the folder path containing the musics
	void init(const std::string& effectFolderPath, const std::string& musicFolderPath);
	
	/// @brief This function destroys the AudioEngine object
	void destroy();

	/// @brief This function loads the sound effect from file path
	/// @param filePath is the folder path to the sound effect
	/// @returns the SoundEffect object
	SoundEffect loadSoundEffect(const std::string& filePath);

	/// @brief This function plays the sound effect by index
	/// @param index is the index of the sound effect
	/// @param loop is the number of times to repeat the sound effect
	void playSoundEffectByIndex(int index, int loop = 0);

	/// @brief This function plays the sound effect by its name
	/// @param fileName is the name of the sound effect
	/// @param loop is the number of times to repeat the sound effect
	void playSoundEffectByName(const std::string& fileName, int loop = 0);

	/// @brief This function loads the music from file path
	/// @param filePath is the folder path to the music
	/// @returns the Music object
	Music loadMusic(const std::string& filePath);

	/// @brief This function plays the music by index
	/// @param index is the index of the music
	/// @param loop is the number of times to repeat the music
	void playMusicByIndex(int index, int loop = 0);

	/// @brief This function plays the music by its name
	/// @param fileName is the name of the music
	/// @param loop is the number of times to repeat the music
	void playMusicByName(const std::string& fileName, int loop = 0);

	/// @brief This function stops the current music
	void stopMusic();

	/// @brief This function adjusts the volume of the sound effect
	/// @param amount is the to adjust volume
	void adjustSoundEffectVolume(int amount);

	/// @brief This function adjusts the volume of the music
	/// @param amount is the to adjust volume
	void adjustMusicVolume(int amount);

	/// @brief This function sets the volume of the sound effect
	/// @param value to set the volume
	void setSoundEffectVolume(int value);

	/// @brief This function sets the volume of the music
	/// @param value to set the volume
	void setMusicVolume(int value);

	/// @brief This function saves the volume config to a text file
	void saveVolumeToFile();

	/// @brief This function prints the current volume of the music and the sound effect
	void printVolume();

	void stopAllSoundEffects();
private:
	bool isInit = false;
	int currentSoundEffectVolume;
	int currentMusicVolume;

	std::vector<SoundEffectObj> v_soundEffect;
	std::vector<MusicObj> v_music;

	std::map<std::string, Mix_Chunk*> m_effectMap;
	std::map<std::string, Mix_Music*> m_musicMap;

	void loadSoundEffectFromDir(const std::string& dir);
	void loadMusicFromDir(const std::string& dir);
};