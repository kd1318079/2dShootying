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
	void PreDraw();
	void Release();

	static const int PartsMax = 15;
	char PlayerParts[PartsMax][PartsMax];


	std::vector<Bullet*> MainBullet;
	std::vector<Bullet*> SubBullet;
	std::vector<Bullet*> SPBullet;
	
	std::vector<Bullet*> BurstBullet;

	std::vector<Explosion*> BulletExp;

	Math::Vector2 ExpPos = { 0,0 };
	//�e�̎��
	int BulletCnt = 30;			//����
	int SubBulletCnt = Straight;	//�蓮
	int SPBulletCnt = Laesr;	//����

	int MissileCnt = 0;
	Math::Vector2 SetMove(Math::Vector2 A) { Move = A; };

	//��ԕ����œ����蔻����s������
	Cell HitJ;

	/////////////////////////////////////////
	//�����Ə��ň�ԋ߂��G�ւ̎��@����� ���� �p�x �X�s�[�h �X�s�[�hXY
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
	KdTexture* DegTex;
	KdTexture BackTex;
	POINT mousePos;

	void MouseGet();

	void MovePlayer();
	void PlayerDegSet();
	void PlayerAttack();
	void ExplosionUpdate();
	void SetMat();

	float PlayerSpd = 1; 

	//�����U���̃t���O
	bool AutoAttack = false;

	//�e�̃N�[���^�C��
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

	//Matrix
	Math::Matrix Scale;
	Math::Matrix Rota;
	Math::Matrix Trans;

	//player
	Math::Matrix Mat;
	Math::Matrix AimMat;
	Math::Matrix DegMat;

	//�v���C���[���W
	Math::Vector2 Pos = {0,0};
	Math::Vector2 Move = {0,0};
	Math::Vector2 PScale = { 0.05,0.05 };
	Math::Vector2 DegScale = { 0.15,0.65 };
	Math::Vector2 AimeScale = { 1,1 };

	float PlayerDeg = 0;
	float AimeDeg = 0;

	float DegSpeed = 0.5;

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
	Math::Vector2 GetMove() { return Move; }
	Math::Vector2 GetMousePos() 
	{
		Math::Vector2 A = { (float)mousePos.x,(float)mousePos.y};
		return A; 
	}
	static void CellGet(Math::Vector2 A,Cell* B);
};

#define CELLGET(A, B) C_Player::CellGet(A, B)
