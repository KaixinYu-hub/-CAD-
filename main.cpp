#include "Scene.h"

#include <QApplication>
#include "Euleropera.h"
#include "DCEL.h"

void makeSqureHole(double Pt1[], double Pt2[], double Pt3[], double Pt4[],Vertex*v ,Solid* solid)
{
    HalfEdge* he = nullptr;
    he = mev(v, Pt1, solid->FaceList()->LoopList());
    Vertex* v2 = he->EndV();

    he = mev(he->EndV(), Pt2, solid->FaceList()->LoopList());

    he = mev(he->EndV(), Pt3, solid->FaceList()->LoopList());

    he = mev(he->EndV(), Pt4, solid->FaceList()->LoopList());
    Vertex* v1 = he->EndV();

    mef(v1, v2, solid->FaceList()->LoopList());

    kemr(v, v2, solid->FaceList()->LoopList());

    kfmrh(solid->FaceList()->nextFace(), solid->FaceList()->nextFace()->nextFace());
}

Solid *makeCubeSolid(double Pt[], double Length)
{
    double Pt1[3]={Pt[0],Pt[1],Pt[2]};
    Vertex* v = nullptr;
    Solid *solid = mvfs(Pt, v);
    HalfEdge* he = nullptr;

    double Pt2[3] = { Pt[0]+Length,Pt[1],Pt[2]};
    he = mev(v, Pt2, solid->FaceList()->LoopList());

    double Pt3[3] = { Pt[0]+Length,Pt[1]+Length,Pt[2]};
    he = mev(he->EndV(), Pt3, solid->FaceList()->LoopList());

    double Pt4[3] = { Pt[0],Pt[1]+Length,Pt[2]};
    he = mev(he->EndV(), Pt4, solid->FaceList()->LoopList());

    mef(he->EndV(), v, he->loop());

    //the first innerloop
    double Pt5[3] = { 2,2,0 };

    double Pt6[3] = { 2,5,0 };

    double Pt7[3] = { 5,5,0 };

    double Pt8[3] = { 5,2,0 };

    makeSqureHole(Pt5, Pt6, Pt7, Pt8, v, solid);

    //the second innerloop
    double Pt9[3] = { 6,6,0 };

    double Pt10[3] = { 6,9,0 };

    double Pt11[3] = { 9,9,0 };

    double Pt12[3] = { 9,6,0 };

    makeSqureHole(Pt9, Pt10, Pt11, Pt12, v, solid);

    //the Third innerloop
    double Pt13[3] = { 2,6,0 };

    double Pt14[3] = { 2,9,0 };

    double Pt15[3] = { 5,9,0 };

    double Pt16[3] = { 5,6,0 };

    makeSqureHole(Pt13, Pt14, Pt15, Pt16, v, solid);

    //the forth innerloop
    double Pt17[3] = { 6,2,0 };

    double Pt18[3] = { 6,5,0 };

    double Pt19[3] = { 9,5,0 };

    double Pt20[3] = { 9,2,0 };

    makeSqureHole(Pt17, Pt18, Pt19, Pt20, v, solid);

    //sweep
    //default direction
    double direction[3] = { 0,0,1 };

    sweep(solid, direction, Length);

    return solid;
}

void solidToPolygon(Solid*solid, Scene&scene)
{
    Face* fc = solid->FaceList();

    //遍历每一个面
    while(fc!=nullptr)
    {
        Loop* lpit=fc->LoopList();

        std::vector<float>Vertices;//用于记录该面的所有环的顶点数组
        std::vector<int>PtNumOfLoop;//用于记录该面的每个环有几个点
        int LoopNum=0;//记录这个面有几个环

        //遍历每个面的每个环
        while(lpit!=nullptr)
        {
            HalfEdge* he = lpit->halfEdges();
            HalfEdge* firstHe = he;
            int countPtNum=0;
            do
            {
                countPtNum++;
                Vertices.push_back(he->StartV()->x());
                Vertices.push_back(he->StartV()->y());
                Vertices.push_back(he->StartV()->z());

                he=he->NextHalf();
            }while(he!=firstHe);

            PtNumOfLoop.push_back(countPtNum);
            LoopNum++;
            lpit=lpit->NextLoop();
        }

        scene.AddPolygon(&Vertices[0],&PtNumOfLoop[0],LoopNum);

        Vertices.clear();
        PtNumOfLoop.clear();
        fc=fc->nextFace();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Scene scene;


    double Pt[3]={0,0,0};
    Solid*solid=makeCubeSolid(Pt,10);

    solidToPolygon(solid,scene);

    scene.show();

    return a.exec();
}
