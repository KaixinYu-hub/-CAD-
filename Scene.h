#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTime>

#include "shader.h"
#include "Camera.h"
#include "Polygon.h"

class Scene:public QOpenGLWidget,protected QOpenGLExtraFunctions
{
public:
    Scene();

    //需要的信息：1、环的顶点数组 2、每个环对应的顶点的个数，存在一个数组里面 3、环的个数，默认为1
    void AddPolygon(float Vertexes[], int PtNumOfLoop[],int LoopNum=1);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void wheelEvent(QWheelEvent *ev);

private:
    std::vector<myPolygon>_mypolygons;
    myPolygon _mypolygon;
    Camera _camera;

    QVector2D _mousePressPos;
    QVector2D _wheelPressPos;

    QVector3D _CameraNowPos;
    QVector3D _CameraNowFront;
    QVector3D _CameraNowUp;

    QTime time;
    GLfloat _deltaTime;
    GLfloat _lastFrame;

};

#endif // WIDGET_H
