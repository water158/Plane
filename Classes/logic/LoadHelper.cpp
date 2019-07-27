#include "LoadHelper.h"



//根据条件加载所需要的资源
void LoadHelper::preloadAllRes(int n_hero_id, int n_wing_id, int n_mode, int n_properytype, int n_lv, bool bLoad)
{
	TiXmlDocument *xml_doc = new TiXmlDocument();
	unsigned long nLength = 0;
	char* pBuff = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("common/lv_ani_list.xml").c_str(),"rb", &nLength );
	xml_doc->LoadMemory( pBuff, nLength );

	TiXmlElement *ele_root = xml_doc->RootElement();
	TiXmlElement *ele_anitype1 = ele_root->FirstChildElement();
	TiXmlElement *ele_anitype2 = ele_anitype1->NextSiblingElement();
	TiXmlElement *ele_anitype3 = ele_anitype2->NextSiblingElement();
	TiXmlElement *ele_anitype4 = ele_anitype3->NextSiblingElement();
	TiXmlElement *ele_anitype5 = ele_anitype4->NextSiblingElement();


	TiXmlElement *ele_hero = ele_anitype1->FirstChildElement();
	TiXmlElement *ele_wing = ele_anitype2->FirstChildElement();
	TiXmlElement *ele_mode = ele_anitype3->FirstChildElement();
	TiXmlElement *ele_propery = ele_anitype4->FirstChildElement();
	TiXmlElement *ele_allhero = ele_anitype5->FirstChildElement();

	//LoadHelper::preloadEnemyResByLv(n_lv);
	for (int i=0;i<n_hero_id;i++)
	{
		ele_hero = ele_hero->NextSiblingElement();
	}
	for (int i=0;i<n_wing_id;i++)
	{
		ele_wing = ele_wing->NextSiblingElement();
	}
	for (int i=0;i<n_mode;i++)
	{
		ele_mode = ele_mode->NextSiblingElement();
	}
	for (int i=0;i<n_properytype;i++)
	{
		ele_propery = ele_propery->NextSiblingElement();
	}

	if(bLoad)
	{
        sAnimationManager->preLoadResByID(ele_hero);
        sAnimationManager->preLoadResByID(ele_wing);
        sAnimationManager->preLoadResByID(ele_mode);
        sAnimationManager->preLoadResByID(ele_propery);
		sAnimationManager->preLoadResByID(ele_allhero);
	}

	delete xml_doc;
	xml_doc = NULL;

	sAnimationManager->getAnimationByXML("common/public_ani_list.xml");
}

//读取关卡资源，获取所需要的敌人飞机id
EnemyVec LoadHelper::getNeedEnemyVecByLv(int n_lv)
{
	int n_scene_id = n_lv/10+1;
	int n_level_id = n_lv%10+1;
	std::vector<int> *enemy_vector = new std::vector<int>;
	TiXmlDocument *ele_tdoc = new TiXmlDocument();
	unsigned long nLength = 0;
	
	CCString* strMissionDataFilename = CCString::createWithFormat("game/level/level_%03d.xml",
		11);
	char* pBuff = (char *)CCFileUtils::sharedFileUtils()->getFileData(strMissionDataFilename->getCString(), "rt", &nLength);
	ele_tdoc->LoadMemory(pBuff, nLength);

	TiXmlElement *ele_root = ele_tdoc->RootElement();
	TiXmlElement *ele_groupbullet= ele_root->FirstChildElement();
	TiXmlElement *ele_targetplane= ele_groupbullet->FirstChildElement();

	while(ele_targetplane)
	{
		const char *c_plane_id = ele_targetplane->Attribute("plane_id");
		enemy_vector->push_back(getIntFromS(c_plane_id));

		ele_targetplane = ele_targetplane->NextSiblingElement();
	}
	delete ele_tdoc;
	ele_tdoc = NULL;

	return enemy_vector;
}

//根据飞机ID加载资源
void LoadHelper::preloadCraftResByCraftID(int data_type, int craft_id)
{
	string strPath = "";
	string strName = "";
	//主机数据
	if(data_type == CRAFT_DATA_TYPE_HERO)
	{
		strName = "hero";
	}
	//敌机数据
	else if(data_type == CRAFT_DATA_TYPE_ENEMY)
	{
		strName = "enemy";
	}

	strPath.append("game/")
		.append(strName)
		.append("/")
		.append(getStringFromInt(craft_id))
		.append("/")
		.append(strName)
		.append("_")
		.append(getStringFromInt(craft_id))
		.append(".plist");

	//将图片添加到缓存
	CCSpriteFrameCache *pcache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pcache->addSpriteFramesWithFile(strPath.c_str());
}

//清空所有加载的资源（动画帧和动画）
void LoadHelper::clearAllRes()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
	CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
}