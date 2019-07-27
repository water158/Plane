#include "GameHelpLayer.h"

GameHelpLayer::GameHelpLayer(void)
{

}

GameHelpLayer::~GameHelpLayer(void)
{

}

bool GameHelpLayer::init()
{
	CCLog("------GameHelpLayer::init----------1-----");
	CCLayer::init();
    
    m_labelArr = CCArray::create();
    
    //在容器层创建scrollView的实例
    tScrollView = CCScrollView::create();
    //设置滚屏属性
    tScrollView->setDirection(kCCScrollViewDirectionVertical);
    
    CCLayer* containLayer = CCLayer::create();
    
    tScrollView->setContainer(containLayer);
    
    
	CCSprite *con_back = CCSprite::create("ui/panel/panel_common_bg1.png");
	con_back->setPosition(ccp(_s_width/2,_s_height/2));
	this->addChild(con_back,0);
	con_back->setScale(2.0f);

	CCSprite *sp_instro = CCSprite::create("ui/panel/panel_common_bg5.png");
	sp_instro->setPosition(ccp(_s_width/2,_s_height/7*4 - 40.0f));
	this->addChild(sp_instro,1);
    
    //设置滑动层的大小,高度为原框的3倍
    containLayer->setContentSize(CCSize(sp_instro->getContentSize().width,sp_instro->getContentSize().height*5.6f));
    //containLayer->setAnchorPoint(ccp(0,0));
    containLayer->setPosition(ccp(0, 0));
    //设置显示的区域
    tScrollView->setViewSize(CCSize(sp_instro->getContentSize().width,sp_instro->getContentSize().height * 0.85f));
    tScrollView->setContentOffset(tScrollView->minContainerOffset());
    //设置华东区域的实际大小
    tScrollView->setContentSize(containLayer->getContentSize());
    //设置滑动属性
    tScrollView->setTouchEnabled(true);
    tScrollView->setDelegate(this);
    //4.85f
    tScrollView->setPosition(ccp(_s_width/2 - sp_instro->getContentSize().width/2,_s_height/3.4f - 40.0f));
    addChild(tScrollView,8);
    
    //定义label的宽度
    float labelWidth = sp_instro->getContentSize().width * 0.85f;
    
    
    //游戏模式问题
    CCString *s_mode_q = StringLibrary::shared()->getStringByKey("help_mode_q");
	CCLabelTTF *l_mode_q = CCLabelTTF::create(s_mode_q->getCString(),"Arial",32);
    l_mode_q->setDimensions(CCSizeMake(labelWidth,l_mode_q->getContentSize().height));
    m_labelArr->addObject(l_mode_q);
    
    //定义label的高度 每单位
    float labelHeight = l_mode_q->getContentSize().height;
    //游戏模式答案
    CCString *s_mode_a = StringLibrary::shared()->getStringByKey("help_mode_a");
	CCLabelTTF *l_mode_a = CCLabelTTF::create(s_mode_a->getCString(),"Arial",32);
    l_mode_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 6));
    m_labelArr->addObject(l_mode_a);
    
    
    //游戏道具有哪些
    CCString *s_tool_q = StringLibrary::shared()->getStringByKey("help_tool_q");
	CCLabelTTF *l_tool_q = CCLabelTTF::create(s_tool_q->getCString(),"Arial",32);
    l_tool_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_tool_q);
    //游戏道具答案
    CCString *s_tool_a = StringLibrary::shared()->getStringByKey("help_tool_a");
	CCLabelTTF *l_tool_a = CCLabelTTF::create(s_tool_a->getCString(),"Arial",32);
    l_tool_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 12));
    m_labelArr->addObject(l_tool_a);
    
    //如何操作
    CCString *s_howControl_q = StringLibrary::shared()->getStringByKey("help_control_q");
	CCLabelTTF *l_howControl_q = CCLabelTTF::create(s_howControl_q->getCString(),"Arial",32);
    l_howControl_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_howControl_q);
    //操作解答
    CCString *s_howControl_a = StringLibrary::shared()->getStringByKey("help_control_a");
	CCLabelTTF *l_howControl_a = CCLabelTTF::create(s_howControl_a->getCString(),"Arial",32);
    l_howControl_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 3));
    m_labelArr->addObject(l_howControl_a);
    
    //如何获得金币
    CCString *s_getGod_q = StringLibrary::shared()->getStringByKey("help_gold_q");
	CCLabelTTF *l_getGod_q = CCLabelTTF::create(s_getGod_q->getCString(),"Arial",32);
    l_getGod_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_getGod_q);
    //获得金币解答
    CCString *s_getGod_a = StringLibrary::shared()->getStringByKey("help_gold_a");
	CCLabelTTF *l_getyGod_a = CCLabelTTF::create(s_getGod_a->getCString(),"Arial",32);
    l_getyGod_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 7));
    m_labelArr->addObject(l_getyGod_a);
    
    //主机种类
    CCString *s_heroType_q = StringLibrary::shared()->getStringByKey("help_mainType_q");
	CCLabelTTF *l_heroType_q = CCLabelTTF::create(s_heroType_q->getCString(),"Arial",32);
    l_heroType_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_heroType_q);
    //主机种类解答
    CCString *s_heroType_a = StringLibrary::shared()->getStringByKey("help_mainType_a");
	CCLabelTTF *l_heroType_a = CCLabelTTF::create(s_heroType_a->getCString(),"Arial",32);
    l_heroType_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 4));
    m_labelArr->addObject(l_heroType_a);
    
    //僚机种类
    CCString *s_wingType_q = StringLibrary::shared()->getStringByKey("help_wingType_q");
	CCLabelTTF *l_wingType_q = CCLabelTTF::create(s_wingType_q->getCString(),"Arial",32);
    l_wingType_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_wingType_q);
    //僚机种类解答
    CCString *s_wingType_a = StringLibrary::shared()->getStringByKey("help_wingType_a");
	CCLabelTTF *l_wingType_a = CCLabelTTF::create(s_wingType_a->getCString(),"Arial",32);
    l_wingType_a->setDimensions(CCSizeMake(labelWidth,labelHeight  * 6));
    m_labelArr->addObject(l_wingType_a);
    
    //如何购买金币
    CCString *s_bugGod_q = StringLibrary::shared()->getStringByKey("help_howBuy_q");
	CCLabelTTF *l_bugGod_q = CCLabelTTF::create(s_bugGod_q->getCString(),"Arial",32);
    l_bugGod_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_bugGod_q);
    //如何购买金币解答
    CCString *s_bugGod_a = StringLibrary::shared()->getStringByKey("help_howBuy_a");
	CCLabelTTF *l_bugGod_a = CCLabelTTF::create(s_bugGod_a->getCString(),"Arial",32);
    l_bugGod_a->setDimensions(CCSizeMake(labelWidth,labelHeight *4));
     m_labelArr->addObject(l_bugGod_a);
    
    //如何获得高级评分
    CCString *s_getHighScore_q = StringLibrary::shared()->getStringByKey("help_getHighScore_q");
	CCLabelTTF *l_getHighScore_q = CCLabelTTF::create(s_getHighScore_q->getCString(),"Arial",32);
    l_getHighScore_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
     m_labelArr->addObject(l_getHighScore_q);
    //如何获得高级评分解答
    CCString *s_getHighScore_a = StringLibrary::shared()->getStringByKey("help_getHighScore_a");
	CCLabelTTF *l_getHighScore_a = CCLabelTTF::create(s_getHighScore_a->getCString(),"Arial",32);
    l_getHighScore_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 4));
    m_labelArr->addObject(l_getHighScore_a);

    //S级评分有什么奖励
    CCString *s_SReward_q = StringLibrary::shared()->getStringByKey("help_SReword_q");
	CCLabelTTF *l_SReward_q = CCLabelTTF::create(s_SReward_q->getCString(),"Arial",32);
    l_SReward_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_SReward_q);

    //S级评分奖励解答
    CCString *s_SReward_a = StringLibrary::shared()->getStringByKey("help_SReword_a");
	CCLabelTTF *l_SReward_a = CCLabelTTF::create(s_SReward_a->getCString(),"Arial",32);
    l_SReward_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 5));
    m_labelArr->addObject(l_SReward_a);
    
    //钻石有什么用
    CCString *s_useOFGem_q = StringLibrary::shared()->getStringByKey("help_useOfGem_q");
	CCLabelTTF *l_useOFGem_q = CCLabelTTF::create(s_useOFGem_q->getCString(),"Arial",32);
    l_useOFGem_q->setDimensions(CCSizeMake(labelWidth,labelHeight));
    m_labelArr->addObject(l_useOFGem_q);
    
    //钻石的作用解答
    CCString *s_useOfGem_a = StringLibrary::shared()->getStringByKey("help_useOfGem_a");
	CCLabelTTF *l_useOfGem_a = CCLabelTTF::create(s_useOfGem_a->getCString(),"Arial",32);
    l_useOfGem_a->setDimensions(CCSizeMake(labelWidth,labelHeight * 7));
    m_labelArr->addObject(l_useOfGem_a);
    
    //如何购买钻石
    CCString *s_buyGem_q = StringLibrary::shared()->getStringByKey("help_buyGem_q");
    CCLabelTTF* l_buyGem_q = CCLabelTTF::create(s_buyGem_q->getCString(), "Arial", 32);
    l_buyGem_q->setDimensions(CCSizeMake(labelWidth, labelHeight));
    m_labelArr->addObject(l_buyGem_q);
    
    //如何购买钻石解答
    CCString *s_buyGem_a = StringLibrary::shared()->getStringByKey("help_buyGem_a");
    CCLabelTTF* l_buyGem_a = CCLabelTTF::create(s_buyGem_a->getCString(), "Arial", 32);
    l_buyGem_a->setDimensions(CCSizeMake(labelWidth, labelHeight * 5));
    m_labelArr->addObject(l_buyGem_a);
   
    //设置字符位置和对其方式
    for (int i=0;i<m_labelArr->count(); i++)
    {
        CCLabelTTF* cur = NULL;
        //0.43f
        
        if ( 0 == i)
        {
            cur = (CCLabelTTF*)m_labelArr->objectAtIndex(i);
            cur->setPosition(ccp(containLayer->getContentSize().width * 0.5f,containLayer->getContentSize().height - 10));
        }
        else
        {
            CCLabelTTF* pre = (CCLabelTTF*)m_labelArr->objectAtIndex(i-1);
            cur = (CCLabelTTF*)m_labelArr->objectAtIndex(i);
            cur->setPosition(ccp(containLayer->getContentSize().width * 0.5f,pre->getPositionY() - pre->getDimensions().height - 20));
        }
        cur->setAnchorPoint(ccp(0.5,1));
        cur->setHorizontalAlignment(kCCTextAlignmentLeft);
        containLayer->addChild(cur,2);
    }
    
	CCMenuItemImage *item_tf = CCMenuItemImage::create("ui/button/btn_common_return_1.png",
                                                       "ui/button/btn_common_return_2.png",
                                                       this,
                                                       menu_selector(GameHelpLayer::menu_Return));
	item_tf->setPosition(ccp(10.0f,item_tf->getContentSize().height));
	item_tf->setAnchorPoint(ccp(0.0f,0.5f));
	CCMenu *ret = CCMenu::create(item_tf,NULL);
	ret->setPosition(CCPointZero);
	this->addChild(ret,1);

	this->setKeypadEnabled(true);
	CCLog("------GameHelpLayer::init----------2-----");
	return true;
}

GameHelpLayer* GameHelpLayer::create()
{
	GameHelpLayer *layer = new GameHelpLayer();
	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

void GameHelpLayer::menu_Return(CCObject* pSender)
{
    //SoundEffectLayer::sharedSoundEffectManager()->requestPlaySoundEffect(SOUND_CLICK, EFF_CLICK);
    //GameDelegate::shared()->openSceneMainMenu();
    //GameDelegate::shared()->openSceneGoldStore();
    //GameDelegate::shared()->openNotification();
}

void GameHelpLayer::keyBackClicked()
{
    //GameDelegate::shared()->openSceneMainMenu();
}

void GameHelpLayer::keyMenuClicked()
{

}


