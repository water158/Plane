//
//  main.m
//  HelloCpp
//
//  Created by mini01 on 14-8-12.
//  Copyright __MyCompanyName__ 2014年. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AppController");
    [pool release];
    return retVal;
}
