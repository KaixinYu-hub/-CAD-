#include "Scene.h"
#include "ui_widget.h"
#include "shader.h"

Scene::Scene()
{

}

void Scene::AddPolygon(float* Vertexes, int* PtNumOfLoop, int LoopNum)
{
    myPolygon poly;
    poly.setLoopVertexes(Vertexes,PtNumOfLoop,LoopNum);
    _mypolygons.push_back(poly);
}

void Scene::initializeGL()
{
    this->initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0,1.0);

    if(!_mypolygons.empty())
    {
        for(auto &i:_mypolygons)
        {
            i.initialize();
        }
    }

    //计时开始
    time.start();
    _deltaTime = 0.0f;
    _lastFrame = 0.0f;
}

void Scene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

}

void Scene::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //刷新时间
    GLfloat currentFrame = (GLfloat)time.elapsed()/100;
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;


    // 多边形的绘制与更新

    if(!_mypolygons.empty())
    {
        for(auto &i:_mypolygons)
        {
            i.paint();
            i.updateView(_camera.getViewMatrix());
            i.updateProj(_camera.getProjMatrix(width(),height()));
        }
    }

    update();

}

//获取鼠标按压时的坐标，保存下来
void Scene::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        _mousePressPos.setX(ev->x());
        _mousePressPos.setY(ev->y());

        _CameraNowPos=_camera._position;
        _CameraNowUp=_camera._up;
        _CameraNowFront=_camera._front;

    }

    if (ev->button() == Qt::MidButton) {
        _wheelPressPos.setX(ev->x());
        _wheelPressPos.setY(ev->y());

        _CameraNowPos = _camera._position;
    }
}

void Scene::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::LeftButton)
    {
        QVector2D rot;
        QQuaternion rotation;
        rot.setX((ev->x() - _mousePressPos.x()) / width());
        rot.setY((ev->y() - _mousePressPos.y()) / height());

        rotation = QQuaternion::fromAxisAndAngle(_CameraNowUp, -200 * rot.x());
        _camera._up = rotation.rotatedVector(_CameraNowUp);
        _camera._position = rotation.rotatedVector(_CameraNowPos);
        _camera._front = rotation.rotatedVector(_CameraNowFront);

        rotation = QQuaternion::fromAxisAndAngle(QVector3D::crossProduct(-_CameraNowFront, _CameraNowUp).normalized(), 200 * rot.y());
        _camera._up = rotation.rotatedVector(_camera._up);
        _camera._position = rotation.rotatedVector(_camera._position);
        _camera._front = rotation.rotatedVector(_camera._front);

        update();
    }

    if (ev->buttons() & Qt::MidButton)
    {
        QVector2D move;
        move.setX((ev->x() - _wheelPressPos.x()) / width());
        move.setY((ev->y() - _wheelPressPos.y()) / height());

        _camera._position = _CameraNowPos +
            QVector3D::crossProduct(-_camera._front, _camera._up).normalized() * 3 *move.x()*_camera._position.length()/2
            + _camera._up.normalized() * 3 *move.y()*_camera._position.length()/2;

        update();
    }

}

void Scene::wheelEvent(QWheelEvent *ev)
{
    QPoint offset = ev->angleDelta();
    _camera.ScaleControl(offset.y()/20.0f);

    update();
}


