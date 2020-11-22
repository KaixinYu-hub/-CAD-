#include "Polygon.h"


const char *PolyVertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "void main(){\n"
        "  gl_Position = projection * view * model *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";

const char *PolyFragmentShaderSource =
        "#version 330 core\n"
        "uniform vec4 incolor;"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = incolor;\n"
        "}\n\0";


myPolygon::myPolygon()
{

}

myPolygon* myPolygon::tessPoly=nullptr;

myPolygon::~myPolygon()
{

}

void myPolygon::setLoopVertexes(float* Vertexes, int* PtNumOfLoop,int LoopNum )
{
    int sum=0;
    for(int i=0;i<LoopNum;i=i+1)
    {
        int currentNum=PtNumOfLoop[i];//当前环的顶点数


        std::vector<Point3d>temp;
        Loops.push_back(temp);
        for(int j=sum;j<sum+currentNum*3;j=j+3)
        {
            //把当前这个环的顶点塞到Loops里面去
            Loops.back().push_back(Point3d(Vertexes[j],Vertexes[j+1],Vertexes[j+2]));
        }
        sum=sum+currentNum*3;
    }

}


void myPolygon::initialize()
{
    this->initializeOpenGLFunctions();
    tessPoly=this;

    //生成一个tessellator对象
    GLUtesselator * tesslator;
    tesslator = gluNewTess();

    //回调函数
    gluTessCallback(tesslator, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
    gluTessCallback(tesslator, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
    gluTessCallback(tesslator, GLU_TESS_END, (void (CALLBACK *)())endCallback);
    gluTessCallback(tesslator, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);

    gluTessBeginPolygon(tesslator, nullptr);

    //统计环的顶点数
    int VertexNum=0;
    for(const auto & i : Loops)
    {
        VertexNum+=i.size();
    }

    GLdouble* temp=new GLdouble[VertexNum*3];

    VertexNum=0;
    for(const auto & i:Loops)
    {
        //遍历每一个环的顶点
        gluTessBeginContour(tesslator);

        for(const auto & j:i)
        {
            temp[VertexNum*3]=j.x;
            temp[VertexNum*3+1]=j.y;
            temp[VertexNum*3+2]=j.z;
            gluTessVertex(tesslator,&temp[VertexNum*3],&temp[VertexNum*3]);
            ++VertexNum;
        }

        gluTessEndContour(tesslator);
    }

    gluTessEndPolygon(tesslator);

    delete []temp;
    gluDeleteTess(tesslator);

    PolyShader.compile(PolyVertexShaderSource,PolyFragmentShaderSource);

    GLuint VBO,VAO;
    for(int i=0;i<tessVertexes.size();i++)
    {
        //创建一个VBO并绑定VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //创建一个VAO并绑定VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //把之前定义的顶点数据复制到缓冲的内存
        glBufferData(GL_ARRAY_BUFFER, tessVertexes[i].size() * sizeof(float), &tessVertexes[i][0], GL_STATIC_DRAW);

        //设置顶点属性指针，从VBO中“挖”出对应属性的数据
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        //放入VAOs和VBOs中
        PolyVBOs.push_back(VBO);
        PolyVAOs.push_back(VAO);

        //解绑
        glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑VBO
        glBindVertexArray(0);//解绑VAO
    }


    //初始化各个矩阵
    PolyShader.use();
    PolyShader.setMatrix4f("model",model);
    view.translate(QVector3D(0.0f, 0.0f, -5.0f));
    PolyShader.setMatrix4f("view", view);
}

void myPolygon::paint()
{
    PolyShader.use();
    for(int i=0;i<PolyVAOs.size();i++)
    {
        glBindVertexArray(PolyVAOs[i]);

        //填充颜色
        PolyShader.setVector4f("incolor",0,0,0,1);
        // 设置正面为填充模式
        glPolygonMode(GL_FRONT, GL_FILL);
        // 设置反面为线形模式
        glPolygonMode(GL_BACK, GL_FILL);

        glDrawArrays(drawMode[i], 0, tessVertexes[i].size() / 3);


    }

    PolyShader.setVector4f("incolor",1,0,0,1);


    for(int i=0;i<Loops.size();i++)
    {
        Point3d first=Loops[i][0];
        Point3d p1,p2;
        for(int j=1;j<Loops[i].size();j++)
        {
            p1=Loops[i][j-1];
            p2=Loops[i][j];

            glBegin(GL_LINE_STRIP);
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
            glEnd();
        }
        glBegin(GL_LINE_STRIP);
        glVertex3f(first.x,first.y,first.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glEnd();
    }

}

void myPolygon::updateView(QMatrix4x4 mat)
{
    view=mat;
    PolyShader.setMatrix4f("view", view);
}

void myPolygon::updateProj(QMatrix4x4 mat)
{
    projection=mat;
    PolyShader.setMatrix4f("projection", projection);
}

void CALLBACK beginCallback(GLenum type)
{
    myPolygon::tessPoly->drawMode.push_back(type);
    myPolygon::tessPoly->tessVertexes.push_back(std::vector<float>());
}


void CALLBACK vertexCallback(GLvoid* vertex)
{
    double *pt=(double*)vertex;
    myPolygon::tessPoly->tessVertexes.back().push_back(pt[0]);
    myPolygon::tessPoly->tessVertexes.back().push_back(pt[1]);
    myPolygon::tessPoly->tessVertexes.back().push_back(pt[2]);
}


void CALLBACK endCallback()
{

}

void CALLBACK errorCallback(GLenum errorCode)
{
    qDebug() << "error:" << errorCode;
}



