#include "DCEL.h"


void Face::AddLoop(Loop* loop)
{
    if (this->LoopList() == nullptr)
    {
        this->SetLoopList(loop);
    }
    else
    {
        Loop* temp = this->LoopList();
        while (temp->NextLoop() != nullptr)
        {
            temp = temp->NextLoop();
        }
        temp->SetNextLoop(loop);
        loop->SetPreLoop(temp);
    }

    loop->SetFace(this);
}

void Solid::AddFace(Face* face)
{
    if (this->FaceList() == nullptr)
    {
        this->SetFaceList(face);
    }
    else
    {
        Face* temp = this->FaceList();
        while (temp->nextFace() != nullptr)
        {
            temp = temp->nextFace();
        }
        temp->SetNextFace(face);
        face->SetPreFace(temp);
    }

    face->SetSolid(this);
}

void Solid::AddEdge(Edge* edge)
{
    if (this->EdgeList() == nullptr)
    {
        this->SetEdgeList(edge);
    }
    else
    {
        Edge* temp = this->EdgeList();
        while (temp->NextEdge() != nullptr)
        {
            temp = temp->NextEdge();
        }
        temp->SetNextEdge(edge);
        edge->SetPreEdge(temp);
    }
}
