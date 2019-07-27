
#ifndef __FERRIS_GAME__SmartRes__
#define __FERRIS_GAME__SmartRes__


#include "cocos2d.h"

USING_NS_CC;

#define SMART_RES_FIXED_HEIGHT 0
#define SMART_RES_FIXED_WIDTH 1
#define _s_center     SmartRes::sharedRes()->getCenter()
#define _s_top        SmartRes::sharedRes()->getTop()
#define _s_right      SmartRes::sharedRes()->getRight()
#define _s_bottom     SmartRes::sharedRes()->getBottom()
#define _s_left       SmartRes::sharedRes()->getLeft()
#define _s_scale      SmartRes::sharedRes()->getScale()
#define _s_xy(v)      SmartRes::sharedRes()->xy(v)
#define _s_height     SmartRes::sharedRes()->getScreenHeight()
#define _s_width      SmartRes::sharedRes()->getScreenWidth()
#define _s_winsize    SmartRes::sharedRes()->getScreenSize()
#define _s_wsize      SmartRes::sharedRes()->getWinSize()

class SmartRes
{
public:
    static SmartRes* sharedRes(void);

    //
    void setVirtualScreenWidth(float width);
    void setVirtualScreenHeight(float height);
    void setVirtualScreenValue(int dir, float value);
	void setVirtualWinWidth(CCSize size);

    const CCSize getScreenSize(void)
    {
        return m_screenSize;
    }

    const float getScreenHeight(void)
    {
        return m_screenSize.height;
    }

    const float getScreenWidth(void)
    {
        return m_screenSize.width;
    }

    float getScale(void)
    {
        return m_scale;
    }

    const CCPoint getCenter(void)
    {
        return m_center;
    }

    float getTop(void)
    {
        return m_top;
    }

    float getRight(void)
    {
        return m_right;
    }

    float getBottom(void)
    {
        return m_bottom;
    }

    float getLeft(void)
    {
        return m_left;
    }

    float xy(float v)
    {
        return m_scale * v;
    }

    const CCSize getWinSize(void)
    {
        return m_winSize;
    }

private:
    SmartRes(void)
        : m_scale(1.0f)
        , m_top(0)
        , m_right(0)
        , m_bottom(0)
        , m_left(0)
    {
    }
    bool init(void);

    static SmartRes* s_sharedRes;

    CCSize m_screenSize;
    CCPoint m_center;
    float m_scale;
    float m_top;
    float m_right;
    float m_bottom;
    float m_left;

    CCSize m_winSize;
};

#endif