#ifndef __FERRIS_GAME__AdManager__
#define __FERRIS_GAME__AdManager__

class AdManager
{
public:
    static AdManager *shared();
    AdManager();
    ~AdManager();
    
    void showRandomBanner();
    
    void showRandomInterstitial();
    
    void preloadAdmobInterstitial();
    
    void showAdmobInterstitial();
    
    /**************************
            触控广告条
     **************************/
    void showCoCoBanner();
    
    
    /**************************
            触控插入广告
     **************************/
    void showCoCoInterstitial();
    
};

#endif