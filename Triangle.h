#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "shader.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class Triangle:public QOpenGLWidget,protected QOpenGLExtraFunctions
{
public:
    Triangle();
    ~Triangle();
    void initialize();
    void paint();

    void updateView(QMatrix4x4 mat);
    void updateProj(QMatrix4x4 mat);


public:
    shader* TriShader;
    GLuint TriVAO;
    GLuint TriVBO;
    QMatrix4x4 model, projection, view;

};

#endif // TRIANGLE_H
