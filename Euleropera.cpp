#include "Euleropera.h"

Solid* mvfs(double Pt[3], Vertex* &v)
{
    Solid *solid = new Solid();
    Face *face = new Face();
    Loop* loop = new Loop();
    v = new Vertex(Pt[0], Pt[1], Pt[2]);

    solid->SetFaceList(face);

    face->SetSolid(solid);
    face->SetLoopList(loop);

    loop->SetFace(face);

    return solid;
}

//set relationship between vertex, halfedges and edge
//start from v1->v2
//left hand rule
void SetEdgeRelation(Edge* &edge, HalfEdge* &LeftHalf, HalfEdge* &RightHalf, Vertex* &v1, Vertex* &v2)
{
    //relationship between edge and halfedges
    edge->SetLeftHalf(LeftHalf);
    edge->SetRightHalf(RightHalf);
    LeftHalf->SetEdge(edge);
    RightHalf->SetEdge(edge);

    //relationship between halfedges
    LeftHalf->SetNeighber(RightHalf);
    RightHalf->SetNeighber(RightHalf);

    //relationship between vertex and halfedges
    //start from v1->v2
    //left hand rule
    LeftHalf->SetStartV(v1);
    LeftHalf->SetEndV(v2);
    RightHalf->SetStartV(v2);
    RightHalf->SetEndV(v1);
}

HalfEdge* mev(Vertex* v1, double Pt[3], Loop* loop)
/*
v1: Existing point
Pt[3]: input point
loop: Existing loop
*/
{
    Solid* solid = loop->face()->solid();
    HalfEdge* LeftHalf = new HalfEdge();
    HalfEdge* RightHalf = new HalfEdge();
    Edge* edge = new Edge();
    Vertex* v2 = new Vertex(Pt[0], Pt[1], Pt[2]);

    //set relationship between vertex, halfedges and edge
    SetEdgeRelation(edge, LeftHalf, RightHalf, v1, v2);

    //Attach the halfedges to the loop
    LeftHalf->SetLoop(loop);
    RightHalf->SetLoop(loop);

    //only one point
    if (loop->halfEdges() == nullptr)
    {
        LeftHalf->SetNextHalf(RightHalf);
        LeftHalf->SetPreHalf(RightHalf);

        RightHalf->SetNextHalf(LeftHalf);
        RightHalf->SetPreHalf(LeftHalf);

        loop->SetHalfEdges(LeftHalf);
    }
    //other cases
    else
    {
        HalfEdge *TempHalf = loop->halfEdges();
        while ((*TempHalf->EndV()) != *v1)TempHalf = TempHalf->NextHalf();

        RightHalf->SetNextHalf(TempHalf->NextHalf());
        TempHalf->NextHalf()->SetPreHalf(RightHalf);

        TempHalf->SetNextHalf(LeftHalf);
        LeftHalf->SetPreHalf(TempHalf);

        LeftHalf->SetNextHalf(RightHalf);
        RightHalf->SetPreHalf(LeftHalf);
    }

    solid->AddEdge(edge);

    return LeftHalf;
}

//make en edge start from v1->v2, left hand rule
//make a face be the neighber of old face
Loop* mef(Vertex* v1, Vertex* v2, Loop* loop)
{
    Face* newFace = new Face();
    Loop* newLoop = new Loop();
    Edge *edge = new Edge();
    HalfEdge* LeftHalf = new HalfEdge();
    HalfEdge* RightHalf = new HalfEdge();

    //set relationship between vertex, halfedges and edge
    SetEdgeRelation(edge, LeftHalf, RightHalf, v1, v2);

    HalfEdge* tempHalf = loop->halfEdges();
    HalfEdge* temp1 = nullptr;
    HalfEdge* temp2 = nullptr;


    while ((*(tempHalf->EndV())) != (*v1))
    {
        tempHalf = tempHalf->NextHalf();
    }

    temp1 = tempHalf;

    while ((*(tempHalf->EndV())) != (*v2))
    {
        tempHalf = tempHalf->NextHalf();
    }
    temp2 = tempHalf;

    /*split the loop to two faces*/
    //
    loop->SetHalfEdges(LeftHalf);
    LeftHalf->SetLoop(loop);

    newLoop->SetHalfEdges(RightHalf);
    RightHalf->SetLoop(newLoop);

    //
    RightHalf->SetNextHalf(temp1->NextHalf());
    temp1->SetNextHalf(RightHalf);

    LeftHalf->SetPreHalf(temp1);
    temp1->SetNextHalf(LeftHalf);

    LeftHalf->SetNextHalf(temp2->NextHalf());
    temp2->NextHalf()->SetPreHalf(LeftHalf);

    temp2->SetNextHalf(RightHalf);
    RightHalf->SetPreHalf(temp2);

    //new inner face
    newFace->AddLoop(newLoop);
    loop->face()->solid()->AddFace(newFace);
    loop->face()->solid()->AddEdge(edge);

    return loop;

}

void kemr(Vertex* v1, Vertex* v2, Loop* loop)
{
    HalfEdge* temp1 = nullptr;
    HalfEdge* temp2 = nullptr;

    Loop* newLoop = new Loop();

    HalfEdge* tempHalf = loop->halfEdges();

    while (*(tempHalf->StartV()) != *v1 || *(tempHalf->EndV()) != *v2)tempHalf = tempHalf->NextHalf();
    temp1 = tempHalf;

    while (*(tempHalf->StartV()) != *v2 || *(tempHalf->EndV()) != *v1)tempHalf = tempHalf->NextHalf();
    temp2 = tempHalf;

    temp1->PreHalf()->SetNextHalf(temp2->NextHalf());
    temp2->PreHalf()->SetNextHalf(temp1->NextHalf());

    //set inner loop
    newLoop->SetHalfEdges(temp2->PreHalf());
    temp2->PreHalf()->SetLoop(newLoop);
    loop->face()->AddLoop(newLoop);


    //set out loop
    //set the first loop of solid 's halfedge
    loop->face()->solid()->FaceList()->LoopList()->SetHalfEdges(temp1->PreHalf());

    delete temp1;
    delete temp2;

}

//kill the face fin inside the face fout, make a whole and an innerloop
void kfmrh(Face *fout, Face *fin)
{
    Loop*loop = fin->LoopList();
    fout->AddLoop(loop);

    Face* faceList = fout->solid()->FaceList();

    if (faceList == nullptr)
    {
        fout->solid()->SetFaceList(faceList->nextFace());
    }
    else
    {
        Face* temp = faceList;
        while (faceList != fin && faceList != nullptr)
        {
            temp = faceList;
            faceList = faceList->nextFace();
        }
        temp->SetNextFace(faceList->nextFace());
    }
    delete fin;
}


void sweep(Solid* solid, double direction[3], double delta)
{
    Face* faceList = solid->FaceList();
    Loop* loopList = faceList->LoopList();
    HalfEdge* tempHalf = nullptr;
    HalfEdge* HalfIt = nullptr;
    Vertex* PreUp = nullptr;
    Vertex* nextV = nullptr;
    double Pt[3];

    Loop* lpit = loopList;

    while (lpit != nullptr)
    {
        Vertex* firstV = lpit->halfEdges()->StartV();

        Pt[0] = firstV->x() + direction[0] * delta;
        Pt[1] = firstV->y() + direction[1] * delta;
        Pt[2] = firstV->z() + direction[2] * delta;

        tempHalf = mev(firstV, Pt, lpit);

        Vertex* firstUp = tempHalf->EndV();

        Vertex* Up = tempHalf->EndV();

        PreUp = Up;

        nextV = lpit->halfEdges()->NextHalf()->StartV();

        HalfIt = lpit->halfEdges()->NextHalf();

        while ((*nextV) != (*firstV))
        {
            Pt[0] = nextV->x() + direction[0] * delta;
            Pt[1] = nextV->y() + direction[1] * delta;
            Pt[2] = nextV->z() + direction[2] * delta;

            tempHalf = mev(nextV, Pt, lpit);

            Up = tempHalf->EndV();

            mef(PreUp, Up, lpit);

            PreUp = Up;

            nextV = HalfIt->NextHalf()->StartV();

            HalfIt = HalfIt->NextHalf();
        }

        mef(PreUp, firstUp, lpit);

        lpit = lpit->NextLoop();
    }


}


