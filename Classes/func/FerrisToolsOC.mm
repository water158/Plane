
#include "FerrisToolsOC.h"

int getDeviceType()
{
    NSString *deviceType = [[UIDevice currentDevice] model];
    
    NSLog(@"deviceType--------------%@",deviceType);
    
    if ([deviceType compare:@"iPad"] == NSOrderedSame)
    {
        NSLog(@"************device is iPad**********");
        return 1;  //1代表iPad
    }
    
    return 0;  //0代表其他
}