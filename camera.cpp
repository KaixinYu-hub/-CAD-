#include "Camera.h"

void Camera::updateCameraVectors()
{
  QVector3D front3(cos(this->_yaw) * cos(this->_picth), sin(this->_picth), sin(this->_yaw) * cos(this->_picth));
  this->_front = front3.normalized();
  this->_right = QVector3D::crossProduct(this->_front, this->_worldUp).normalized();
  this->_up = QVector3D::crossProduct(this->_right, this->_front).normalized();
}

void Camera::ScaleControl(GLfloat yoffset)
{
    double sensitive=0.05;
    QVector3D direction=this->_front;
    direction.normalize();
    this->_position=this->_position+yoffset*direction*this->_position.length()*sensitive;
}

QMatrix4x4 Camera::Camera::getViewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(this->_position, this->_position + this->_front, this->_up);
    return view;
}

QMatrix4x4 Camera::getProjMatrix(GLfloat w,GLfloat h)
{
    QMatrix4x4 Proj;
    Proj.perspective(45.0f, w/h, 0.1f, 10000.0f);
    return Proj;
}

