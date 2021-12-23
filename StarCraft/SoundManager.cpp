#include "stdafx.h"
#include "SoundManager.h"
#include "RandomManager.h"

SoundManager::SoundClip::SoundClip(eSoundTag soundTag, Channel* pChannel, float volume)
	: mSoundTag{ soundTag }, mpChannel { pChannel }
{
	mpChannel->setVolume(volume);
}

void SoundManager::SoundClip::Stop()
{
	mpChannel->stop();
}
bool SoundManager::SoundClip::IsEnd()
{
	bool bIsPlaying = true;
	mpChannel->isPlaying(&bIsPlaying);
	return !bIsPlaying;
}

SoundManager::SoundInfo::SoundInfo(Sound* pSound, float volume, bool isOnce, eSoundTag friendTag)
	:mVolume{ volume }, mbIsOnce{ isOnce }, mFriendTag{ friendTag }
{
	mVecSound.push_back(pSound);
}

SoundManager::SoundInfo::~SoundInfo()
{
	Sound* pSound = nullptr;
	for (auto it = mVecSound.begin(); it != mVecSound.end();)
	{
		pSound = (*it);
		pSound->release();
		it = mVecSound.erase(it);
	}
}

void SoundManager::SoundInfo::Update()
{
	if (mbIsCanPlay) { return; }

	mCurDelayTime += (float)DELTA_TIME;
	if (mCurDelayTime >= mMaxDelayTime)
	{
		mCurDelayTime = 0.0f;
		mbIsCanPlay = true;
	}
}

Sound* SoundManager::SoundInfo::GetSound()
{
	return mVecSound[RANDOM->GetValue(0, GetSoundSize() - 1)];
}

size_t SoundManager::SoundInfo::GetSoundSize()
{
	return mVecSound.size();
}

void SoundManager::SoundInfo::AddSound(Sound* pSound)
{
	mVecSound.push_back(pSound);
}

void SoundManager::SoundInfo::Play(float length)
{
	mbIsCanPlay = false;

	if (mbIsOnce)
	{
		mMaxDelayTime = length;
	}
}

void SoundManager::Init()
{
	System_Create(&mpSystem);
	mpSystem->init(SOUND_MAX_CHANNEL, FMOD_INIT_NORMAL, 0);

	LoadFile(eSoundTag::MarineAttack, "Sounds/Marine/tmafir00.wav", 1.0f, false);

	LoadFile(eSoundTag::MarineDead, "Sounds/Marine/tmadth00.wav", 1.0f, false);
	LoadFile(eSoundTag::MarineDead, "Sounds/Marine/tmadth01.wav", 1.0f, false);

	LoadFile(eSoundTag::MarineMove, "Sounds/Marine/tmayes00.wav", 1.0f, false, true, eSoundTag::MarineClick);
	LoadFile(eSoundTag::MarineMove, "Sounds/Marine/tmayes01.wav", 1.0f, false, true, eSoundTag::MarineClick);
	LoadFile(eSoundTag::MarineMove, "Sounds/Marine/tmayes02.wav", 1.0f, false, true, eSoundTag::MarineClick);
	LoadFile(eSoundTag::MarineMove, "Sounds/Marine/tmayes03.wav", 1.0f, false, true, eSoundTag::MarineClick);

	LoadFile(eSoundTag::MarineClick, "Sounds/Marine/tmawht00.wav", 1.0f, false, true, eSoundTag::MarineMove);
	LoadFile(eSoundTag::MarineClick, "Sounds/Marine/tmawht01.wav", 1.0f, false, true, eSoundTag::MarineMove);
	LoadFile(eSoundTag::MarineClick, "Sounds/Marine/tmawht02.wav", 1.0f, false, true, eSoundTag::MarineMove);
	LoadFile(eSoundTag::MarineClick, "Sounds/Marine/tmawht03.wav", 1.0f, false, true, eSoundTag::MarineMove);

	LoadFile(eSoundTag::GoliathAttack, "Sounds/Goliath/tgofir00.wav", 1.0f, false);

	LoadFile(eSoundTag::GoliathDead, "Sounds/Goliath/tgodth00.wav", 1.0f, false);

	LoadFile(eSoundTag::GoliathMove, "Sounds/Goliath/tgoyes00.wav", 1.0f, false, true, eSoundTag::GoliathClick);
	LoadFile(eSoundTag::GoliathMove, "Sounds/Goliath/tgoyes01.wav", 1.0f, false, true, eSoundTag::GoliathClick);
	LoadFile(eSoundTag::GoliathMove, "Sounds/Goliath/tgoyes02.wav", 1.0f, false, true, eSoundTag::GoliathClick);
	LoadFile(eSoundTag::GoliathMove, "Sounds/Goliath/tgoyes03.wav", 1.0f, false, true, eSoundTag::GoliathClick);

	LoadFile(eSoundTag::GoliathClick, "Sounds/Goliath/tgowht00.wav", 1.0f, false, true, eSoundTag::GoliathMove);
	LoadFile(eSoundTag::GoliathClick, "Sounds/Goliath/tgowht01.wav", 1.0f, false, true, eSoundTag::GoliathMove);
	LoadFile(eSoundTag::GoliathClick, "Sounds/Goliath/tgowht02.wav", 1.0f, false, true, eSoundTag::GoliathMove);
	LoadFile(eSoundTag::GoliathClick, "Sounds/Goliath/tgowht03.wav", 1.0f, false, true, eSoundTag::GoliathMove);

	LoadFile(eSoundTag::TerranTheme, "Musics/terran1.wav", 0.5f, true, true);
	LoadFile(eSoundTag::TerranTheme, "Musics/terran2.wav", 0.5f, true, true);
	LoadFile(eSoundTag::TerranTheme, "Musics/terran3.wav", 0.5f, true, true);


	LoadFile(eSoundTag::TitleBackground, "Musics/title.wav", 0.5f, true, true);
}

void SoundManager::Release()
{
	StopAll();

	for (auto it = mMapInfo.begin(); it != mMapInfo.end();)
	{
		delete it->second;
		it = mMapInfo.erase(it);
	}

	mpSystem->release();
	mpSystem->close();
}

void SoundManager::Update()
{
	mpSystem->update();

	for (auto it = mMapInfo.begin(); it != mMapInfo.end(); ++it)
	{
		it->second->Update();
	}

	for (auto it = mListClip.begin(); it != mListClip.end();)
	{
		if ((*it)->IsEnd())
		{
			delete (*it);
			it = mListClip.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SoundManager::LoadFile(eSoundTag tag, string path, float volume, bool isBgm, bool isOnce, eSoundTag friendTag)
{
	auto pSoundInfo = mMapInfo.find(tag);

	Sound* pSound = nullptr;

	if (isBgm)
	{
		mpSystem->createStream(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &pSound);
	}
	else
	{
		mpSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &pSound);
	}

	if (pSoundInfo == mMapInfo.end())
	{
		if (pSound)
		{
			mMapInfo[tag] = new SoundInfo(pSound, volume, isOnce, friendTag);
		}
	}
	else
	{
		if (pSound)
		{
			mMapInfo[tag]->AddSound(pSound);
		}
	}
}

void SoundManager::Play(eSoundTag tag)
{
	if (mMapInfo[tag]->IsHaveFirend())
	{
		if (mMapInfo[mMapInfo[tag]->GetFriendTag()]->IsCanPlay() == false ||
			mMapInfo[tag]->IsCanPlay() == false)
		{
			return;
		}
	}
	else
	{
		if (mMapInfo[tag]->IsCanPlay() == false) { return; }
	}

	Channel* pChannel;
	Sound* pSound = mMapInfo[tag]->GetSound();
	mpSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
	if (pChannel == nullptr) { return; }

	if (mMapInfo[tag]->IsOncePlay())
	{
		unsigned int len = 0;
		pSound->getLength(&len, FMOD_TIMEUNIT_MS);
		mMapInfo[tag]->Play((float)len * 0.001f);
	}
	else
	{
		mMapInfo[tag]->Play();
	}

	mListClip.push_back(new SoundClip(tag, pChannel, mMapInfo[tag]->GetVolume()));
}

void SoundManager::StopAll()
{
	for (auto it = mListClip.begin(); it != mListClip.end();)
	{
		(*it)->Stop();
		mMapInfo[(*it)->GetSoundTag()]->SetCanPlay();
		delete (*it);
		it = mListClip.erase(it);
	}
}