#ifndef _GUNGROUPDATA_H_
#define _GUNGROUPDATA_H_

#include "cocos2d.h"
#include "GunData.h"

USING_NS_CC;
using namespace std;
//typedef map<string, GunData> MapGunData;

class GunGroupData : public CCObject
{
public:
	virtual ~GunGroupData(void);
    
    static GunGroupData* create(TiXmlElement *elm);

	//加载自身的数据
	bool loadData(TiXmlElement *elm);

    //加载当前枪管组内的所有枪管
    void loadGuns(TiXmlElement *elm);
    GunGroupData(void);
private:
	
    
protected:
	string id;
public:
	virtual string getID(void) const
	{
		return this->id;
	}
public:
	virtual void setID(string var)
	{
		id = var;
	}

    //CC_SYNTHESIZE (string, id, ID);
    CC_SYNTHESIZE (string, description, Description);
    CC_SYNTHESIZE (CCArray*, guns, Guns);
    //数量
    CC_SYNTHESIZE(int, m_nNum, Num);

};

#endif