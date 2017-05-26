//
//  UnityAdsMana.cpp
//  kuxx-hllm_v1.1.1
//
//  Created by Hailin Rao on 2016/12/6.
//
//

#include "UnityAdsMana.h"
//#include "RootViewController.h"
#include "MyUnityAdsDelegate.h"
#import "GlobalData.h"
//#import "TalkingDataGA.h"

//RootViewController * ads;
MyUnityAdsDelegate * ads;
void initAds() {
    //ads = [[RootViewController alloc] init];
    //ads = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    ads = [[MyUnityAdsDelegate alloc] init];
    //[ads initUnityAds];
    [ads initVungleAds];
    [ads initAdmobAds];
}

void showAds(int adswhere, int coin, int gem, int hp) {
    NSArray * eventName = [NSArray arrayWithObjects:@"revive",@"success",@"removeads",@"dailyads",@"addtime",@"chooseprop",@"shop",@"buylife",@"account",nil];
    //[TalkingDataGA onEvent:[eventName objectAtIndex:adswhere] eventData:nil];
    int choose = GlobalData::getAdsChoose(adswhere);
    /*if (adswhere == 0 || adswhere == 1 || adswhere == 3 || adswhere == 4 || adswhere == 5 || adswhere == 6 || adswhere == 7) {
        if ([ads unityIsReady]) {
            [ads showUnityAds:adswhere icoin:coin igem:gem ihp:hp];
            //[TalkingDataGA onEvent:@"unity" eventData:nil];
        }
        else if ([ads vungleIsReady]) {
            [ads showVungleAds:adswhere icoin:coin igem:gem ihp:hp];
            //[TalkingDataGA onEvent:@"vungle" eventData:nil];
        }
    }
    else {
        if ([ads admobIsReady]) {
            [ads showAdMobAds:adswhere icoin:coin igem:gem ihp:hp];
            //[TalkingDataGA onEvent:@"google" eventData:nil];
        }
        else if ([ads vungleIsReady]) {
            [ads showVungleAds:adswhere icoin:coin igem:gem ihp:hp];
            //[TalkingDataGA onEvent:@"vungle" eventData:nil];
        }
        else if ([ads unityIsReady])
            [ads showUnityAds:adswhere icoin:coin igem:gem ihp:hp];
    }*/
    if (choose == 1)
    {
        if ([ads admobIsReady]) {
            [ads showAdMobAds:adswhere icoin:coin igem:gem ihp:hp];
        }
        else if ([ads vungleIsReady]) {
            [ads showVungleAds:adswhere icoin:coin igem:gem ihp:hp];
        }
        else if ([ads unityIsReady])
            [ads showUnityAds:adswhere icoin:coin igem:gem ihp:hp];
    }
    else if (choose == 2)
    {
        if ([ads vungleIsReady]) {
            [ads showAdMobAds:adswhere icoin:coin igem:gem ihp:hp];
        }
        else if ([ads admobIsReady]) {
            [ads showVungleAds:adswhere icoin:coin igem:gem ihp:hp];
        }
        else if ([ads unityIsReady])
            [ads showUnityAds:adswhere icoin:coin igem:gem ihp:hp];
    }
    else if (choose == 3)
    {
        if ([ads unityIsReady]) {
            [ads showAdMobAds:adswhere icoin:coin igem:gem ihp:hp];
        }
        else if ([ads admobIsReady]) {
            [ads showVungleAds:adswhere icoin:coin igem:gem ihp:hp];
        }
        else if ([ads vungleIsReady])
            [ads showUnityAds:adswhere icoin:coin igem:gem ihp:hp];
    }
}

bool isAdsReady() {
    return [ads isReady];
}

void alterView() {
    /*UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"评论" message:@"求求你给个好评吧！" delegate:nil cancelButtonTitle:@"拒绝" otherButtonTitles:@"下次再说", nil];
    alert.alertViewStyle = UIAlertViewStyleDefault;
    [alert show];*/
    /*UIActionSheet * sheet = [[UIActionSheet alloc] initWithTitle:@"求评论" delegate:nil cancelButtonTitle:@"残忍拒绝" destructiveButtonTitle:@"下次再说" otherButtonTitles:@"乐意接受", nil];
    UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [sheet showInView:];*/
    UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    UIAlertController * alert = [UIAlertController alertControllerWithTitle:@"请求" message:@"玩的这么爽，求求您给个五星好评吧！" preferredStyle:UIAlertControllerStyleActionSheet];
    //NSMutableAttributedString *hogan = [[NSMutableAttributedString alloc] initWithString:@"玩的这么爽"];
    //[hogan addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:20.0] range:NSMakeRange(0, 20)];
    //[alert setValue:hogan forKey:@"attributedTitle"];
    alert.popoverPresentationController.barButtonItem = s.navigationItem.leftBarButtonItem;
    [alert addAction:[UIAlertAction actionWithTitle:@"残忍拒绝" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        GlobalData::setNoComments(true);
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"下次再说" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"乐意接受" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://itunes.apple.com/cn/app/%E6%96%B0%E6%AC%A2%E4%B9%90%E8%BF%9E%E8%BF%9E%E7%9C%8B-qq%E7%BB%8F%E5%85%B8%E5%8D%95%E6%9C%BA%E5%AF%B9%E5%AF%B9%E7%A2%B02017/id1196569348?mt=8"]];
        GlobalData::setNoComments(true);
    }]];
    [s presentViewController:alert animated:YES completion:nil];
}
