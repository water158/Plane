//
//  LaserBeam.cpp
//  UseShader
//
//  Created by renyi on 14-9-4.
//
//

#include "LaserBeam.h"

bool LaserBeam::init()
{
	m_fTotalTime = 0.0f;
	m_nTimeUniformLocation = 0;


    CCGLProgram *pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderFilename("game/laser/lesson.vert", "game/laser/lesson.frag");
    pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    pProgram->link();
    pProgram->updateUniforms();
    pProgram->retain();
    
 
    this->setShaderProgram(pProgram);
  
    
    m_nTimeUniformLocation = glGetUniformLocation(pProgram->getProgram(), "u_time");

    m_pShaderProgram->use();
    
    this->scheduleUpdate();
    return true;
}

LaserBeam* LaserBeam::create(const char *pszFileName, CraftView* node1, CraftView* node2)
{
    LaserBeam *pobSprite = new LaserBeam();
    if (pobSprite && pobSprite->initWithFile(pszFileName) && pobSprite->init())
    {
        pobSprite->autorelease();

		pobSprite->m_pHero = node1;
		pobSprite->m_pEnemy = node2;

        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void LaserBeam::update(float delta)
{
	//主机和敌机的位置
	CCPoint posHero = m_pHero->getPosition();
	CCPoint posEnemy = m_pEnemy->getPosition();

	float fWidth = this->getContentSize().width;
	float fHeight = this->getContentSize().height;

	float temp_x = posEnemy.x - posHero.x;
	float temp_y = posEnemy.y - posHero.y;
	
	//更新位置
	this->setPosition(ccp(posHero.x + temp_x/2, posHero.y + temp_y/2));

	if(temp_y < 0)
	{
		temp_y = -temp_x;
	}
	if(temp_x < 0)
	{
		temp_x = -temp_x;
	}
	//更新画板大小
	this->setScaleX(temp_x / fWidth - 0.05f);
	this->setScaleY(temp_y / fHeight);

    m_fTotalTime += delta * 2.0f;
    
    m_pShaderProgram->use();
    
    m_nTimeUniformLocation = m_pShaderProgram->getUniformLocationForName("u_time");

	int shader_x = m_pShaderProgram->getUniformLocationForName("u_x");
	int shader_y = m_pShaderProgram->getUniformLocationForName("u_y");

    glUniform1f(m_nTimeUniformLocation, m_fTotalTime);

	float laser_x = this->getPositionX();
	float fPercent_x = laser_x/960.0f;

	float laser_y = this->getPositionY();
	float fPercent_y = laser_y/960.0f;

	glUniform1f(shader_x, fPercent_x);
	glUniform1f(shader_y, fPercent_y);


	//将位置信息传导配置文件
	int hero_x = m_pShaderProgram->getUniformLocationForName("hero_x");
	int hero_y = m_pShaderProgram->getUniformLocationForName("hero_y");

	int enemy_x = m_pShaderProgram->getUniformLocationForName("enemy_x");
	int enemy_y = m_pShaderProgram->getUniformLocationForName("enemy_y");

	glUniform1f(hero_x, posHero.x);
	glUniform1f(hero_y, posHero.y);
	glUniform1f(enemy_x, posEnemy.x);
	glUniform1f(enemy_y, posEnemy.y);

}