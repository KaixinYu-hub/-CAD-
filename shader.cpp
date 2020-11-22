#include "shader.h"

shader::shader():_shaderProgram(nullptr)
{

}

shader::~shader(){

}

void shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    //顶点着色器
        QOpenGLShader vertexShader(QOpenGLShader::Vertex);
        bool success = vertexShader.compileSourceCode(vertexSource);
        if(!success){
            qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
            qDebug() << vertexShader.log() << endl;
        }
    //片段着色器
        QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
        success  = fragmentShader.compileSourceCode(fragmentSource);
        if(!success){
            qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
            qDebug() << fragmentShader.log() << endl;
        }
    //几何着色器
        QOpenGLShader geometryShader(QOpenGLShader::Geometry);
        if(geometrySource !=nullptr){
            success  = geometryShader.compileSourceCode(geometrySource);
            if(!success){
                qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED" << endl;
                qDebug() << geometryShader.log() << endl;
            }
        }
    //将他们附着到shaderprogram上，并链接
        _shaderProgram = new QOpenGLShaderProgram();
        _shaderProgram->addShader(&vertexShader);
        _shaderProgram->addShader(&fragmentShader);
        if(geometrySource != nullptr)
            _shaderProgram->addShader(&geometryShader);
        success = _shaderProgram->link();
        if(!success){
            qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
            qDebug() << _shaderProgram->log() << endl;
        }

}

void shader::setFloat(const QString& name, const GLfloat& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }

 void shader::setInteger(const QString& name, const GLint& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }

 void shader::setVector2f(const QString& name, const GLfloat& x, const GLfloat& y){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, QVector2D(x, y));
 }

 void shader::setVector2f(const QString& name, const QVector2D& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }

 void shader::setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, QVector3D(x, y, z));
 }

 void shader::setVector3f(const QString& name, const QVector3D& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }

 void shader::setVector4f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, QVector4D(x, y, z, w));
 }

 void shader::setVector4f(const QString& name, const QVector4D& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }

 void shader::setMatrix4f(const QString& name, const QMatrix4x4& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }

 void shader::setBool(const QString& name, const GLboolean& value){
   GLuint loc = _shaderProgram->uniformLocation(name);
   _shaderProgram->setUniformValue(loc, value);
 }
