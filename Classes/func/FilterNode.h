#ifndef FILETERSPRITE_H
#define FILETERSPRITE_H

#include "cocos2d.h"
USING_NS_CC;

class FilterProto
{
public:
	FilterProto():_rt(false) {}
	virtual void			capture() = 0;
	virtual void			updateCapture(bool b) = 0;
	void				realtime(bool b) {
		if (b != _rt) {
			_rt = b;
			updateCapture(b);
		}
	}
	inline bool			isRealTime() {return _rt;}
private:
	bool				_rt;
};

#define UPCAP_FUNC(_cls_, _freshfunc_) \
	void updateCapture(bool b) { \
		if (b) \
			this->schedule(schedule_selector(_cls_::_freshfunc_)); \
		else \
			this->unschedule(schedule_selector(_cls_::_freshfunc_));\
	}

/**
 * @brief The FilterNode class
 */
class FilterNode : public CCRenderTexture, public FilterProto
{
public:
	FilterNode();
	~FilterNode();
public:
	static CCGLProgram*		commonPrograme(const GLchar* src);
	static FilterNode*		create(CCNode* src, const CCSize& size);
	inline static FilterNode*	create(CCNode* src) {
		if (src) return create(src, src->getContentSize());
		return NULL;
	}
public:
	bool 				init(CCNode* src, const CCSize& size);
	/* 设定filter */
	inline void 			filter(CCGLProgram* shader) {
		this->getSprite()->setShaderProgram(shader);
	}
	inline void			changeNode(CCNode* src) {_srcNode = src; capture();}

	/* 快照 */
	void				capture();
	UPCAP_FUNC(FilterNode, freshCapture)
private:
	void				freshCapture(float dt) { capture();}
private:
	CCNode*				_srcNode;
	CCPoint				_pos;
};

/**
 * @brief The GaussianBlur class
 */
class GaussianBlur : public CCNode, public FilterProto
{
public:
	GaussianBlur();
	~GaussianBlur();
public:
	static bool			do_ready();
	static void			do_free();
	static CCGLProgram*		blurProgram(bool isV, GLfloat ratio, GLfloat hvsize);
	static void			setBlurData(CCGLProgram* p, bool isV, GLfloat ratio, GLfloat hvsize);
	static GaussianBlur*		create(CCNode* src, const CCSize& size);
	inline static GaussianBlur*	create(CCNode* src) {
		if (src) return create(src, src->getContentSize());
		return NULL;
	}
	/**
	 * @brief screenBlurNode 获取一个屏幕大小的模糊单例
	 */
	static GaussianBlur*		screenBlurNodeInstance();
	static GaussianBlur*		takeScreenCapture();
public:
	bool 				init(CCNode* src, const CCSize& size, bool reused = false);
	/**
	 * @brief setBlurSize 设置模糊程度
	 * @param size 模糊范围
	 * @param which 0横纵向同时设置，1横向设置，2纵向设置
	 * @param ratio 模糊半径
	 */
	void				setBlurSize(const CCSize& size, const int which = 0, GLfloat ratio = 3.0);
	bool				reset(CCNode* src);
	void				cleanFromWorld();
	void				show();
	/* 快照 */
	void				capture();
	UPCAP_FUNC(GaussianBlur, freshCapture)
private:
	void				freshCapture(float dt) { capture();}
private:
	FilterNode		*f1, *f2;
	bool				_showing;
};

#endif // FILETERSPRITE_H
