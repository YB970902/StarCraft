#pragma once
#include "Singleton.h"
#include "fmod/fmod.hpp"
#pragma comment (lib, "fmodex_vc.lib")
using namespace FMOD;

static const int SOUND_MAX_CHANNEL = 20;
static const float MAX_DELAY_TIME = 0.1f;

#define SOUND SoundManager::GetInstance()

class SoundManager : public Singleton<SoundManager>
{
private:
	class SoundInfo
	{
	private:
		vector<Sound*> mVecSound;
		float mVolume = 1.0f;

		bool mbIsCanPlay = true;
		float mCurDelayTime = 0.0f;

	public:
		SoundInfo(Sound* pSound, float volume);
		~SoundInfo();
		void Update();

		Sound* GetSound();
		size_t GetSoundSize();
		void AddSound(Sound* pSound);

		inline float GetVolume() { return mVolume; }
		inline bool IsCanPlay() { return mbIsCanPlay; }
		inline void SetIsCanPlay(bool set) { mbIsCanPlay = set; }
	};

	class SoundClip
	{
	private:
		Channel* mpChannel = nullptr;

	public:
		SoundClip(Channel* pChannel, float volume = 1.0f);

		void Stop();
		bool IsEnd();
	};

	System* mpSystem = nullptr;
	unordered_map<eSoundTag, SoundInfo*> mMapInfo;
	list<SoundClip*> mListClip;

public:
	void Init();
	void Release();
	void Update();

	void LoadFile(eSoundTag tag, string path, float volume, bool isBgm);

	void Play(eSoundTag tag);
	void StopAll();
};