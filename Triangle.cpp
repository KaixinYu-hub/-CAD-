#include "Triangle.h"
#include <string>
#include <QDebug>

const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "void main(){\n"
        "  gl_Position = projection * view * model *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";

const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

Triangle::Triangle()
{
    TriShader=new shader();
}

Triangle::~Triangle()
{
    delete TriShader;
    TriShader=nullptr;
}

void Triangle::initialize()
{
    this->initializeOpenGLFunctions();

    TriShader->compile(vertexShaderSource,fragmentShaderSource);

    //VAO，VBO数据部分
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,

    };

    //创建一个VBO并绑定VBO
    glGenBuffers(1, &TriVBO);
    glBindBuffer(GL_ARRAY_BUFFER, TriVBO);
    //把之前定义的顶点数据复制到缓冲的内存
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //创建一个VAO并绑定VAO
    glGenVertexArrays(1, &TriVAO);
    glBindVertexArray(TriVAO);
    //设置顶点属性指针，从VBO中“挖”出对应属性的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑VBO
    glBindVertexArray(0);//解绑VAO

    //初始化各个矩阵
    TriShader->use();
    TriShader->setMatrix4f("model",model);
    view.translate(QVector3D(0.0f, 0.0f, -5.0f));
    TriShader->setMatrix4f("view", view);
    projection.perspective(45.0f, (GLfloat)width()/(GLfloat)height(), 0.1f, 100.0f);
    TriShader->setMatrix4f("projection", projection);
}

void Triangle::paint()
{
    //开启着色器程序
    TriShader->use();

//    qDebug()<<view<<endl;

    //开始绘制图形

    glBindVertexArray(TriVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Triangle::updateView(QMatrix4x4 mat)
{
    view=mat;
    TriShader->setMatrix4f("view", view);
}

void Triangle::updateProj(QMatrix4x4 mat)
{
    projection=mat;
    TriShader->setMatrix4f("projection", projection);
}
