#include "FilterNode.h"
#include "cocos-ext.h"

FilterNode::FilterNode():_srcNode(NULL)
{ }

FilterNode::~FilterNode()
{
	CCLog("------------");
}

CCGLProgram* FilterNode::commonPrograme(const GLchar* src)
{
	CCGLProgram* pp = new CCGLProgram();
	if (pp) {
		pp->initWithVertexShaderByteArray(ccPositionTextureColor_vert, src);
		pp->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		pp->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		pp->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		pp->link();
		pp->updateUniforms();
	}
	return pp;
}

FilterNode* FilterNode::create(CCNode* src, const CCSize& size)
{
	FilterNode* ret = new FilterNode();
	if (ret && ret->init(src, size)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_RELEASE(ret);
		return NULL;
	}
}

bool FilterNode::init(CCNode* src, const CCSize& size)
{
	bool bRet = false;
	do {
		if (_srcNode) {
			CCLog("init can only be called once!");
		}
		CC_BREAK_IF(!CCRenderTexture::initWithWidthAndHeight(size.width, size.height,
								     kCCTexture2DPixelFormat_RGBA8888));
		_srcNode = src;
		if (_srcNode)
			_pos = _srcNode->getPosition();
		else
			_pos = CCPointZero;
		bRet = true;
	}while(0);
	return bRet;
}

void FilterNode::capture()
{
	if (!_srcNode){ return; }
	CCPoint pos = _srcNode->getPosition();
	CCPoint ar = _srcNode->getAnchorPoint();
	_pos = ccpSub(pos, _pos);
	_srcNode->setPosition(ar.x * _srcNode->getContentSize().width, ar.y * _srcNode->getContentSize().height);
	this->beginWithClear(0, 0, 0, 0);
	_srcNode->visit();
	this->end();
	_srcNode->setPosition(pos);
	this->getSprite()->setPosition(ccpAdd(this->getSprite()->getPosition(), _pos));
	_pos = pos;
}

/*********************| GuassianBlur class |*********************/
extern GLchar*	 hblurSrc = NULL;
extern GLchar*	 vblurSrc = NULL;
static GaussianBlur* _scSizeBlurNode = NULL;

GaussianBlur::GaussianBlur():f1(NULL), f2(NULL), _showing(false)
{  }

GaussianBlur::~GaussianBlur()
{
	CC_SAFE_RELEASE_NULL(f1);
	CC_SAFE_RELEASE_NULL(f2);
}

bool GaussianBlur::do_ready()
{
	unsigned long l = 0;
	unsigned char* data = NULL;
	if (!hblurSrc) {
    		hblurSrc = (GLchar*)CCFileUtils::sharedFileUtils()->getFileData("shaders/hblur.fsh", "rb", &l);
		hblurSrc[l] = 0;
	}
	if (!vblurSrc) {
    		vblurSrc = (GLchar*)CCFileUtils::sharedFileUtils()->getFileData("shaders/vblur.fsh", "rb", &l);
		vblurSrc[l] = 0;
	}
	screenBlurNodeInstance();
	return hblurSrc && vblurSrc;
}

void GaussianBlur::do_free()
{
	CC_SAFE_DELETE_ARRAY(hblurSrc);
	CC_SAFE_DELETE_ARRAY(vblurSrc);
	CC_SAFE_RELEASE_NULL(_scSizeBlurNode);
}

CCGLProgram*GaussianBlur::blurProgram(bool isV, GLfloat ratio, GLfloat hvsize)
{
	CCGLProgram* p = FilterNode::commonPrograme(isV ? vblurSrc : hblurSrc);
	setBlurData(p, isV, ratio, hvsize);
	return p;
}

void GaussianBlur::setBlurData(CCGLProgram* p, bool isV, GLfloat ratio, GLfloat hvsize)
{
	if (p) {
		p->use();
		GLint blurSLoc = glGetUniformLocation(p->getProgram(), "ratio");
		p->setUniformLocationWith1f(blurSLoc, ratio);
		blurSLoc = glGetUniformLocation(p->getProgram(), isV ? "vsize" : "hsize");
		p->setUniformLocationWith1f(blurSLoc, hvsize);
	}
}

GaussianBlur*GaussianBlur::create(CCNode* src, const CCSize& size)
{
	GaussianBlur* ret = new GaussianBlur();
	if (ret && ret->init(src, size)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return NULL;
	}
}

GaussianBlur*GaussianBlur::screenBlurNodeInstance()
{
	if (_scSizeBlurNode)
		return _scSizeBlurNode;
	_scSizeBlurNode = new GaussianBlur();
	_scSizeBlurNode->retain();
	_scSizeBlurNode->init(NULL, CCDirector::sharedDirector()->getVisibleSize(), true);
	return _scSizeBlurNode;
}

GaussianBlur* GaussianBlur::takeScreenCapture()
{
	GaussianBlur* n = screenBlurNodeInstance();
	CCNode* s = CCDirector::sharedDirector()->getRunningScene();
	if (s) {
		CCPoint a = s->getAnchorPoint();
		s->setAnchorPoint(CCPointZero);
		n->reset(s);
		s->setAnchorPoint(a);
		return n;
	}
	return NULL;
}

bool GaussianBlur::init(CCNode* src, const CCSize& size, bool reused)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!CCNode::init());
		f1 = FilterNode::create(src, size);
		CC_BREAK_IF(!f1);
		f1->retain();
		CCGLProgram* s = FilterNode::commonPrograme(hblurSrc);
		f1->filter(s);
		setBlurSize(size, 1);
		f1->capture();
		s->release();

		f2 = FilterNode::create(f1->getSprite(), size);
		CC_BREAK_IF(!f2);
		s = FilterNode::commonPrograme(vblurSrc);
		f2->filter(s);
		setBlurSize(size, 2);
		f2->retain();
		f2->capture();
		s->release();

		this->setContentSize(size);
		this->setAnchorPoint(CCPointZero);
		this->setPosition(CCPointZero);
		f2->setPosition(size.width /2, size.height/2);
		this->addChild(f2);

		bRet = true;
	}while(0);
	return bRet;
}

void GaussianBlur::setBlurSize(const CCSize& size, const int which , GLfloat ratio)
{
	if (!which || 1 == which) {
		setBlurData(f1->getSprite()->getShaderProgram(), false, ratio, size.width);
	}
	if (!which || 2 == which) {
		setBlurData(f2->getSprite()->getShaderProgram(), true, ratio, size.height);
	}
}

bool GaussianBlur::reset(CCNode* src)
{
	f1->changeNode(src);
	f2->changeNode(f1->getSprite());
}

void GaussianBlur::cleanFromWorld()
{
	realtime(false);
	this->removeFromParent();
	this->removeAllChildren();
	this->addChild(f2);
	_showing = false;
}

void GaussianBlur::show()
{
	if (_showing) return;
	CCScene* s =CCDirector::sharedDirector()->getRunningScene();
	if (s) {
		_showing = true;
		s->addChild(this);
	}
}

void GaussianBlur::capture()
{
	f1->capture();
	f2->capture();
}

