//
//  ViewController.m
//  OffScreenDemo
//
//  Created by 夏婷 on 2020/7/7.
//  Copyright © 2020 sunny. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIView *view = [[UIView alloc]initWithFrame:CGRectMake(50, 50, 200, 200)];
    view.backgroundColor = [UIColor magentaColor];
    view.layer.borderColor = [UIColor cyanColor].CGColor;
    view.layer.cornerRadius = 100.0;
    view.layer.borderWidth = 5;
    //比上一段代码  多添加了contents
    view.layer.contents = (__bridge id )[UIImage imageNamed:@"fff.jpg"].CGImage;
    // 设置背景和边框的裁剪
    view.clipsToBounds = YES;
    // 设置包括contents的边框裁剪
    view.layer.masksToBounds = YES;
    view.center = CGPointMake(self.view.center.x,view.center.y);
    [self.view addSubview:view];
    
    UIImageView *imageView = [[UIImageView alloc]initWithFrame:CGRectMake(50, 300, 200, 200)];
    //比上一段多添加了背景颜色
    imageView.backgroundColor = [UIColor cyanColor];
    imageView.image = [UIImage imageNamed:@"fff.jpg"];
    imageView.layer.cornerRadius = 100;
    // 设置背景和边框的裁剪
    imageView.clipsToBounds = YES;
    // 设置包括contents的边框裁剪
    imageView.layer.masksToBounds = YES;
    imageView.center = CGPointMake(self.view.center.x,imageView.center.y);
    [self.view addSubview:imageView];
    
    
    

    
//    UIButton *button = [[UIButton alloc]initWithFrame:CGRectMake(50, 300, 100, 100)];
//    [button setImage:[UIImage imageNamed:@"fff.jpg"] forState:UIControlStateNormal];
//    button.imageView.layer.cornerRadius = 50;
//    button.imageView.layer.masksToBounds = YES;
//    button.layer.cornerRadius = 50;
//    button.clipsToBounds = YES;
//    [self.view addSubview:button];
    

   
}


@end
