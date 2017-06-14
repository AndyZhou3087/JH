//
//  iosfunc.m
//  kuxx-hllm_v1.1.1
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
    //��char*ת����OC��NSString
    NSString *nsMessage= [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    
    //���iOS�ļ��а�
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    
    //�ı���а������
    pasteboard.string = nsMessage;
    toast("QQ�����Ѿ����Ƶ��˼����壡");
}

const void toast(char * info)
{
    UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    MBProgressHUD * HUD = [[MBProgressHUD alloc] initWithView:s.view];
    [s.view addSubview:HUD];
    HUD.label.text = [NSString stringWithUTF8String:info];
    HUD.mode = MBProgressHUDModeText;
    
    //ָ���������ĵ��X���Y���ƫ�����������ָ��������Ļ�м���ʾ
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
