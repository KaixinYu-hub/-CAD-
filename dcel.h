#ifndef DCEL_H
#define DCEL_H

#include <iostream>

class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;

class Vertex
{
public:
    //constructor
    Vertex(double x, double y, double z) :_ID(0), _x(x), _y(y), _z(z), _preV(nullptr), _nextV(nullptr) {}

    //read only
    int ID() const { return _ID; }
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
    Vertex *preV() const { _preV; }
    Vertex *nextV() const { _nextV; }

    //set value
    void SetX(int x) { _x = x; }
    void SetY(int y) { _y = y; }
    void SetZ(int z) { _z = z; }

    bool operator !=(Vertex v)
    {
        if ((*this).x() != v.x() || (*this).y() != v.y() || (*this).z() != v.z())
        {
            return true;
        }
        else
        {
            return false;
        }
    }


private:
    int _ID;
    double _x, _y, _z;
    Vertex* _preV;
    Vertex* _nextV;
};

class HalfEdge
{
public:
    //constructor
    HalfEdge() :_loop(nullptr), _Edge(nullptr), _neighber(nullptr)
        , _preH(nullptr), _nextH(nullptr), _StartV(nullptr), _EndV(nullptr) {}

    //read only
    Loop* loop()const { return _loop; }
    Edge* edge()const { return _Edge; }
    HalfEdge* neighber()const { return _neighber; }
    HalfEdge* PreHalf()const { return _preH; }
    HalfEdge* NextHalf()const { return _nextH; }
    Vertex* StartV()const { return _StartV; }
    Vertex* EndV()const { return _EndV; }

    //set value
    void SetLoop(Loop* loop) { _loop = loop; }
    void SetEdge(Edge* Edge) { _Edge = Edge; }
    void SetNeighber(HalfEdge* neighber) { _neighber = neighber; }
    void SetPreHalf(HalfEdge* preH) { _preH = preH; }
    void SetNextHalf(HalfEdge* nextH) { _nextH = nextH; }
    void SetStartV(Vertex *StartV) { _StartV = StartV; }
    void SetEndV(Vertex *EndV) { _EndV = EndV; }

private:
    Loop* _loop;
    Edge* _Edge;
    HalfEdge* _neighber;
    HalfEdge* _preH;
    HalfEdge* _nextH;
    Vertex* _StartV;
    Vertex* _EndV;
};

class Edge
{
public:
    //constructor
    Edge() :_LeftHalf(nullptr), _RightHalf(nullptr), _preE(nullptr), _nextE(nullptr) {}

    //read only
    HalfEdge* LeftHalf()const { return _LeftHalf; }
    HalfEdge* RightHalf()const { return _RightHalf; }
    Edge* PreEdge()const { return _preE; }
    Edge* NextEdge()const { return _nextE; }

    //set value
    void SetLeftHalf(HalfEdge* LeftHalf) { _LeftHalf = LeftHalf; }
    void SetRightHalf(HalfEdge* RightHalf) { _RightHalf = RightHalf; }
    void SetPreEdge(Edge* preE) { _preE = preE; }
    void SetNextEdge(Edge* nextE) { _nextE = nextE; }

private:
    HalfEdge* _LeftHalf;
    HalfEdge* _RightHalf;
    Edge* _preE;
    Edge* _nextE;
};

class Loop
{
public:
    //constructor
    Loop() :_halfEdges(nullptr), _face(nullptr), _preL(nullptr), _nextL(nullptr) {}

    //read only
    HalfEdge *halfEdges()const { return _halfEdges; }
    Face *face()const { return _face; }
    Loop *PreLoop()const { return _preL; }
    Loop *NextLoop()const { return _nextL; }

    //set value
    void SetHalfEdges(HalfEdge* halfEdges) { _halfEdges = halfEdges; }
    void SetFace(Face* face) { _face = face; }
    void SetPreLoop(Loop* preL) { _preL = preL; }
    void SetNextLoop(Loop* nextL) { _nextL = nextL; }

private:
    HalfEdge *_halfEdges;
    Face* _face;
    Loop* _preL;
    Loop* _nextL;
};

class Face
{
public:
    //constructor
    Face() :_solid(nullptr), _preF(nullptr), _nextF(nullptr),_LoopList(nullptr) {}

    //read only
    Solid* solid()const { return _solid; }
    Face* preFace()const { return _preF; }
    Face* nextFace()const { return _nextF; }
    Loop* LoopList()const { return _LoopList; }

    //set value
    void SetSolid(Solid* solid) { _solid = solid; }
    void SetPreFace(Face* preF) { _preF = preF; }
    void SetNextFace(Face* nextF) { _nextF = nextF; }
    void SetLoopList(Loop* LoopList) { _LoopList = LoopList; }

    void AddLoop(Loop* loop);

private:
    Solid* _solid;
    Face* _preF;
    Face* _nextF;
    Loop* _LoopList;
};

class Solid
{
public:
    //constructor
    Solid() :_preS(nullptr), _nextS(nullptr), _FaceList(nullptr), _EdgeList(nullptr) {}

    //read only
    Solid* preS()const { return _preS; }
    Solid* nextS()const { return _preS; }
    Face* FaceList()const { return _FaceList; }
    Edge* EdgeList()const { return _EdgeList; }

    //set value
    void SetFaceList(Face* FaceList) { _FaceList = FaceList; }
    void SetEdgeList(Edge* EdgeList) { _EdgeList = EdgeList; }

    void AddFace(Face* face);
    void AddEdge(Edge* edge);

private:
    Solid* _preS;
    Solid* _nextS;
    Face* _FaceList;
    Edge* _EdgeList;
};

#endif // DCEL_H
