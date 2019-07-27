#include "MyAnimationManager.h"
DECLARE_SINGLETON_MEMBER(AnimationManager);

AnimationManager::AnimationManager()
{
	aniList = NULL;
}
AnimationManager::~AnimationManager()
{
	if(aniList)
	{
		delete aniList;
		aniList = NULL;
	}
}
void AnimationManager::getAnimationByXML(const char* xmlfile)
{
    if(!xmlfile)
        return;
    if(aniList)
    {
        delete aniList;
        aniList = NULL;
    }
    //从xml里读取所需要的文字
    if(!aniList)
    {
        aniList = new TiXmlDocument();
        unsigned long nLength = 0;
        char* pBuff = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(xmlfile).c_str(),"rb", &nLength );
        aniList->LoadMemory( pBuff, nLength );
    }
    
    //获得根元素
    TiXmlElement *rootelement = aniList->RootElement();
    TiXmlElement *animationlist = rootelement->FirstChildElement();
    TiXmlElement *animation = animationlist->FirstChildElement();
    //获取动画数量
    const char *paninum = animationlist->Attribute("num");
    int inum = atoi(paninum);
    char name1[200];
    for(int num=0;num<inum;num++)
    {
        //获取动画plist文件路径
        const char *panipath = animation->FirstChild()->Value();
        
        //将图片添加到缓存
        CCSpriteFrameCache *pcache = CCSpriteFrameCache::sharedSpriteFrameCache();
        pcache->addSpriteFramesWithFile(panipath);
        //获取动画帧数和缓存中的标记
        const char *pframenum = animation->Attribute("framenum");
        const char *ptag = animation->Attribute("tag");
        int iframenum = atoi(pframenum);
        CCArray *array1 = CCArray::create();
        
        for (int i=1;i<=iframenum;i++)
        {
            std::string aniname = ptag;
            std::string format = "_%d.png";
            aniname += format;
            sprintf(name1, aniname.c_str(), i);
            CCSpriteFrame* pframe = pcache->spriteFrameByName(name1);
            array1->addObject(pframe);
        }
        //根据数组创建动画并添加到动画缓存
        float delay_time = 0.1f;
        if(num == 0)
            delay_time = 0.05f;
        CCAnimation *panimation = CCAnimation::createWithSpriteFrames(array1,delay_time);
        CCAnimationCache::sharedAnimationCache()->addAnimation(panimation,ptag);
        
        CCLog("Animation %s is Cached.", ptag);
        
        if(num < inum - 1)
            animation = animation->NextSiblingElement();
    }

}

CCSpriteFrame* AnimationManager::getAniFrameByIndex(const char* anitag,unsigned int index)
{
	if(!anitag || index < 0)
		return NULL;
	char temp[200];
	int nindex = index+1;
	std::string pre = anitag;
	std::string last = pre+"_%d.png";
	sprintf(temp,last.c_str(),nindex);
	CCSpriteFrame *tempframe = (CCSpriteFrame *)CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
	if(tempframe)
		return tempframe;
	return NULL;
}

void AnimationManager::preLoadResByID(TiXmlElement *xml_ele)
{
	if(!xml_ele)
	{
		CCLOG("----xml_ele---is----null--");
		return;
	}
	TiXmlElement *ele_ani_type = xml_ele;
	char name1[200];
	TiXmlElement *ele_second = ele_ani_type->FirstChildElement();

	while(ele_second)
	{
		//获取动画plist文件路径
		const char *panipath = ele_second->FirstChild()->Value();

		//将图片添加到缓存
		CCSpriteFrameCache *pcache = CCSpriteFrameCache::sharedSpriteFrameCache();
		pcache->addSpriteFramesWithFile(panipath);
		//获取动画帧数和缓存中的标记
		const char *pframenum = ele_second->Attribute("framenum");
		const char *ptag = ele_second->Attribute("tag");
		int iframenum = atoi(pframenum);
		CCArray *array1 = CCArray::create();

		for (int i=1;i<=iframenum;i++)
		{
			std::string aniname = ptag;
			std::string format = "_%d.png";
			aniname += format;
			sprintf(name1, aniname.c_str(), i);
			CCSpriteFrame* pframe = pcache->spriteFrameByName(name1);
			array1->addObject(pframe);
		}
		//根据数组创建动画并添加到动画缓存
		float delay_time = 0.1f;
		CCAnimation *panimation = CCAnimation::createWithSpriteFrames(array1,delay_time);
		CCAnimationCache::sharedAnimationCache()->addAnimation(panimation,ptag);

		//CCLOG("---ani---name-===--%s--",ptag);
		ele_second = ele_second->NextSiblingElement();
	}

}

void AnimationManager::freeAnimation(const char* ani_name)
{
	if(!ani_name)
	{
		cocos2d::CCMessageBox("class AnimationManager     ani_name is null","");
		return;
	}
	CCAnimationCache::sharedAnimationCache()->removeAnimationByName(ani_name);
}
void AnimationManager::freeAllAnimations()
{
	CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
}