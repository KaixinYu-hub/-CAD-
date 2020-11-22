#ifndef EULEROPERA_H
#define EULEROPERA_H
#include "DCEL.h"

//Euler Operator and Sweep

Solid* mvfs(double Pt[3], Vertex* &v);

HalfEdge* mev(Vertex* v1, double Pt[3], Loop* loop);

Loop* mef(Vertex* v1, Vertex* v2, Loop* loop);

void kemr(Vertex* v1, Vertex* v2, Loop* loop);

void kfmrh(Face *fout, Face *fin);

void sweep(Solid* solid, double direction[3], double delta);

Solid* makeCubeSolid(double Pt[3],double Length);

#endif // EULEROPERA_H
