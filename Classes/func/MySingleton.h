#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/***************
实现一个模板类，用其子类实现一些全局的变量管理
  利用instance()函数可以获取唯一实例，利用release()释放唯一实例
***************/
template <class T>
class MySingleton
{
public:
	//获取实例的唯一函数
	static inline T* instance();
	//释放类的唯一实例
	void release();

protected:
	MySingleton(void){}
	~MySingleton(void){}
	static T* _instance;
};

template <class T>
T* MySingleton<T>::instance()
{
	if(!_instance)
		_instance = new T;
	return _instance;
}

template <class T>
void MySingleton<T>::release()
{
	if(!_instance)
		return;
	delete _instance;
	_instance = 0;
}

//使用时需要在CPP文件开始部分声明一下，否则识别不了模板类
#define DECLARE_SINGLETON_MEMBER(_Ty)	\
	template <> _Ty* MySingleton<_Ty>::_instance = NULL;


#endif