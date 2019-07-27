#include "FerrisParticle.h"
#include "UserData.h"
#include "textures/CCTextureCache.h"
#include "textures/CCTextureAtlas.h"
#include "support/base64.h"
#include "support/CCPointExtension.h"
#include "platform/CCFileUtils.h"
#include "platform/CCImage.h"
#include "platform/platform.h"
#include "support/zip_support/ZipUtils.h"
#include "CCDirector.h"
#include "support/CCProfiling.h"

FerrisParticle::FerrisParticle()
{
    
}

FerrisParticle::~FerrisParticle()
{
    
}

FerrisParticle * FerrisParticle::create(const char *plistFile)
{
    FerrisParticle *pRet = new FerrisParticle();
    if (pRet && pRet->initWithFile(plistFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool FerrisParticle::initWithFile(const char *plistFile)
{
    bool bRet = false;
    m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());
    
    CCAssert( dict != NULL, "Particles: file not found");
    
    // XXX compute path from a path, should define a function somewhere to do it
    string listFilePath = plistFile;
    if (listFilePath.find('/') != string::npos)
    {
        listFilePath = listFilePath.substr(0, listFilePath.rfind('/') + 1);
        bRet = this->initWithDictionary(dict, listFilePath.c_str());
    }
    else
    {
        bRet = this->initWithDictionary(dict, "");
    }
    
    dict->release();
    
    return bRet;
}

bool FerrisParticle::initWithDictionary(CCDictionary *dictionary, const char *dirname)
{
    
    bool bRet = false;
    unsigned char *buffer = NULL;
    unsigned char *deflated = NULL;
    CCImage *image = NULL;
    do
    {
        int maxParticles = dictionary->valueForKey("maxParticles")->intValue();
        
//        std::string textureName = dictionary->valueForKey("textureFileName")->getCString();
//        
//        CCLog("maxParticles =====    11   ===== %d",maxParticles);
        
        if (!UserData::shared()->getParticleEnable())
        {
            maxParticles = maxParticles / 2;
        }
        
//        CCLog("maxParticles =====    22   ===== %d",maxParticles);
//        CCLog("maxParticles =====    path   ===== %s",textureName.c_str());
        
        // self, not super
        if(this->initWithTotalParticles(maxParticles))
        {
            // angle
            m_fAngle = dictionary->valueForKey("angle")->floatValue();
            m_fAngleVar = dictionary->valueForKey("angleVariance")->floatValue();
            
            // duration
            m_fDuration = dictionary->valueForKey("duration")->floatValue();
            
            // blend function
            m_tBlendFunc.src = dictionary->valueForKey("blendFuncSource")->intValue();
            m_tBlendFunc.dst = dictionary->valueForKey("blendFuncDestination")->intValue();
            
            // color
            m_tStartColor.r = dictionary->valueForKey("startColorRed")->floatValue();
            m_tStartColor.g = dictionary->valueForKey("startColorGreen")->floatValue();
            m_tStartColor.b = dictionary->valueForKey("startColorBlue")->floatValue();
            m_tStartColor.a = dictionary->valueForKey("startColorAlpha")->floatValue();
            
            m_tStartColorVar.r = dictionary->valueForKey("startColorVarianceRed")->floatValue();
            m_tStartColorVar.g = dictionary->valueForKey("startColorVarianceGreen")->floatValue();
            m_tStartColorVar.b = dictionary->valueForKey("startColorVarianceBlue")->floatValue();
            m_tStartColorVar.a = dictionary->valueForKey("startColorVarianceAlpha")->floatValue();
            
            m_tEndColor.r = dictionary->valueForKey("finishColorRed")->floatValue();
            m_tEndColor.g = dictionary->valueForKey("finishColorGreen")->floatValue();
            m_tEndColor.b = dictionary->valueForKey("finishColorBlue")->floatValue();
            m_tEndColor.a = dictionary->valueForKey("finishColorAlpha")->floatValue();
            
            m_tEndColorVar.r = dictionary->valueForKey("finishColorVarianceRed")->floatValue();
            m_tEndColorVar.g = dictionary->valueForKey("finishColorVarianceGreen")->floatValue();
            m_tEndColorVar.b = dictionary->valueForKey("finishColorVarianceBlue")->floatValue();
            m_tEndColorVar.a = dictionary->valueForKey("finishColorVarianceAlpha")->floatValue();
            
            // particle size
            m_fStartSize = dictionary->valueForKey("startParticleSize")->floatValue();
            m_fStartSizeVar = dictionary->valueForKey("startParticleSizeVariance")->floatValue();
            m_fEndSize = dictionary->valueForKey("finishParticleSize")->floatValue();
            m_fEndSizeVar = dictionary->valueForKey("finishParticleSizeVariance")->floatValue();
            
            // position
            float x = dictionary->valueForKey("sourcePositionx")->floatValue();
            float y = dictionary->valueForKey("sourcePositiony")->floatValue();
            this->setPosition( ccp(x,y) );
            m_tPosVar.x = dictionary->valueForKey("sourcePositionVariancex")->floatValue();
            m_tPosVar.y = dictionary->valueForKey("sourcePositionVariancey")->floatValue();
            
            // Spinning
            m_fStartSpin = dictionary->valueForKey("rotationStart")->floatValue();
            m_fStartSpinVar = dictionary->valueForKey("rotationStartVariance")->floatValue();
            m_fEndSpin= dictionary->valueForKey("rotationEnd")->floatValue();
            m_fEndSpinVar= dictionary->valueForKey("rotationEndVariance")->floatValue();
            
            m_nEmitterMode = dictionary->valueForKey("emitterType")->intValue();
            
            // Mode A: Gravity + tangential accel + radial accel
            if( m_nEmitterMode == kCCParticleModeGravity )
            {
                // gravity
                modeA.gravity.x = dictionary->valueForKey("gravityx")->floatValue();
                modeA.gravity.y = dictionary->valueForKey("gravityy")->floatValue();
                
                // speed
                modeA.speed = dictionary->valueForKey("speed")->floatValue();
                modeA.speedVar = dictionary->valueForKey("speedVariance")->floatValue();
                
                // radial acceleration
                modeA.radialAccel = dictionary->valueForKey("radialAcceleration")->floatValue();
                modeA.radialAccelVar = dictionary->valueForKey("radialAccelVariance")->floatValue();
                
                // tangential acceleration
                modeA.tangentialAccel = dictionary->valueForKey("tangentialAcceleration")->floatValue();
                modeA.tangentialAccelVar = dictionary->valueForKey("tangentialAccelVariance")->floatValue();
                
                // rotation is dir
                modeA.rotationIsDir = dictionary->valueForKey("rotationIsDir")->boolValue();
            }
            
            // or Mode B: radius movement
            else if( m_nEmitterMode == kCCParticleModeRadius )
            {
                modeB.startRadius = dictionary->valueForKey("maxRadius")->floatValue();
                modeB.startRadiusVar = dictionary->valueForKey("maxRadiusVariance")->floatValue();
                modeB.endRadius = dictionary->valueForKey("minRadius")->floatValue();
                modeB.endRadiusVar = 0.0f;
                modeB.rotatePerSecond = dictionary->valueForKey("rotatePerSecond")->floatValue();
                modeB.rotatePerSecondVar = dictionary->valueForKey("rotatePerSecondVariance")->floatValue();
                
            } else {
                CCAssert( false, "Invalid emitterType in config file");
                CC_BREAK_IF(true);
            }
            
            // life span
            m_fLife = dictionary->valueForKey("particleLifespan")->floatValue();
            m_fLifeVar = dictionary->valueForKey("particleLifespanVariance")->floatValue();
            
            // emission Rate
            m_fEmissionRate = m_uTotalParticles / m_fLife;
            
            //don't get the internal texture if a batchNode is used
            if (!m_pBatchNode)
            {
                // Set a compatible default for the alpha transfer
                m_bOpacityModifyRGB = false;
                
                // texture
                // Try to get the texture from the cache
                std::string textureName = dictionary->valueForKey("textureFileName")->getCString();
                
                size_t rPos = textureName.rfind('/');
                
                if (rPos != string::npos)
                {
                    string textureDir = textureName.substr(0, rPos + 1);
                    
                    if (dirname != NULL && textureDir != dirname)
                    {
                        textureName = textureName.substr(rPos+1);
                        textureName = string(dirname) + textureName;
                    }
                }
                else
                {
                    if (dirname != NULL)
                    {
                        textureName = string(dirname) + textureName;
                    }
                }
                
                CCTexture2D *tex = NULL;
                
                if (textureName.length() > 0)
                {
                    // set not pop-up message box when load image failed
                    bool bNotify = CCFileUtils::sharedFileUtils()->isPopupNotify();
                    CCFileUtils::sharedFileUtils()->setPopupNotify(false);
                    tex = CCTextureCache::sharedTextureCache()->addImage(textureName.c_str());
                    // reset the value of UIImage notify
                    CCFileUtils::sharedFileUtils()->setPopupNotify(bNotify);
                }
                
                if (tex)
                {
                    setTexture(tex);
                }
                else
                {
                    const char *textureData = dictionary->valueForKey("textureImageData")->getCString();
                    CCAssert(textureData, "");
                    
                    int dataLen = strlen(textureData);
                    if(dataLen != 0)
                    {
                        // if it fails, try to get it from the base64-gzipped data
                        int decodeLen = base64Decode((unsigned char*)textureData, (unsigned int)dataLen, &buffer);
                        CCAssert( buffer != NULL, "CCParticleSystem: error decoding textureImageData");
                        CC_BREAK_IF(!buffer);
                        
                        int deflatedLen = ZipUtils::ccInflateMemory(buffer, decodeLen, &deflated);
                        CCAssert( deflated != NULL, "CCParticleSystem: error ungzipping textureImageData");
                        CC_BREAK_IF(!deflated);
                        
                        // For android, we should retain it in VolatileTexture::addCCImage which invoked in CCTextureCache::sharedTextureCache()->addUIImage()
                        image = new CCImage();
                        bool isOK = image->initWithImageData(deflated, deflatedLen);
                        CCAssert(isOK, "CCParticleSystem: error init image with Data");
                        CC_BREAK_IF(!isOK);
                        
                        setTexture(CCTextureCache::sharedTextureCache()->addUIImage(image, textureName.c_str()));
                        
                        image->release();
                    }
                }
                CCAssert( this->m_pTexture != NULL, "CCParticleSystem: error loading the texture");
            }
            bRet = true;
        }
    } while (0);
    CC_SAFE_DELETE_ARRAY(buffer);
    CC_SAFE_DELETE_ARRAY(deflated);
    return bRet;
}