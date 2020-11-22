#ifndef CAMERA_H
#define CAMERA_H
#include <QOpenGLShader>//为了glfloat 这些变量，所以引进这个头文件

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

//使用这些宏，可以很方便的修改初始化数据
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.25;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ZOOM = 45.0f;


class Camera
{
public:
    Camera():_position( QVector3D(0.0f, 0.0f, 3.0f)),
        _worldUp(QVector3D(0.0f, 1.0f, 0.0f)),
        _front(QVector3D(0.0f, 0.0f, -1.0f)),
        _picth(PITCH),_yaw(YAW)
    {
        //计算一下_up和_right
        this->updateCameraVectors();
    }

    void updateCameraVectors();

    void ScaleControl(GLfloat yoffset);

    QMatrix4x4 getViewMatrix();
    QMatrix4x4 getProjMatrix(GLfloat w,GLfloat h);



public:
    //相机在世界坐标系的位置，以及世界坐标系的上向量
    QVector3D _position;
    QVector3D _worldUp;

    //view坐标系的三个方向，向前，向上，向右
    QVector3D _front;
    QVector3D _up;
    QVector3D _right;

    //两个欧拉角
    GLfloat _picth;
    GLfloat _yaw;

};

#endif // CAMERA_H
