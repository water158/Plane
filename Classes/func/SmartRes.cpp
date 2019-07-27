
#include "SmartRes.h"

SmartRes* SmartRes::s_sharedRes = NULL;

SmartRes* SmartRes::sharedRes(void)
{
    if (!s_sharedRes)
    {
        s_sharedRes = new SmartRes();
        s_sharedRes->init();
    }
    return s_sharedRes;
}

bool SmartRes::init(void)
{
    //const CCSize& winSizeInPixels = CCDirector::sharedDirector()->getWinSizeInPixels();
    //setVirtualScreenWidth(winSizeInPixels.width);
    return true;
}


void SmartRes::setVirtualScreenValue(int dir, float value)
{
    if (dir == SMART_RES_FIXED_HEIGHT)
    {
        const CCSize& winSizeInPixels = CCDirector::sharedDirector()->getWinSizeInPixels();
        
        m_scale = winSizeInPixels.width / value;
        m_screenSize.width = value;
        m_screenSize.height = winSizeInPixels.height / m_scale;
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_screenSize.width, m_screenSize.height, kResolutionNoBorder);
        
        CCLOG("Virtual width: %0.2f, Virtual height: %0.2f, scale: %0.2f",
              m_screenSize.width, m_screenSize.height, m_scale);
        
        const CCSize& winSize = CCDirector::sharedDirector()->getWinSize();
        m_center.x = winSize.width / 2;
        m_center.y = winSize.height / 2;
        m_left = 0;
        m_right = m_screenSize.width - 1;
        m_top = m_screenSize.height - 1;
        m_bottom = 0;
    }
    else
    {
        
    }
}

void SmartRes::setVirtualScreenWidth(float width)
{
    // 木有找到 2.0 里面怎么获取设备方向。。所以假定是横向了
    const CCSize& winSizeInPixels = CCDirector::sharedDirector()->getWinSizeInPixels();
    CCLOG("getWinSizeInPixels width: %0.2f, height: %0.2f",
          winSizeInPixels.width, winSizeInPixels.height);
    
    float factor1 = CCDirector::sharedDirector()->getContentScaleFactor();
    CCLOG("getContentScaleFactor1 = %0.2f", factor1);
    
    const CCSize& winSize_pre = CCDirector::sharedDirector()->getWinSize();
    CCLOG("getWinSize Pre width: %0.2f, height: %0.2f",
          winSize_pre.width, winSize_pre.height);
    
    
    m_scale = winSizeInPixels.width / width;
    m_screenSize.width = width;
    m_screenSize.height = winSizeInPixels.height / m_scale;
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_screenSize.width, m_screenSize.height, kResolutionNoBorder);

    CCLOG("Virtual width: %0.2f, Virtual height: %0.2f, scale: %0.2f",
        m_screenSize.width, m_screenSize.height, m_scale);
    
    const CCSize& winSizeInPixels2 = CCDirector::sharedDirector()->getWinSizeInPixels();
    CCLOG("getWinSizeInPixels2 width: %0.2f, height: %0.2f",
          winSizeInPixels2.width, winSizeInPixels2.height);
    
    float factor2 = CCDirector::sharedDirector()->getContentScaleFactor();
    CCLOG("getContentScaleFactor2 = %0.2f", factor2);

    const CCSize& winSize = CCDirector::sharedDirector()->getWinSize();
    CCLOG("getWinSize Aft width: %0.2f, height: %0.2f",
          winSize.width, winSize.height);
    
    m_center.x = winSize.width / 2;
    m_center.y = winSize.height / 2;
    m_left = 0;
    m_right = m_screenSize.width - 1;
    m_top = m_screenSize.height - 1;
    m_bottom = 0;

    CCLOG("center x: %0.2f, center y: %0.2f", m_center.x, m_center.y);
    CCLOG("top: %0.2f, right: %0.2f, bottom: %0.2f, left: %0.2f", m_top, m_right, m_bottom, m_left);
}

void SmartRes::setVirtualScreenHeight(float height)
{
	const CCSize& winSizeInPixels = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCLOG("getWinSizeInPixels width: %0.2f, height: %0.2f",
		winSizeInPixels.width, winSizeInPixels.height);

	float factor1 = CCDirector::sharedDirector()->getContentScaleFactor();
	CCLOG("getContentScaleFactor1 = %0.2f", factor1);

	const CCSize& winSize_pre = CCDirector::sharedDirector()->getWinSize();
	CCLOG("getWinSize Pre width: %0.2f, height: %0.2f",
		winSize_pre.width, winSize_pre.height);


	m_scale = winSizeInPixels.height / height;
    //CCLOG("%f------------------------------1",CCDirector::sharedDirector()->getWinSize().height);
    // CCLOG("%f------------------------------1",winSizeInPixels.height);
	m_screenSize.width = winSizeInPixels.width / m_scale;
	m_screenSize.height = height;
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_screenSize.width, m_screenSize.height, kResolutionNoBorder);

	CCLOG("Virtual width: %0.2f, Virtual height: %0.2f, scale: %0.2f",
		m_screenSize.width, m_screenSize.height, m_scale);

	const CCSize& winSizeInPixels2 = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCLOG("getWinSizeInPixels2 width: %0.2f, height: %0.2f",
		winSizeInPixels2.width, winSizeInPixels2.height);

	float factor2 = CCDirector::sharedDirector()->getContentScaleFactor();
	CCLOG("getContentScaleFactor2 = %0.2f", factor2);

	const CCSize& winSize = CCDirector::sharedDirector()->getWinSize();
	CCLOG("getWinSize Aft width: %0.2f, height: %0.2f",
		winSize.width, winSize.height);

	m_center.x = winSize.width / 2;
	m_center.y = winSize.height / 2;
	m_left = 0;
	m_right = m_screenSize.width - 1;
	m_top = m_screenSize.height - 1;
	m_bottom = 0;

	CCLOG("center x: %0.2f, center y: %0.2f", m_center.x, m_center.y);
	CCLOG("top: %0.2f, right: %0.2f, bottom: %0.2f, left: %0.2f", m_top, m_right, m_bottom, m_left);
}

void SmartRes::setVirtualWinWidth(CCSize size)
{
	m_winSize = size;
}
