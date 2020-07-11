// 导入着色器管理器
#include "GLShaderManager.h"
//GLTools 中包含了大部分GLTool中类似C语言的独立函数
#include "GLTools.h"
// 在Mac系统下导入 #include <GLUT/GLUT.h> 在Windows 和Linux系统中用freeglut的静态库版本并需要添加一个宏
#include <GLUT/GLUT.h>

// 定义一个着色器
GLShaderManager shaderManager;
// 定义一个批次容器，是GLTools的一个简单的容器里
GLBatch triangelBath;

void setupRC (void) {
    printf("首先执行的这里1\n");
    // 设置清屏颜色（背景）
    glClearColor(0.8, 0.3, 0.6, 1);
    //初始化一个固定着色器，没有着色器，在OpenGL核心框架中是无法进行任何渲染的
    shaderManager.InitializeStockShaders();
    
    // 指定顶点 坐标范围[-1,1]
    GLfloat vVerts[] = {
        -0.5,0.0,0.0,
          0.5,0.0,0.0,
          0.0,0.5,0.0,
    };
    // 开启三角形批次类
    triangelBath.Begin(GL_TRIANGLES, 3);
    // 拷贝顶点数据到着色器
    triangelBath.CopyVertexData3f(vVerts);
    //三角形批次类处理结束
    triangelBath.End();
}

void ChangeSize(int w,int h) {
    /**
     x,y代表窗口中视图的左下角坐标，而高度、宽度是像素位表示,通常都是0
     */
    glViewport(0, 0, w, h);
    printf("窗口发生改变或者初始化2\n");
}
void RanderScene (void) {
    // 清除一个或一组特定的缓存区
    printf("触发渲染了3\n");
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    // 设置一组浮点数来表示颜色
    GLfloat VRed[] = {0.3,0.8,0.6,1.0};
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,VRed);
    //提交着色器
    triangelBath.Draw();
    //将后台缓冲区进行渲染，然后结束后交换给前台
    glutSwapBuffers();
}

 //在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
/*
 缓存区是一块存在图像信息的存储空间，红色、绿色、蓝色和alpha分量一起作为颜色缓存区或像素缓存区
 OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
 清除缓存区对数值进行预置
 参数：指定将要清除的缓存的
 GL_COLOR_BUFFER_BIT :指示当前激活的用来进行颜色写入缓冲区
 GL_DEPTH_BUFFER_BIT :指示深度缓存区
 GL_STENCIL_BUFFER_BIT:指示模板缓冲区
 */



int main(int argc,char *argv[])
{
    // 初始化GLUT库，这个函数传说是命令参数并初始化glut库
    glutInit(&argc, argv);
    // 双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Triangle");
    // 注册重塑函数
    glutReshapeFunc(ChangeSize);
    // 注册显示函数
    glutDisplayFunc(RanderScene);
    // 初始化一个GLEW库，确保OpenGL API 对程序完全可用
    GLenum status = glewInit();
    if(GLEW_OK != status) {
        // 打印错误情况
        printf("GLEW Error:%s\n",glewGetErrorString(status));
    }
    //  环境可用，设置我们的渲染环境
    setupRC();
    /**
    GLUT 内部运行一个本地消息循环，拦截适当的消息，然后调用我们不同时间注册的回调函数，我们一共注册了两个函数
    1.为窗口改变大小的一个回调函数
    2.包含OpenGL渲染的回调函数*/
    
    // 启动GLUT的循环
    glutMainLoop();
    return  0;
}
