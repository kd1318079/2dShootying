#pragma once
#include "Num.h"

class NumDraw
{
public:
	NumDraw(Math::Vector2 A ,int B,const int Timer = 60);
	~NumDraw() {};

	Math::Vector2 Pos;
	int Dmg = 0;
	int Cnt = 0;
	void Draw(int ESize , const bool A = false);
	
private:
	KdTexture* NumTex;
	KdTexture* ScoreTex;

};

