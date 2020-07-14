//演示了OpenGL背面剔除，深度测试，和多边形模式
#include "GLTools.h"	
#include "GLMatrixStack.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLGeometryTransform.h"

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
//设置角色帧，作为相机
GLFrame viewFrame;
// 使用GLFrustum类来设置透视投影
GLFrustum viewFrustum;
// 三角形批次类
GLTriangleBatch torusBatch;
//矩阵堆栈
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
//几何变换的管道
GLGeometryTransform transformPipeline;
// 着色器管理
GLShaderManager shaderManager;

// 标记：背面剔除、深度测试
int iCull = 0;
int iDepth = 0;


//渲染场景
void RenderScene()
{
    // 1.清楚窗口和深度缓冲区
    // 不清空颜色/深度缓冲区 数据残留
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(iCull){
        //开启表面剔除
        glEnable(GL_CULL_FACE);
        //设置正面 默认 GL_CCW
        glFrontFace(GL_CCW);
        //设置剔除面 默认 GL_BACK
        glCullFace(GL_BACK);
    }else {
        glDisable(GL_CULL_FACE);
    }
    
    if (iDepth) {
        glEnable(GL_DEPTH_TEST);
    }else
        glDisable(GL_DEPTH_TEST);
    // 2.把摄像机矩阵压栈
    modelViewMatrix.PushMatrix(viewFrame);
    // 3.设置绘制颜色
    GLfloat vYerrow[] = {1.0f,1.0f,0.0f,1.0f};
    // 4.使用平面着色器
     //不加光源  旋转之后不会变黑 但是依然出现了问题，只是我们区分不出来,
     // shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vYerrow);
    // 添加默认光源
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT,transformPipeline.GetModelViewMatrix(),transformPipeline.GetProjectionMatrix(),vYerrow);
    // 5. 绘制
    torusBatch.Draw();
    // 6. 出栈
    modelViewMatrix.PopMatrix();
    // 7. 交换缓冲区
    glutSwapBuffers();
}

//窗口改变
void ChangeSize(int w, int h)
{
    //1防止h变为0
    if (h==0)
        h = 1;
    //2 设置视口窗口大小
    glViewport(0, 0, w, h);
    //3 设置观察视角
    //参数1:角度
    //参数2:纵横比
    //参数3:近截剪面距离
    //参数4:远截剪面距离
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0, 100.0f);
    //4 把透视矩阵加载到透视投影的矩阵堆栈中
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    //5.初始化渲染管道，获取modelviewMatrix 和 projectionMatrix相乘的结果
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void SpecialKeys(int key,int x,int y) {
    //1 判断方向
    if (key == GLUT_KEY_UP) {
        //2. 围绕X轴 旋转-5.0度，将-5度转换为弧度
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_DOWN) {
        // 围绕X轴 旋转5.0度，将5度转换为弧度
        viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_LEFT) {
        // 围绕Y轴 旋转-5.0度，将-5度转换为弧度
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
    }
    if (key == GLUT_KEY_RIGHT) {
        viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
    }
    // 刷新重绘
    glutPostRedisplay();
    
}
void SetupRC(void) {
    //1 设置清屏颜色
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    //2 初始化着色器管理器
    shaderManager.InitializeStockShaders();
    //3 将相机向后移动7个单元:肉眼到物体的距离
    viewFrame.MoveForward(7.0);
    
    //创建一个甜甜圈
    //参数1:容器帮助类
    //参数2:外边缘半径
    //参数3:内边缘半径
    //参数4、5:主半径和从半径的细分单元数量
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
    //4.设置绘制点的大小
    glPointSize(4.0f);
    
}
void ProcessMenu(int value) {
    switch (value) {
        case 1:
            iDepth = !iDepth;
            break;
        case 2:
            iCull = !iCull;
            break;
        case 3:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 4:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 5:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    //申请缓冲区
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGBA|GLUT_STENCIL);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Geometry Test Program");
    //注册窗口改变函数
    glutReshapeFunc(ChangeSize);
    //上下左右点击回调
    glutSpecialFunc(SpecialKeys);
    //注册渲染显示函数
    glutDisplayFunc(RenderScene);
    // 添加右击菜单栏
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Toggle depth test", 1);
    glutAddMenuEntry("Toggle cull bacekface", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set Line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW error:%s",glewGetErrorString(err));
    }
    SetupRC();
    glutMainLoop();
    return 0;
}
