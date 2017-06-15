//
//  iosfunc.m
//
//  Created by  admin on 16-11-25.
//
//

#import <Foundation/Foundation.h>
#import "iosfunc.h"
#include "MBProgressHUD.h"

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

void copytoclipboard(char * p) {
    //把char*转换成OC的NSString
    NSString *nsMessage= [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    
    //获得iOS的剪切板
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    
    //改变剪切板的内容
    pasteboard.string = nsMessage;
    toast("QQ号码已经复制到剪贴板！");
}

const void toast(char * info)
{
    UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    MBProgressHUD * HUD = [[MBProgressHUD alloc] initWithView:s.view];
    [s.view addSubview:HUD];
    HUD.label.text = [NSString stringWithUTF8String:info];
    HUD.mode = MBProgressHUDModeText;
    
    //指定距离中心点的X轴和Y轴的偏移量，如果不指定则在屏幕中间显示
    //    HUD.yOffset = 150.0f;
    //    HUD.xOffset = 100.0f;
    
    [HUD showAnimated:YES whileExecutingBlock:^{
        sleep(1.5);
    } completionBlock:^{
        [HUD removeFromSuperview];
        [HUD release];
        //HUD = NULL;
    }];
}
