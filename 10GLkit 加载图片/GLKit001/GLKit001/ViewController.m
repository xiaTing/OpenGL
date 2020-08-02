//
//  ViewController.m
//  GLKit001
//
//  Created by 夏婷 on 2020/8/1.
//  Copyright © 2020 sunny. All rights reserved.
//

#import "ViewController.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

@interface ViewController (){
    EAGLContext *context;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    //1.初始化上下文 context
    context = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!context) {
        NSLog(@"Create ES context Failed");
    }
    //3.设置当前上下文
    [EAGLContext setCurrentContext:context];
    //4.
    GLKView *view = (GLKView *)self.view;
    view.context = context;
    //5.
    glClearColor(1, 0, 0, 1);
    
}
#pragma mark --
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    glClear(GL_COLOR_BUFFER_BIT);
}

@end
