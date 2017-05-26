//
//  iosfunc.m
//  kuxx-hllm_v1.1.1
//
//  Created by  admin on 16-11-25.
//
//

#import "HintBox.h"
#include "MBProgressHUD.h"

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
