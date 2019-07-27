#ifndef __FERRIS_GAME__GameAboutLayer__
#define __FERRIS_GAME__GameAboutLayer__

#include "cocos2d.h"

#include "ComplaintManager.h"
#include "CouponManager.h"
#include "UserData.h"

#include "FerrisDialog.h"
#include "FerrisResult.h"
#include "GameMainLayer.h"
#include "SoundEffectLayer.h"

USING_NS_CC;

#define TagInputControl 50
#define TagInputControlBack 51
#define TagInputDialog 100
#define TagAboutNode 101
#define TagCouponAlert 102


typedef enum
{
    kCouponOpen,
    kCouponClosing,
    kCouponClosed,
} CouponDialogStatus;

class GameAboutLayer : public CCLayer, public cocos2d::CCTextFieldDelegate
{
public:
    static GameAboutLayer* create();
    
	virtual ~GameAboutLayer(void);
    
    /**********************************************
     * 输入法挂接
     **********************************************/
	// 要有输出口
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
	// 当用户关闭虚拟键盘的时候回调函数
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
	// 当用户进行输入 虚拟键盘的时候回调函数
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
	// 当用户进行删除文字 虚拟键盘的时候回调函数
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);

    /**********************************************
     * 输入法挂接
     **********************************************/
	virtual void keyBackClicked();
	virtual void keyMenuClicked();
    
    /**********************************************
     * 创建节点
     **********************************************/
	//获取关于节点
	CCNode* getAboutNode();

	//获得输入框节点
	CCNode* getInputControlNode();

private:
	GameAboutLayer(void);
	bool init();
	
    /**********************************************
     * 其他控制
     **********************************************/
    
    //返回
    void menu_Return(CCObject* pSender);
    
    /**********************************************
     * 申诉
     **********************************************/
    
    //发起申诉
	void menu_SendComplaint(CCObject* pSender);
    
    //申诉结果显示
    
    /**********************************************
     * 奖券
     **********************************************/
    
    //开启输入奖券编码的界面
	void menu_OpenCoupon(CCObject* pSender);
    
    //确定关闭输入奖券编码的界面
    void callbackCouponOpened(CCNode* pSender);
    
    //开始关闭输入奖券编码的界面
	void menu_StartCloseCoupon(CCObject* pSender);
    
    //确定关闭输入奖券编码的界面
    void callbackCouponClosed(CCNode* pSender);
    
    //用户的输入框体(实际上是一个按钮),点击后挂接输入法
    void menu_StartInputCode(CCObject* pSender);
    
    //发送奖券编码
	void menu_SendCoupon(CCObject* pSender);
    
    //发送奖券后,服务器产生的回调
    void callbackCouponResult(FerrisResult *result);
    
    void callbackCloseCouponAlert(CCObject* pSender);
    
private:
    CouponManager *managerCoupon;
    
    CouponDialogStatus eClosingCoupon;

};

#endif