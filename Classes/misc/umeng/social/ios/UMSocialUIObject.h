//
//  UMSocialUIObject.h
//  UmengGame
//
//  Created by yeahugo on 14-3-28.
//
//

#import <Foundation/Foundation.h>

#include "cocos2d.h"
#include "CCUMTypeDef.h"
#import "UMSocial.h"

using namespace umeng::social;
using namespace cocos2d;

@interface UMSocialUIObject : NSObject<UMSocialUIDelegate>
{
    ShareEventHandler handler;
}

-(id)initWithCallback:(ShareEventHandler)callBack;

@end
