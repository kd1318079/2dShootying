#pragma once
#include "../../Define.h"

class EnemyBullet
{
public:
	EnemyBullet(Math::Vector2 P,int A , int Atk, float SPD);
	EnemyBullet(Math::Vector2 P,int A , int Atk, float SPD ,float deg);
	~EnemyBullet() {};

	void Draw();
	void Update();
		 
	Math::Vector2 GetPos() { return Pos; }
	bool Exp = false;
private:
	int EBType = 0;
	int ATK = 0;



	//‹óŠÔ•ªŠ„‚Å“–‚½‚è”»’è‚ğs‚¤‚½‚ß
	Cell EBHitJ;

	KdTexture* BTex;
	Math::Rectangle rect;

	Math::Matrix Scale;
	Math::Matrix Rota;
	Math::Matrix Trans;
	Math::Matrix Mat;

	void MatSet();

	Math::Vector2 Pos = { 0,0 };
	Math::Vector2 Move;
	Math::Vector2 PScale = { 1,1 };

	long double Deg = 0;

};