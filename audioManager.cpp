
#include "audioManager.h"

//Audio* AudioManager::pAudioArray[AUDIOKIND_MAX];
AUDIO_DATA AudioManager::audioData[AUDIOKIND_MAX];

void AudioManager::Init() {

	for (int i = 0; i < AUDIOKIND_MAX; i++) {
		audioData[i].pAudio = new Audio;
		audioData[i].volume = 1.0f;
	}

	audioData[AK_SE_KETTEI].pAudio->Load("asset/sound/se/kettei.wav");
	audioData[AK_SE_CURSORMOVE].pAudio->Load("asset/sound/se/cursormove.wav");
	audioData[AK_SE_CURSORMOVE].volume = 0.1f;
	audioData[AK_SE_CANCEL].pAudio->Load("asset/sound/se/cancel.wav");
	audioData[AK_SE_GAMESTART].pAudio->Load("asset/sound/se/gameStart.wav");
	audioData[AK_SE_GAMESTART].volume = 0.1f;

	audioData[AK_SE_BLOWSMALL].pAudio->Load("asset/sound/se/blowSmall.wav");
	audioData[AK_SE_BLOWMIDDLE].pAudio->Load("asset/sound/se/blowMiddle.wav");
	audioData[AK_SE_ZANGEKI].pAudio->Load("asset/sound/se/zangeki.wav");
	audioData[AK_SE_GEKIHA].pAudio->Load("asset/sound/se/gekiha.wav");

	audioData[AK_SE_YUMI].pAudio->Load("asset/sound/se/yumi.wav");
	audioData[AK_SE_CHARGE].pAudio->Load("asset/sound/se/charge.wav");

	audioData[AK_SE_SHIELDDAMAGE].pAudio->Load("asset/sound/se/shieldDamage.wav");


	audioData[AK_BGM_TITLE].pAudio->Load("asset/sound/bgm/title.wav");
	audioData[AK_BGM_TITLE].volume = 0.05f;

	audioData[AK_BGM_MENU].pAudio->Load("asset/sound/bgm/Electric_Shine.wav");
	audioData[AK_BGM_MENU].volume = 0.05f;
	
	audioData[AK_BGM_BATTLE].pAudio->Load("asset/sound/bgm/Sparkle.wav");
	audioData[AK_BGM_BATTLE].volume = 0.05f;

	audioData[AK_BGM_RESULT].pAudio->Load("asset/sound/bgm/Direction_of_the_power.wav");
	audioData[AK_BGM_RESULT].volume = 0.05f;

}

void AudioManager::Uninit() {

}

void AudioManager::Update(){

}

void AudioManager::Draw() {


}

void AudioManager::AudioPlay(AUDIO_KIND index, bool loop) {

	//‰¹—Ê’²®
	audioData[index].pAudio->VolumeSet(audioData[index].volume);

	audioData[index].pAudio->Play(loop);

}

void AudioManager::AudioStop(AUDIO_KIND index) {

	audioData[index].pAudio->Stop();

}