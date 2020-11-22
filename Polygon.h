#ifndef POLYGON_H
#define POLYGON_H
#include "shader.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <windows.h>
#include <glut.h>
#include <vector>
#include "Point3d.h"


void CALLBACK vertexCallback(GLvoid * vertex);
void CALLBACK beginCallback(GLenum type);
void CALLBACK endCallback();
void CALLBACK errorCallback(GLenum errorCode);

class myPolygon:protected QOpenGLExtraFunctions

{
public:
    myPolygon();
    ~myPolygon();

    //设置多边形的环的顶点坐标
    //LoopNum是环的个数
    //PtNumOfLoop是一个数组，记录了每个环对应的顶点个数
    //Vertexes是一个数组，记录了所有顶点的坐标信息
    void setLoopVertexes(float* Vertexes, int* PtNumOfLoop,int LoopNum );

    void initialize();
    void paint();


    friend void CALLBACK vertexCallback(GLvoid * vertex);
    friend void CALLBACK beginCallback(GLenum type);
    friend void CALLBACK endCallback();
    friend void CALLBACK errorCallback(GLenum errorCode);

public:
    static myPolygon* tessPoly;
    void updateView(QMatrix4x4 mat);
    void updateProj(QMatrix4x4 mat);

private:
    shader PolyShader;
    std::vector<GLuint>PolyVAOs;
    std::vector<GLuint>PolyVBOs;
    std::vector<std::vector<Point3d>>Loops;

    std::vector<GLenum>drawMode;
    std::vector<std::vector<GLfloat>>tessVertexes;

    QMatrix4x4 model, projection, view;

};



#endif // POLYGON_H
