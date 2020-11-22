#ifndef SHADER_H
#define SHADER_H

#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QString>

class shader
{
public:
    shader();
    void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

    ~shader();

    void use(){
      _shaderProgram->bind();
    }
    void setFloat(const QString& name, const GLfloat& value);

    void setInteger(const QString& name, const GLint& value);

    void setVector2f(const QString& name, const GLfloat& x, const GLfloat& y);

    void setVector2f(const QString& name, const QVector2D& value);

    void setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z);

    void setVector3f(const QString& name, const QVector3D& value);

    void setVector4f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w);

    void setVector4f(const QString& name, const QVector4D& value);

    void setMatrix4f(const QString& name, const QMatrix4x4& value);

    void setBool(const QString& name, const GLboolean& value);

private:
    QOpenGLShaderProgram* _shaderProgram;

};



#endif // SHADER_H
