#include "ParticleHelper.H"


//为特定按钮添加粒子效果
void ParticleHelper::addParticleForButton(const char* filepath, CCNode* pAimButton, CCPoint init_pos, float width, float height, bool symmetry, float offset_x, float offset_y)
{
	if(!pAimButton)
	{
		CCMessageBox("pAimButton is null", "GameDelegate");
		return;
	}
	//计算时间
	float fSpeed = 200.0f;
	float fTime_x = width/fSpeed;
	float fTime_y = height/fSpeed;
    
	//按钮粒子特效
	//FerrisParticle* particle_temp = FerrisParticle::getParticleByFile(filepath);
	CCParticleSystemQuad* particle_temp = CCParticleSystemQuad::create(filepath);
	if(!particle_temp)
	{
		CCMessageBox("no such particle file", "GameDelegate");
		return;
	}
	particle_temp->setPosition(init_pos);
	pAimButton->addChild(particle_temp, 0, 10);
    
	CCSequence* seq1 = CCSequence::create(CCMoveBy::create(fTime_y, ccp(offset_x, height)),
                                          CCMoveBy::create(fTime_x, ccp(width - offset_x, 0.0f)),
                                          CCMoveBy::create(fTime_y, ccp(-offset_x, -height)),
                                          CCMoveBy::create(fTime_x, ccp(-width + offset_x, 0.0f)),
                                          NULL);
    
	particle_temp->runAction(CCRepeatForever::create(seq1));
    
	if(symmetry)
	{
		//FerrisParticle* particle_temp1 = FerrisParticle::getParticleByFile(filepath);
		CCParticleSystemQuad* particle_temp1 = CCParticleSystemQuad::create(filepath);
		particle_temp1->setPosition(ccpAdd(ccp(width, height), init_pos));
		pAimButton->addChild(particle_temp1, 0, 10);
        
		CCSequence* seq2 = CCSequence::create(CCMoveBy::create(fTime_y, ccp(-offset_x, -height)),
                                              CCMoveBy::create(fTime_x, ccp(-width + offset_x, 0.0f)),
                                              CCMoveBy::create(fTime_y, ccp(offset_x, height)), 
                                              CCMoveBy::create(fTime_x, ccp(width - offset_x, 0.0f)),
                                              NULL);
		particle_temp1->runAction(CCRepeatForever::create(seq2));
	}
}