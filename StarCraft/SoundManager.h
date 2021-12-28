#pragma once
#include "Singleton.h"
#include "fmod/fmod.hpp"
#pragma comment (lib, "fmodex_vc.lib")
using namespace FMOD;

static const int SOUND_MAX_CHANNEL = 20;
static const float MAX_DELAY_TIME = 0.1f;

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
		float mMaxDelayTime = MAX_DELAY_TIME;

		bool mbIsOnce = false;

		eSoundTag mFriendTag = eSoundTag::None;
	public:
		SoundInfo(Sound* pSound, float volume, bool isOnce = false, eSoundTag friendTag = eSoundTag::None);
		~SoundInfo();
		void Update();

		Sound* GetSound();
		size_t GetSoundSize();
		void AddSound(Sound* pSound);

		inline float GetVolume() { return mVolume; }
		inline bool IsCanPlay() { return mbIsCanPlay; }
		inline void SetCanPlay() { mbIsCanPlay = true; mCurDelayTime = 0.0f; }
		inline bool IsOncePlay() { return mbIsOnce; }
		inline bool IsHaveFirend() { return (mFriendTag != eSoundTag::None); }
		inline eSoundTag GetFriendTag() { return mFriendTag; }
		void Play(float length = 0);
	};

	class SoundClip
	{
	private:
		Channel* mpChannel = nullptr;
		eSoundTag mSoundTag;

	public:
		SoundClip(eSoundTag soundTag, Channel* pChannel, float volume = 1.0f);

		void Stop();
		bool IsEnd();

		inline eSoundTag GetSoundTag() { return mSoundTag; }
	};

	System* mpSystem = nullptr;
	unordered_map<eSoundTag, SoundInfo*> mMapInfo;
	list<SoundClip*> mListClip;

public:
	void Init();
	void Release();
	void Update();

	void LoadFile(eSoundTag tag, string path, float volume, bool isBgm, bool isOnce = false, eSoundTag friendTag = eSoundTag::None);

	void Play(eSoundTag tag);
	void StopAll();
};