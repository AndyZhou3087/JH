/********************************************************************
* 
* 音效，音乐播放
*********************************************************************/

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include <iostream>
#include "cocos2d.h"
#include <map>

USING_NS_CC;

class CDSoundEngine;

class SoundManager : public Ref
{
	//音效ID列表
    typedef std::map<int, std::string> SoundIdMapType;
    
public:
	enum SoundId
	{
		SOUND_ID_START,//开始界面
		SOUND_ID_HOME,//家
		SOUND_ID_MAP,//地图界面
		SOUDN_ID_ENTER_MAPADDR,//进入地图地点（家以外）
		SOUND_ID_FIGHT,//战斗
		SOUND_ID_DEATH,//死亡界面
		SOUND_ID_BUTTON,//按钮点击
	};
	
	//soundmanage 实例
    static SoundManager* getInstance();
    
	//初始化
    Ref* init();
    
	//加载声音
    void loadSounds();
	//卸载声音
    void unloadSounds();

	/****************************
	播放背景音乐
	@param filename 背景音乐文件名，Resources目录下(eg:sounds/bg.mp3)
	****************************/
	void playBackMusic(const char * filename);

	/****************************
	停止播放背景音乐
	****************************/
	void stopBackMusic();

	/****************************
	暂停播放背景音乐
	****************************/
	void pauseBackMusic();

	/****************************
	恢复播放背景音乐
	****************************/
	void resumeBackMusic();

	/****************************
	播放音效
	@param soundId 音效ID,SoundId enum中值
	@param isloop 是否循环，默认不循环
	****************************/
	int playSound(int soundId, bool isloop = false);
    
	/****************************
	停止音效
	@param soundId 音效ID,SoundId enum中值
	****************************/
	void stopSound(int soundId);

	/****************************
	停止所有音效
	****************************/
	void stopAllEffectSound();

	/****************************
	暂停所有音效
	****************************/
	void pauseAllEffectSound();

	/****************************
	恢复所有音效
	****************************/
	void resumeAllEffectSound();
    
	/****************************
	设置音效开关
	@param isSoundOn ture:开，false:关
	****************************/
    void setIsSoundOn(bool isSoundOn);

	/****************************
	设置背景音乐开关
	@param isSoundOn ture:开，false:关
	****************************/
	void setIsMusicOn(bool isMusicOn);
    
	/****************************
	获取设备音效大小
	****************************/
    float getVolume();

	/****************************
	设置设备声音大小
	@param volume 声音大小，最大100
	****************************/
    void setVolume(float volume);

	/****************************
	获取音效开关
	@return  false:关，true:开
	****************************/
	bool getSoundIsOn();

	/****************************
	获取背景音乐开关
	@return  false:关，true:开
	****************************/
	bool getMusicPlayed();
private:
	bool                _isSoundOn;//音效开关
	bool				_isMusicOn;//背景音乐开关
	bool				_isMusicPlayed;//背景音乐是否正在播放
    SoundIdMapType      _soundIds;//音效ID
};

#endif
