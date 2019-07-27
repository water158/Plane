#include "GameConfigLayer.h"
#include "SoundEffectLayer.h"
#include "FerrisParticle.h"

GameConfigLayer::GameConfigLayer(void)
{

}

GameConfigLayer::~GameConfigLayer(void)
{
    m_pUILayer->removeFromParent();
}

GameConfigLayer* GameConfigLayer::create()
{
	GameConfigLayer* layer = new GameConfigLayer();
	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	return NULL;
}

bool GameConfigLayer::init()
{
	CCLayer::init();
 
    m_pUILayer = TouchGroup::create();
    m_pUILayer->retain();
    
    m_pUILayer->setPosition(ccp(- _s_width, 0));
    addChild(m_pUILayer);
    
    m_pBgColorLayer = CCLayerColor::create(ccc4(0, 0, 0, 0),_s_width, _s_height);
    
    
    addChild(m_pBgColorLayer,-1);
    
    Layout* root_configure = static_cast<Layout*>(NodeReader::getInstance()->createNode("ui/cocostudio/setting_layer.ExportJson"));
    m_pUILayer->addWidget(root_configure);
    
    Button* btn_close = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_close"));
    btn_close->addTouchEventListener(this, toucheventselector(GameConfigLayer::menu_close));
    
    //音效按钮1
    btn_sound_on = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_sound_on"));
    btn_sound_on->setTag(101);
    btn_sound_on->setBrightStyle(BRIGHT_NORMAL);
    btn_sound_on->addTouchEventListener(this, toucheventselector(GameConfigLayer::soundSettingCallBack));
    //2
    btn_sound_off = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_sound_off"));
    btn_sound_off->setTag(102);
    btn_sound_off->setBrightStyle(BRIGHT_NORMAL);
    btn_sound_off->addTouchEventListener(this, toucheventselector(GameConfigLayer::soundSettingCallBack));
    
    if (UserData::shared()->getSoundEnable())
    {
        btn_sound_on->setBrightStyle(BRIGHT_HIGHLIGHT);
        btn_sound_off->setBrightStyle(BRIGHT_NORMAL);
    }
    else
    {
        btn_sound_off->setBrightStyle(BRIGHT_HIGHLIGHT);
        btn_sound_on->setBrightStyle(BRIGHT_NORMAL);
    }
    
    //音乐按钮1
    btn_music_on = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_muisc_on"));
    btn_music_on->setTag(101);
    btn_music_on->addTouchEventListener(this, toucheventselector(GameConfigLayer::musicSettingCallBack));
    //2
    btn_music_off = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_muisc_off"));
    btn_music_off->setTag(102);
    btn_music_off->addTouchEventListener(this, toucheventselector(GameConfigLayer::musicSettingCallBack));
    
    if (UserData::shared()->getMusicEnable())
    {
        btn_music_on->setBrightStyle(BRIGHT_HIGHLIGHT);
    }
    else
    {
        btn_music_off->setBrightStyle(BRIGHT_HIGHLIGHT);
    }
    
    //画面设置
    btn_particle_on = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_scene_on"));
    btn_particle_on->setTag(101);
    btn_particle_on->addTouchEventListener(this, toucheventselector(GameConfigLayer::particlesSettingCallBack));
    btn_particle_off = static_cast<Button*>(m_pUILayer->getWidgetByName("btn_scene_off"));
    btn_particle_off->setTag(102);
    btn_particle_off->addTouchEventListener(this, toucheventselector(GameConfigLayer::particlesSettingCallBack));
    
    if (UserData::shared()->getParticleEnable())
    {
        btn_particle_on->setBrightStyle(BRIGHT_HIGHLIGHT);
    }
    else
    {
        btn_particle_off->setBrightStyle(BRIGHT_HIGHLIGHT);
    }
    
    
    m_pUILayer->runAction(CCMoveBy::create(0.5, ccp(_s_width, 0)));
    
    m_pBgColorLayer->runAction(CCFadeTo::create(0.5f, 220.0f));

	return true;
}

void GameConfigLayer::menu_close(CCObject* pSender,TouchEventType type)
{
    
    if (type == TOUCH_EVENT_ENDED)
    {
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        
        CCSequence* seq = CCSequence::create(CCMoveBy::create(0.5f, ccp(- _s_width, 0)),
                                             CCCallFuncN::create(this, callfuncN_selector(GameConfigLayer::actionFinish)),NULL);
        
        m_pBgColorLayer->runAction(CCFadeTo::create(0.5f, 0.0f));
        
        m_pUILayer->runAction(seq);
    }

}

void GameConfigLayer::actionFinish(cocos2d::CCObject *pSender)
{
    GameDelegate::shared()->closeSceneConfig();
}

void GameConfigLayer::particlesSettingCallBack(CCObject* pSender,TouchEventType type)
{
    Button* touchBut = (Button*)pSender;
    int tag = touchBut->getTag();
    
    touchBut->setBrightStyle(BRIGHT_HIGHLIGHT);
    
    if (tag == 101)
    {
        if (!UserData::shared()->getParticleEnable())
        {
            btn_particle_off->setBrightStyle(BRIGHT_NORMAL);
            UserData::shared()->setParticleEnable(true);
        }
    }
    else
    {
        if (UserData::shared()->getParticleEnable())
        {
            btn_particle_on->setBrightStyle(BRIGHT_NORMAL);
            UserData::shared()->setParticleEnable(false);
        }
    }
    
    UserData::shared()->save();
}

void GameConfigLayer::musicSettingCallBack(CCObject* pSender,TouchEventType type)//声音设置回调
{
    Button* touchBut = (Button*)pSender;
    int tag = touchBut->getTag();
    //选中button设置为高亮选中
    touchBut->setBrightStyle(BRIGHT_HIGHLIGHT);
    
    if (tag == 101)
    {
        if (!UserData::shared()->getMusicEnable())
        {
            btn_music_off->setBrightStyle(BRIGHT_NORMAL);
            UserData::shared()->setMusicEnable(true);
        }
        
    }
    else
    {
        if (UserData::shared()->getMusicEnable())
        {
            btn_music_on->setBrightStyle(BRIGHT_NORMAL);
            UserData::shared()->setMusicEnable(false);
        }
        
    }
    
    if(UserData::shared()->getMusicEnable())
    {
        if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            std::string curBgMisName = SoundEffectManager::shared()->getBackgroundMiscFileName();
            if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            {
                SoundEffectManager::shared()->requestPlayBackgroundMisc(curBgMisName.c_str());
            }
            else
            {
                SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
            }
        }
    }
    else
    {
        SoundEffectManager::shared()->stopBackgroundSound();
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    
    UserData::shared()->save();

}

void GameConfigLayer::soundSettingCallBack(CCObject* pSender,TouchEventType type)//音效设置回调
{
    Button* touchBut = (Button*)pSender;
    //选中button设置为高亮选中
    int tag = touchBut->getTag();
    
    touchBut->setBrightStyle(BRIGHT_HIGHLIGHT);
    
    if (tag == 101)
    {
        if (!UserData::shared()->getSoundEnable())
        {
            btn_sound_off->setBrightStyle(BRIGHT_NORMAL);
            UserData::shared()->setSoundEnable(true);
        }
        
    }
    else
    {
        if (UserData::shared()->getSoundEnable())
        {
            btn_sound_on->setBrightStyle(BRIGHT_NORMAL);
            UserData::shared()->setSoundEnable(false);
        }
        
    }

    
    UserData::shared()->save();
}

void GameConfigLayer::effectCallBack()
{
	if(!UserData::shared()->getSoundEnable())
    {
        SoundEffectManager::shared()->stopAllSoundEffect();
    }
}

void GameConfigLayer::musicCallBack()
{
	if(UserData::shared()->getMusicEnable())
    {
        if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            std::string curBgMisName = SoundEffectManager::shared()->getBackgroundMiscFileName();
            if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            {
                SoundEffectManager::shared()->requestPlayBackgroundMisc(curBgMisName.c_str());
            }
            else
            {
                SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
            }
        }
    }
	else
    {
        SoundEffectManager::shared()->stopBackgroundSound();
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
}

void GameConfigLayer::vibrationCallBack()
{
	if(UserData::shared()->getVibraEnable())
	{
        
	}else
	{

	}
}

void GameConfigLayer::keyBackClicked()
{
	menu_close(NULL, TOUCH_EVENT_ENDED);
}

void GameConfigLayer::keyMenuClicked()
{

}

void GameConfigLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch *touch = (CCTouch *)(*it);
	CCPoint location = touch->getLocation();

	CCSprite *sp_sure = (CCSprite *)this->getChildByTag(0);
	CCSprite *sp_sure1 = (CCSprite *)this->getChildByTag(1);
	CCSprite *sp_sure2 = (CCSprite *)this->getChildByTag(2);
	
    float temp_w = (effect_left.getMidX()-effect_left.getMinX())/2;
	if(effect_left.containsPoint(location))
	{
		if(sp_sure)
		{
            sp_sure->setPosition(ccp(effect_left.getMidX()-temp_w,effect_left.getMidY()));
        }
        UserData::shared()->setSoundEnable(true);
        UserData::shared()->save();
        //播放点击声音
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
	}
    else if(effect_right.containsPoint(location))
	{
		if(sp_sure)
        {
			sp_sure->setPosition(ccp(effect_right.getMidX()-temp_w,effect_right.getMidY()));
		}
        UserData::shared()->setSoundEnable(false);
        UserData::shared()->save();
        //播放点击声音
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
	}
    else if(music_left.containsPoint(location))
	{
		if(sp_sure1)
        {
			sp_sure1->setPosition(ccp(music_left.getMidX()-temp_w,music_left.getMidY()));
        }
        //播放点击声音
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        UserData::shared()->setMusicEnable(true);
        UserData::shared()->save();
	}
    else if(music_right.containsPoint(location))
	{
		if(sp_sure1)
		{
            sp_sure1->setPosition(ccp(music_right.getMidX()-temp_w,music_right.getMidY()));
        }
        //播放点击声音
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        UserData::shared()->setMusicEnable(false);
        UserData::shared()->save();
	}
    else if(vibration_left.containsPoint(location))
	{
		if(sp_sure2)
		{
            sp_sure2->setPosition(ccp(vibration_left.getMidX()-temp_w,vibration_left.getMidY()));
        }
        //播放点击声音
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        UserData::shared()->setVibraEnable(true);
        UserData::shared()->save();
	}
    else if(vibration_right.containsPoint(location))
	{
		if(sp_sure2)
		{
           sp_sure2->setPosition(ccp(vibration_right.getMidX()-temp_w,vibration_right.getMidY()));
        }
        //播放点击声音
        SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_CLICK,EFF_CLICK_LIGHT);
        UserData::shared()->setVibraEnable(false);
        UserData::shared()->save();
	}
    //使设置生效
    this->effectCallBack();
    this->musicCallBack();
    this->vibrationCallBack();
}

