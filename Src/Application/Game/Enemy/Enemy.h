#pragma once
#include "../../Define.h"
#include "../Bullet/Bullet.h"
#include "../NumDraw.h"
#include "../Bullet/EnemyBullet.h"

class Bullet;

class Enemy
{
public:
	Enemy(int A);
	Enemy(int A,Math::Vector2 B);
	~Enemy() {};

	//行動パターン
	EnemyState EState = EnemyState::Return;
	//行動変化タイマー
	int Timer = 2000;

	void Update();
	void Draw();
	void ATHit(std::vector<Bullet*>& Bu);

	double PointToLine(Math::Vector2 A, Math::Vector2 B, Math::Vector2 C);
	bool isColliding(Math::Vector2 rectCenter,Math::Vector2 rectSize,float rectDeg,Math::Vector2 circlePos,float circleRadius);
	bool ChainHit(Math::Vector2 ChP);


	bool BulletF1 = false;
	Math::Vector2 MovePlas;



	bool ChainF = false;
	int ChainCnt = 0;
	//始点・終点
	std::vector<Math::Vector2> ChainPosA;
	std::vector<Math::Vector2> ChainPosB;

	std::vector<NumDraw*> Dmgnum;
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
	bool Virus1 = false;
	int VirusCnt = 0;
	//毒
	bool Poizon = false;
	int PoizonCnt = 0;

	bool GravityF = false;


	int cost = 0;
	int HP = 2000;
private:
	int TypeNum = 0;
	bool Boss = false;

	int ScoreP = 100;
	int ATK;
	int DEF;
	int Size = 64;


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

	Math::Vector2 Pos;
	Math::Vector2 Move;
	Math::Vector2 PScale;

	long double Deg = 0;
	
	int ContiCnt = 0;

	Math::Vector2 PosSet();
	//状態異常Update
	void Condisyon();

	//死亡時　基本はExpをtrueにするボスだけ例外処理
	void DeathUpdate(Enemy* A);

	void E_Update(int i);

	bool MoveUpdate(int i);
	bool AttackUpdate(int i);
	bool ReturnUpdate(int i);

	void ChangeState(EnemyState E, const int SetTimer = 800);

	void Update0();
	void Update1();
	void Update2();
	void Update3();
	void Update4();
	void Update5();
	//ボス
	void Update6();
	void Update7();
	void Update8();



	void Atk0();		//止まって攻撃
	void Atk1();		//引っ張って飛んでいく
	void Atk2();		//なにもしない
	void Atk3();		//5の敵を生み出す　※永続
	void Atk4();		//
	void Atk5();
	void Atk6();
	void Atk7();
	void Atk8();

	Math::Vector2 Move0();
	Math::Vector2 Move1(const bool A = false);
	Math::Vector2 Move2(const float Spd = 1.2);
	Math::Vector2 Move3();
	Math::Vector2 Move6();
	Math::Vector2 Move7();
	Math::Vector2 Move8();

	float DegSpeed = 10;

	int DegC;
	bool ERe = false;
	bool BossF = false;

	void SetStetas(int A);
public:
	Math::Vector2 GetPos() { return Pos; }
};
