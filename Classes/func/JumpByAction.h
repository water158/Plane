/*********
	created at 2014 07 14
	���١������˶���������
	
*********/

#ifndef __FERRIS_GAME__JumpByAction__
#define __FERRIS_GAME__JumpByAction__

#include "cocos2d.h"
#include "GlobalEnum.h"
#include "actions/CCActionInterval.h"
#include "FerrisTools.h"

USING_NS_CC;
USING_NS_STD;

//�������ٶ� 10
#define G 3;

//�׺����صĻ��� 1�׵���200����
#define MeterTransPixel 100


class JumpByAction : public CCActionInterval
{
public:
	/** initializes the action */
	bool initWithDuration(float duration, const CCPoint& position, bool speedup);

	virtual CCObject* copyWithZone(CCZone* pZone, const CCPoint& position, bool speedup);
	virtual void startWithTarget(CCNode *pTarget);
	virtual void update(float time);
	virtual CCActionInterval* reverse(void);

public:
	/** creates the action */
	static JumpByAction* create(const CCPoint& vec, bool speedup = false);
protected:
	CCPoint         m_startPosition;       //��ʼλ��
	CCPoint         m_delta;			   //λ������
	CCPoint         m_previousPos;		   //��һ��λ��
	float           m_fDuring;			   //����
	bool            m_bUp;				   //�Ƿ������˶�
	bool            m_bSpeedUp;			   //�Ƿ����
	float           m_fPreTime;			   //��һ�ε�ʱ��

};

#endif