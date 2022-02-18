#pragma once

#include <xaudio2.h>
#include "gameObject.h"


class Audio : public GameObject
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice = nullptr;
	BYTE*					m_SoundData = nullptr;

	int						m_Length = 0;
	int						m_PlayLength = 0;


public:
	static void InitMaster();
	static void UninitMaster();

	Audio() {};

	void Init() {};
	void Uninit();
	void Update() {};
	void Draw() {};

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void Stop();

	void VolumeSet(float s_v);

};

