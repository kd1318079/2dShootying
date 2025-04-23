#pragma once
#include "../../Define.h"
#include "../Enemy/Enemy.h"

class Enemy;
class Bullet
{
public:
	Bullet(int A);
	~Bullet() {}
	
	void Draw();
	void Update();

	int BulletType = 0;

	bool BurstF = false;
	bool FireF = false;
	bool GraF = false;
	bool SDF = false;

	int GraCnt = 0;

	bool HomingF = false;
	int HomingCnt = 0;
	Math::Vector2 HomingE = {0,0};
	Enemy* HomingEnemy;
	Math::Vector2 HomingDir = { 0,0 };

	bool VirusF = false;
	bool MoonF = false;
	bool SunF = false;
	bool PoizonF = false;
private:
	float AutoDeg = 0;
	//èâä˙ílÇÃê›íË //à⁄ìÆó ÇÃê›íË
	void ATStraight(Math::Vector2 A);
	void ATFast(Math::Vector2 A);
	void ATMissile(Math::Vector2 A);
	void ATAuto(Math::Vector2 A);
	void ATHighPower(Math::Vector2 A);
	void ATBurst(Math::Vector2 A);
	void ATBurstP(Math::Vector2 A);
	void ATPenetration(Math::Vector2 A);
	void ATCharge(Math::Vector2 A);
	void ATFire(Math::Vector2 A);
	void ATLaesr(Math::Vector2 A);
	void ATGravity(Math::Vector2 A);
	void ATHoming(Math::Vector2 A);
	void ATRailGun(Math::Vector2 A);
	void ATChain(Math::Vector2 A);
	void ATVirus(Math::Vector2 A);
	void ATMoon(Math::Vector2 A);
	void ATCommet(Math::Vector2 A);
	void ATSun(Math::Vector2 A);
	void ATPoizon(Math::Vector2 A);
	void ATConti(Math::Vector2 A);
	void ATSDust(Math::Vector2 A);
	void ATSLight(Math::Vector2 A);

	int TexNo = 0;
	int Attack = 1;
	
	float PlayerDeg = 0;
	float DirDeg = 0;

	Math::Vector2 M;
	Math::Vector2 Move = { 0,0 };

	float BurstDeg = 0;
	int BurstCnt = 0;
	int SDCnt = 0;

	void DecreCh();

	int FireCnt = 0;

	//ãÛä‘ï™äÑÇ≈ìñÇΩÇËîªíËÇçsÇ§ÇΩÇﬂ
	Cell BHitJ;

	KdTexture* BTex;
	Math::Rectangle rect;

	Math::Matrix Scale;
	Math::Matrix Rota;
	Math::Matrix Trans;
	Math::Matrix Mat;

	void MatSet();

	Math::Vector2 Main = { 0,0 };
	Math::Vector2 Pos = { 0,0 };
	Math::Vector2 PScale = { 1,1 };

	long double Deg = 0;
	long double MemDeg = 0;
	long double MoveDeg = 0;
	
	bool MissileLeft = false;
	int MissileCnt = 0;
	const float MissileSpd = 1.5;
	Math::Vector2 MoveRad;

public:
	Math::Vector2 GetPos() { return Pos; };
	int GetATK() { return Attack; };
	Cell GetCell() { return BHitJ; }
};
