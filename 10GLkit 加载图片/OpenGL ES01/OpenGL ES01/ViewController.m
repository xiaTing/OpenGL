//
//  ViewController.m
//  OpenGL ES01
//
//  Created by 夏婷 on 2020/7/26.
//  Copyright © 2020 sunny. All rights reserved.
//

#import "ViewController.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

@interface ViewController (){
    EAGLContext *context;
    GLKBaseEffect *cEffect;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    //1 设置初始环境
    [self setupConfig];
    // 2. 加载顶点/纹理坐标数据
    [self setUpVertexData];
    // 3. 加载纹理数据(使用 GLKBaseEffect)
    [self setUpTexture];
}

- (void)setupConfig {
    // 1.设置上下文 & 设置当前上下文
    /*EAGLContext 是苹果iOS平台下实现OpenGL ES渲染层
     kEAGLRenderingAPIOpenGLES1 固定管线
     kEAGLRenderingAPIOpenGLES2 = 2,
     kEAGLRenderingAPIOpenGLES3 = 3,
     
     */
    context = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
    // 判断是否创建成功
    if (!context) {
        NSLog(@"Create context Failed");
    }
    // 设置当前上下文
    [EAGLContext setCurrentContext:context];
    //2.获取GLKView & 设置context
    GLKView *view = (GLKView *)self.view;
    view.context = context;
    /*3.配置视图创建的渲染缓存区.
        
        (1). drawableColorFormat: 颜色缓存区格式.
        简介:  OpenGL ES 有一个缓存区，它用以存储将在屏幕中显示的颜色。你可以使用其属性来设置缓冲区中的每个像素的颜色格式。
        
        GLKViewDrawableColorFormatRGBA8888 = 0,
        默认.缓存区的每个像素的最小组成部分（RGBA）使用8个bit，（所以每个像素4个字节，4*8个bit）。
        
        GLKViewDrawableColorFormatRGB565,
        如果你的APP允许更小范围的颜色，即可设置这个。会让你的APP消耗更小的资源（内存和处理时间）
        
        (2). drawableDepthFormat: 深度缓存区格式
        
        GLKViewDrawableDepthFormatNone = 0,意味着完全没有深度缓冲区
        GLKViewDrawableDepthFormat16,
        GLKViewDrawableDepthFormat24,
        如果你要使用这个属性（一般用于3D游戏），你应该选择GLKViewDrawableDepthFormat16
        或GLKViewDrawableDepthFormat24。这里的差别是使用GLKViewDrawableDepthFormat16
        将消耗更少的资源
        
        */
       
    // 3.配置视图创建的渲染缓存区 RGBA 各占8位
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    // 深度格式用24位
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    // 4. 设置清屏颜色
    glClearColor(1.0, 0.0, 0, 1.0);

}
- (void)setUpVertexData {
    
    GLfloat vertexData[] = {
        
        0.5, -0.5, 0.0f,    1.0f, 0.0f, //右下
        0.5, 0.5,  0.0f,    1.0f, 1.0f, //右上
        -0.5, 0.5, 0.0f,    0.0f, 1.0f, //左上
        
        0.5, -0.5, 0.0f,    1.0f, 0.0f, //右下
        -0.5, 0.5, 0.0f,    0.0f, 1.0f, //左上
        -0.5, -0.5, 0.0f,   0.0f, 0.0f, //左下
    };
    /*
     顶点数组: 开发者可以选择设定函数指针，在调用绘制方法的时候，直接由内存传入顶点数据，也就是说这部分数据之前是存储在内存当中的，被称为顶点数组
     顶点缓存区: 性能更高的做法是，提前分配一块显存，将顶点数据预先传入到显存当中。这部分的显存，就被称为顶点缓冲区
     */
    // 2.开辟顶点缓存区
     //2.1 创建顶点缓存区标识ID
    GLuint bufferID;
    glGenBuffers(1, &bufferID);
    // 2.2 绑定顶点缓存区，明确作用
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    // 2.3 将数据拷贝到顶点缓存区中（显存中）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    //3.打开资源通道
    /* (1)在iOS中, 默认情况下，出于性能考虑，所有顶点着色器的属性（Attribute）变量都是关闭的.
        意味着,顶点数据在着色器端(服务端)是不可用的. 即使你已经使用glBufferData方法,将顶点数据从内存拷贝到顶点缓存区中(GPU显存中).
        所以, 必须由glEnableVertexAttribArray 方法打开通道.指定访问属性.才能让顶点着色器能够访问到从CPU复制到GPU的数据.
        注意: 数据在GPU端是否可见，即，着色器能否读取到数据，由是否启用了对应的属性决定，这就是glEnableVertexAttribArray的功能，允许顶点着色器读取GPU（服务器端）数据。
      
       (2)方法简介
       glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
      
       功能: 上传顶点数据到显存的方法（设置合适的方式从buffer里面读取数据）
       参数列表:
           index,指定要修改的顶点属性的索引值,例如
           size, 每次读取数量。（如position是由3个（x,y,z）组成，而颜色是4个（r,g,b,a）,纹理则是2个.）
           type,指定数组中每个组件的数据类型。可用的符号常量有GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT, GL_FIXED, 和 GL_FLOAT，初始值为GL_FLOAT。
           normalized,指定当被访问时，固定点数据值是否应该被归一化（GL_TRUE）或者直接转换为固定点值（GL_FALSE）
           stride,指定连续顶点属性之间的偏移量。如果为0，那么顶点属性会被理解为：它们是紧密排列在一起的。初始值为0
           ptr指定一个指针，指向数组中第一个顶点属性的第一个组件。初始值为0
     */
    // 顶点坐标数据
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    // 从第0个开始读，没有读3个GL_FLOAT数据，是否归一化，每5个为一组
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT,GL_FALSE, sizeof(GLfloat) * 5, (GLfloat *)NULL + 0);
    //纹理坐标数据
    glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
    // 从下标为3开始读，每5个数据为一组
    glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLfloat *)NULL + 3);
    
}
-(void)setUpTexture {
    //1.获取图片路径
    NSString *filePath = [[NSBundle mainBundle]pathForResource:@"kunkun" ofType:@"jpg"];
    //2.设置纹理参数
    //纹理坐标原点是左下角,但是图片显示原点应该是左上角.不设置图片会倒置
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:@(1),GLKTextureLoaderOriginBottomLeft, nil];
    GLKTextureInfo *textureInfo = [GLKTextureLoader textureWithContentsOfFile:filePath options:options error:nil];
    //3. GLKit
    cEffect = [[GLKBaseEffect alloc]init];
    cEffect.texture2d0.enabled = GL_TRUE;
    cEffect.texture2d0.name = textureInfo.name;
    
}
#pragma mark -- GLKViewDelegate
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    //1.清除颜色缓存区
    glClear(GL_COLOR_BUFFER_BIT);
    //2.准备绘制
    [cEffect prepareToDraw];
    //3.开始绘制 6个顶点
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
@end
