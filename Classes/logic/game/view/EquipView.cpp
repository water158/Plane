#include "EquipView.h"


EquipView::EquipView()
{
	m_pEquipItemData = NULL;
}

EquipView::~EquipView()
{

}

EquipView* EquipView::create(CraftData* data, EquipItem* item)
{
	EquipView* pNode = new EquipView();
	if(pNode && pNode->init(data, item))
	{
		pNode->autorelease();
		return pNode;
	}

	return NULL;
}

bool EquipView::init(CraftData* data, EquipItem* item_data)
{
	if(!CraftView::init(data))
	{
		return false;
	}

	//设置炮台类型
	this->setType(CRAFT_TYPE_BATTERY);

	m_pEquipItemData = item_data;
    
    string gun_id = item_data->getImgName();
    
    this->time = 0.0f;
    this->m_equipDirection = 0;
    
    dataTargetConon = DataManager::sharedDataManager()->getTargetConnon(gun_id);
    
    if (!dataTargetConon)
    {
        CCLog("get gun %s data failed!",gun_id.c_str());
        
        return false;
    }
    
	return true;
}

void EquipView::update(float dt)
{
    float timeCurrent = time;
    
    if(!this->getIsBeginFly())
    {
        return;
    }
    
    if(this->getIsDied())
    {
        return;
    }
    
    //武器开关未开启就不发射子弹
    if(!this->getGunSwitch())
    {
        return;
    }
    
    CCObject* it;
    CCARRAY_FOREACH(gun_details, it)
    {
        GunDetail* detail = (GunDetail *)it;
        
        if(!detail->getIsBeginFire())
        {
            detail->setIsBeginFire((timeCurrent >= detail->getTimeBeginFire()) ? true : false);
        }
        else
        {
            TargetGunGroupData *dataTargetGG = detail->getTargetGunGroupData();
            
            //设置枪管的伤害值
            dataTargetGG->setHarm(m_pEquipItemData->getAttack());
            //设置腔管的伤害系数
            detail->setHarmRatio(0.03f);
            
//            GunData* dataGun = detail->getGunData();
//            if (time >= detail->getTimeTarget())
//            {
//                //炮发射音效
//                if (dataGun->getIsMissile())
//                {
//                    //SoundEffectManager::shared()->requestPlayPerSoundEffect(EFF_MISSLE);
//                    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_MISSLE, EFF_MISSLE);
//                    SoundEffectManager::shared()->setIsPlayMissionSound(true);
//                }
//                else
//                {
//                    //SoundEffectManager::shared()->requestPlayPerSoundEffect(EFF_NORMAL_BULLET);
//                    SoundEffectManager::shared()->requestPlaySoundEffect(SOUND_NORMAL_BULLET, EFF_NORMAL_BULLET);
//                    SoundEffectManager::shared()->setIsPlayNormalSound(true);
//                }
//            }
            
            shotBullet(dt, detail);
        }
    }
    
    time += dt;
}

void EquipView::changePhrase(int phrase)
{
    if (dataTargetConon)
    {
        this->setPhrase(phrase);
        
        unscheduleUpdate();
        
        this->time = 0.0f;
        
        //清理所有枪管
        gun_details->removeAllObjects();
        //构建新的枪管
        string phrase_id = getStringFromInt(phrase);
        
        PhaseData* dataPhrase = dataTargetConon->getPhaseData(phrase_id);
        
        if (!dataPhrase)
        {
            return;
        }
        
        CCArray* phrase_targetgungroups = NULL;
        
        //处理炮702左右方向问题
        int plane_id = getIntFromS(dataTargetConon->getPlaneID().c_str());
        
        if ((plane_id == 702) && (m_equipDirection == 1))
        {
            CCArray* conditions = dataPhrase->getConditions();
            
            CCObject* obj = NULL;
            CCARRAY_FOREACH(conditions, obj)
            {
                ConditionData* data = (ConditionData*)obj;
                phrase_targetgungroups = data->getTargetGunGroup();
            }
        }
        else
        {
            phrase_targetgungroups = dataPhrase->getTargetGunGroup();
        }

        //准备GunDetail是每次update的时候 用到的数据
        prepareGunDetail(phrase_targetgungroups);
        
        initWeapon();
    }
}

void EquipView::initWeapon()
{
    //开始UPDATE
    this->scheduleUpdate();
//    this->scheduleUpdateWithPriority(0);
}