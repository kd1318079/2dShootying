#pragma once
#include "../Bullet/Bullet.h"
#include "../Explosion/Explosion.h"
#include <memory>
#include "../../Define.h"

class C_Player
{
public:
	C_Player() {};
	~C_Player() {};

	void Init();
	void Update();
	void Draw();
	void Release();

	std::vector<Bullet*> MainBullet;
	std::vector<Bullet*> SubBullet;
	std::vector<Bullet*> SPBullet;
	
	std::vector<Bullet*> BurstBullet;

	std::vector<Explosion*> BulletExp;

	Math::Vector2 ExpPos = { 0,0 };
	//弾の種類
	int BulletCnt = 30;			//自動
	int SubBulletCnt = Fire;		//手動
	int SPBulletCnt = Gravity;	//特殊

	int MissileCnt = 0;
	Math::Vector2 SetMove(Math::Vector2 A) { Move = A; };

	//空間分割で当たり判定を行うため
	Cell HitJ;

	/////////////////////////////////////////
	//自動照準で一番近い敵への自機からの 距離 角度 スピード スピードXY
	float AutoDis = 0;
	float AutoDeg= 0;
	const float AutoSpd = 5;

	float AutoX = 0;
	float AutoY = 0;

	int DisCnt;
	int ECnt;
	/////////////////////////////////////////
	bool HighP = false;
	bool HighInit = false;
	int HighPCnt = 0;
	const float BackSpd = 0.8;
	Math::Vector2 BackMove = { 0,0 };

	bool ReCnt = false;

	const int BurstMax = 8;
	int BurstDeg = 0;
	Math::Vector2 BurstPos;

	int ChargeCnt = 0;
	int ChargeLv = 0;
	
	int LaesrCnt = 0;

	int ContiCnt = 0;
private:
	int HP = 10;
	int DEF = 5;
	int Energy = 100;

	int EnergyMax = 100;

	KdTexture* PlayerTex;
	KdTexture AimeTex;
	POINT mousePos;

	void MouseGet();

	void MovePlayer();
	void PlayerDegSet();
	void PlayerAttack();
	void ExplosionUpdate();
	void SetMat();

	float PlayerSpd = 0; 

	//自動攻撃のフラグ
	bool AutoAttack = false;

	//弾のクールタイム
	int MainBulletTime = 0;
	int SubBulletTime = 0;
	int SPBulletTime = 0;

	bool SPACE = false;

	bool MissileM = false;
	bool MissileS = false;

	void FireExpF();

	void LaesrFCnt();

	bool LaesrF = false;
	const int LeasrMax = 60;

	Math::Vector2 Scroll = {0,0};
	float scrolMAX = 2000;                     //右の限界
	float scrolMIN = -2000;                     //左の限界
	float scrolUP = 2000;                      //上の上限
	float scrolDOWN = -2000;                    //下の上限
	void ScrollMax();

	//Matrix
	Math::Matrix Scale;
	Math::Matrix Rota;
	Math::Matrix Trans;
	//player
	Math::Matrix Mat;
	//Aime
	Math::Matrix AimMat;

	//プレイヤー座標
	Math::Vector2 Main = {0,0};
	Math::Vector2 Pos = {0,0};
	Math::Vector2 Move = {0,0};
	Math::Vector2 PScale = { 1,1 };

	float PlayerDeg = 0;
	float AimeDeg = 0;
	
	bool SHIFT = false;
	bool SHIFT1 = false;
	int Count = 0;

	Math::Rectangle rect;

	void ATerase();
	int Attack(std::vector<Bullet*>& Bu,int A,int B);
	void Attack2(std::vector<Bullet*>& Bu,int A ,int* B ,bool* F);
	void AutoAT();

public:
	int		GetHP()			{ return HP; }
	int		GetDEF()		{ return DEF; }
	int		GetEnergy()		{ return Energy; }
	void	SetHP(int A)	{  HP = A; }
	void	SetDEF(int A)	{ DEF = A; }
	void	SetEnergy(int A){ Energy = A ; }


	float GetSpd() 	{return PlayerSpd; };
	float GetPDeg() 	{return PlayerDeg; };
	Math::Vector2 GetPos() { return Pos; }
	Math::Matrix GetMatrix() { return Mat; }
	Math::Vector2 GetMain() { return Main; }
	Math::Vector2 GetMove() { return Move; }
	Math::Vector2 GetScroll() { return Scroll; }
	Math::Vector2 GetMousePos() 
	{
		Math::Vector2 A = { (float)mousePos.x,(float)mousePos.y};
		return A; 
	}
	static void CellGet(Math::Vector2 A,Cell* B);
};

#define CELLGET(A, B) C_Player::CellGet(A, B)
