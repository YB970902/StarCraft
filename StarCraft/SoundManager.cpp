#include "stdafx.h"
#include "SoundManager.h"
#include "RandomManager.h"

SoundManager::SoundClip::SoundClip(Channel* pChannel, float volume)
	:mpChannel{ pChannel }
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

SoundManager::SoundInfo::SoundInfo(Sound* pSound, float volume)
	:mVolume{ volume }
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
	if (mCurDelayTime >= MAX_DELAY_TIME)
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

void SoundManager::Init()
{
	System_Create(&mpSystem);
	mpSystem->init(SOUND_MAX_CHANNEL, FMOD_INIT_NORMAL, 0);

	LoadFile(eSoundTag::MarineAttack, "Sounds/Marine/tmafir00.wav", 1.0f, false);
	LoadFile(eSoundTag::MarineDead, "Sounds/Marine/tmadth00.wav", 1.0f, false);
	LoadFile(eSoundTag::MarineDead, "Sounds/Marine/tmadth01.wav", 1.0f, false);
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

void SoundManager::LoadFile(eSoundTag tag, string path, float volume, bool isBgm)
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
			mMapInfo[tag] = new SoundInfo(pSound, volume);
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
	if (mMapInfo[tag]->IsCanPlay() == false) { return; }

	Channel* pChannel;
	mpSystem->playSound(FMOD_CHANNEL_FREE, mMapInfo[tag]->GetSound(), false, &pChannel);
	if (pChannel == nullptr) { return; }

	mMapInfo[tag]->SetIsCanPlay(false);

	mListClip.push_back(new SoundClip(pChannel, mMapInfo[tag]->GetVolume()));
}

void SoundManager::StopAll()
{
	for (auto it = mListClip.begin(); it != mListClip.end();)
	{
		delete (*it);
		it = mListClip.erase(it);
	}
}