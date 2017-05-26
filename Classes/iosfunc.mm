//
//  iosfunc.m
//  kuxx-hllm_v1.1.1
//
//  Created by  admin on 16-11-25.
//
//

#import <Foundation/Foundation.h>
#import "iosfunc.h"

const char * getuuid()
{
    NSString * uuid = [[NSUUID UUID] UUIDString];
    return [uuid UTF8String];
}

const char * getbundleid()
{
    return [[[NSBundle mainBundle]bundleIdentifier] UTF8String];
}

const char * getvercode()
{
    return [[[NSBundle mainBundle]objectForInfoDictionaryKey:@"CFBundleShortVersionString"] UTF8String];
}


void setUserDefaultsBool(char * key, bool value) {
    NSUserDefaults * usedef = [NSUserDefaults standardUserDefaults];
    [usedef setBool:value forKey:[NSString stringWithUTF8String:key]];
}

bool getUserDefaultsBool() {
    NSUserDefaults * usedef = [NSUserDefaults standardUserDefaults];
    bool a = [usedef boolForKey:@"noads"];
    return a;
}

