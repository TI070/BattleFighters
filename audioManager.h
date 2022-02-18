#pragma once

#include "audio.h"
#include "managerBase.h"
#include <list>

struct AUDIO_DATA {

	Audio* pAudio;
	float volume;
};

enum AUDIO_KIND {

	AK_SE_KETTEI,
	AK_SE_CURSORMOVE,
	AK_SE_CANCEL,
	AK_SE_GAMESTART,

	AK_SE_BLOWSMALL,
	AK_SE_BLOWMIDDLE,
	AK_SE_ZANGEKI,
	AK_SE_GEKIHA,

	AK_SE_YUMI,
	AK_SE_CHARGE,
	AK_SE_SHIELDDAMAGE,


	AK_BGM_TITLE,
	AK_BGM_MENU,
	AK_BGM_BATTLE,
	AK_BGM_RESULT,

	AUDIOKIND_MAX

};


class AudioManager : public ManagerBase
{

private:

	//static Audio* pAudioArray[AUDIOKIND_MAX];
	static AUDIO_DATA audioData[AUDIOKIND_MAX];

public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void AudioPlay(AUDIO_KIND index,bool loop);
	static void AudioStop(AUDIO_KIND index);

};