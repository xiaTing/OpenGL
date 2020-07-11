
#include "GLShaderManager.h"
#include "GLTools.h"
#include <GLUT/GLUT.h>

// 定义一个着色器管理器
GLShaderManager shaderMagager;
// 定义一个批次类容器
GLBatch triangleBatch;
// 正方形边长的1/2
GLfloat blockSize = 0.1f;
//正方形顶点数据
GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0,
    blockSize,-blockSize,0.0,
    blockSize,blockSize,0.0,
    -blockSize,blockSize,0.0,
};
//x轴方向的移动量
GLfloat xPos = 0.0f;
//y轴方向的移动量
GLfloat yPos = 0.0f;

/*
 在窗口大小改变时，接收新的宽度&高度。
 */
void changeSize(int w,int h)
{
    /*
      x,y 参数代表窗口中视图的左下角坐标，而宽度、高度是像素为表示，通常x,y 都是为0
     */
    glViewport(0, 0, w, h);

}

void RenderScene(void){
 //清理要使用的缓存区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    GLfloat vColor[] = {0.3,0.8,0.6,1.0};
    M3DMatrix44f mFinalTansformMatrix;
    //平移动
    m3dTranslationMatrix44(mFinalTansformMatrix, xPos, yPos, 0);
    //提交矩阵结果给固定着色器，触发绘制
    shaderMagager.UseStockShader(GLT_SHADER_FLAT,mFinalTansformMatrix,vColor);
    triangleBatch.Draw();
    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y){
    GLfloat stepSize = 0.025f;
    if (key == GLUT_KEY_UP) {
        yPos += stepSize;
    }
    if (key == GLUT_KEY_DOWN) {
        yPos -= stepSize;
    }
    if (key == GLUT_KEY_LEFT) {
        xPos -= stepSize;
    }
    if (key == GLUT_KEY_RIGHT) {
        xPos += stepSize;
    }
    // 边界碰撞检测
    if (xPos < -1.0f + blockSize) {
        xPos = -1.0f + blockSize;
    }
    if (xPos > 1.0f - blockSize) {
        xPos = 1.0f - blockSize;
    }
    if (yPos < - 1.0f + blockSize) {
        yPos = -1.0f + blockSize;
    }
    if (yPos > 1.0f - blockSize) {
        yPos = 1.0f -blockSize;
    }
    glutPostRedisplay();
}

void setupRC(void) {
    // 设置清屏颜色
    
    glClearColor(0.8, 0.3, 0.6, 1.0);
    
    //初始化固定着色器
    shaderMagager.InitializeStockShaders();
    
    //指定批次容器的类型和顶点个数
    
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    
    //拷贝图形的顶点数据
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    triangleBatch.End();
    
}

int main(int argc,char *argv[])
{
    //设置工作路径
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(600, 600);
    glutCreateWindow("方向键控制正方形移动");
    // 注册函数
    //注册窗口发生改变寒色
    glutReshapeFunc(changeSize);
    //注册渲染函数
    glutDisplayFunc(RenderScene);
    //注册特殊函数，监听方向键按钮操作的回调函数
    glutSpecialFunc(SpecialKeys);
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("GLEW Error:%s\n",glewGetErrorString(status));
    }
    // 设置渲染环境
    setupRC();
    // 启动GLUT循环
    glutMainLoop();
    return 0;
}
