#pragma once
#include "../Bullet/Bullet.h"
#include "../Explosion/Explosion.h"
#include "../Bullet/EnemyBullet.h"
#include <memory>
#include "../../Define.h"

class C_Player
{
public:
	C_Player() {};
	~C_Player() {};


	//ゲーム開始時に変数を初期化する関数
	void ReturnInit();
	void Init();
	void Update();
	void Draw();
	void PreDraw();
	void Release();

	static const int PartsMax = 15;
	char PlayerParts[PartsMax][PartsMax];

	void EnemyB();


	std::vector<Bullet*> MainBullet;
	std::vector<Bullet*> SubBullet;
	std::vector<Bullet*> SPBullet;


	std::vector<EnemyBullet*> EBullet;

	std::vector<Bullet*> BurstBullet;

	std::vector<Explosion*> BulletExp;

	Math::Vector2 ExpPos = { 0,0 };

	//弾の数のvector 弾の型とクールタイムの管理
	std::vector<WaponBullet> MainBu;
	std::vector<WaponBullet> SubBu;

	int SPBulletCnt = Laesr;	//特殊
	int SPBulletTime = 0;

	//自分と弾のワープ処理
	bool Warp = false;
	bool BuWarp = false;



	bool DisPlayOutH(Math::Vector2 A);
	bool DisPlayOutW(Math::Vector2 A);

	int MissileCnt = 0;
	Math::Vector2 SetMove(Math::Vector2 A) { Move = A; };

	//空間分割で当たり判定を行うため
	Cell HitJ;

	/////////////////////////////////////////
	//自動照準で一番近い敵への自機からの 距離 角度 スピード スピードXY
	float AutoDis = 0;
	float AutoDeg = 0;
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

	int BurstMax = 8;
	int BurstDeg = 0;
	Math::Vector2 BurstPos;

	int ChargeCnt = 0;
	int ChargeLv = 0;
	int LaesrCnt = 0;
	int ContiCnt = 0;

	bool Alive = false;

	//弾初期攻撃力
	int BulletAT[BulletMax];
	//弾強化回数
	int BulletPower[BulletMax];
	//SP強化フラグ
	bool BulletSP[BulletMax];

	int Wave = 0;
	int Level = 0;
	int Score = 0;

	int LevelCnt = 0;
	KdTexture LevelUpTex;

	int LevelCnt1 = 0;
	int LevelCnt2 = 0;
	int LevelCnt3 = 0;

	void LevelUpdate(int i);

	bool SPBu = false;

	//取得している弾
	std::vector<int> GetBullet;

	//使用していない弾		※使用している弾はBulletの方に
	std::vector<int> OthersBullet;
	
	int Drone = 0;
	bool PowerUP(int A);
	
	int StaCnt = 0;
private:
	int MaxHP = 10;
	int HP = MaxHP;
	int DEF = 5;
	int Energy = 100;

	int EnergyMax = 100;

	static const int MaxLv = 1000;
	float EXP_Lv[MaxLv];
	float EXPSum;

	KdTexture* PlayerTex;
	KdTexture AimeTex;
	KdTexture* DegTex;
	KdTexture BackTex;
	POINT mousePos;

	void MouseGet();

	void MovePlayer();
	void PlayerDegSet();
	void PlayerAttack();
	void ExplosionUpdate();
	void SetMat();
	void LvUp();


	//自動攻撃のフラグ
	bool AutoAttack = false;



	bool SPACE = false;

	bool MissileM = false;
	bool MissileS = false;

	void FireExpF();

	void LaesrFCnt();

	bool LaesrF = false;
	int LeasrMax = 60;

	//Matrix
	Math::Matrix Scale;
	Math::Matrix Rota;
	Math::Matrix Trans;

	//player
	Math::Matrix Mat;
	Math::Matrix AimMat;
	Math::Matrix DegMat;

	//プレイヤー座標
	Math::Vector2 Pos = { 0,0 };
	Math::Vector2 Move = { 0,0 };
	Math::Vector2 PScale = { 0.05,0.05 };
	Math::Vector2 DegScale = { 0.15,0.65 };
	Math::Vector2 AimeScale = { 1,1 };

	float PlayerDeg = 0;
	float AimeDeg = 0;

	float PlayerSpd;
	float DegSpeed;

	bool SHIFT = false;
	bool SHIFT1 = false;
	int Count = 0;

	Math::Rectangle rect;

	void ATerase();
	int Attack(std::vector<Bullet*>& Bu, int A, int B);
	
	void AutoAT();

public:
	int		GetHP() { return HP; }
	int		GetMaxHP() { return MaxHP; }
	int		GetDEF() { return DEF; }
	int		GetEnergy() { return Energy; }
	void	SetHP(int A) { HP = A; }
	void	SetDEF(int A) { DEF = A; }
	void	SetEnergy(int A) { Energy = A; }


	float GetSpd() { return PlayerSpd; }
	float GetDegSpd() { return DegSpeed; }
	float GetPDeg() { return PlayerDeg; }
	Math::Vector2 GetPos() { return Pos; }
	Math::Matrix GetMatrix() { return Mat; }
	Math::Vector2 GetMove() { return Move; }
	Math::Vector2 GetMousePos()
	{
		Math::Vector2 A = { (float)mousePos.x,(float)mousePos.y };
		return A;
	}
	static void CellGet(Math::Vector2 A, Cell* B);
};

#define CELLGET(A, B) C_Player::CellGet(A, B)
