#pragma once
#include "../../Define.h"
#include "../Bullet/Bullet.h"

class Bullet;

class Enemy
{
public:
	Enemy(int A);
	~Enemy() {};

	void Update();
	void Draw();
	void ATHit(std::vector<Bullet*>& Bu);

	double PointToLine(Math::Vector2 A, Math::Vector2 B, Math::Vector2 C);
	bool isColliding(Math::Vector2 rectCenter,Math::Vector2 rectSize,float rectDeg,Math::Vector2 circlePos,float circleRadius);
	bool ChainHit(Math::Vector2 ChP);

	bool ChainF = false;
	int ChainCnt = 0;
	//始点・終点
	std::vector<Math::Vector2> ChainPosA;
	std::vector<Math::Vector2> ChainPosB;
	Enemy* CEnemy;
	std::vector<Enemy*> ChainEnemy;
	//チェインしているかのフラグ
	bool ChainB = false;
	float AutoDis =0;
	int DisCnt = 0;

	bool Exp = false;
	bool Death = false;
	//空間分割で当たり判定を行うため
	Cell EHitJ;

	//ウイルス
	bool Virus = false;
	int VirusCnt = 0;
	//毒
	bool Poizon = false;
	int PoizonCnt = 0;

	int HP = 200;
private:
	int ATK;
	int DEF;
	
	int Dmg = 0;

	bool Hit = false;
	
	int ExpCnt = 0;

	KdTexture* ETex;
	Math::Rectangle rect;

	Math::Matrix Scale;
	Math::Matrix Rota;
	Math::Matrix Trans;
	Math::Matrix Mat;

	void MatSet();

	Math::Vector2 Main;
	Math::Vector2 Pos;
	Math::Vector2 Move;
	Math::Vector2 PScale;

	long double Deg = 0;
	
	int ContiCnt = 0;
	//状態異常Update
	void Condisyon();

	//死亡時　基本はExpをtrueにするボスだけ例外処理
	void DeathUpdate(Enemy* A);
public:
	Math::Vector2 GetPos() { return Pos; }
};
