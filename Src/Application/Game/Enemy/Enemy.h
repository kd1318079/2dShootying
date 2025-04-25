#pragma once
#include "../../Define.h"
#include "../Bullet/Bullet.h"
#include "../NumDraw.h"

class Bullet;

class Enemy
{
public:
	Enemy(int A);
	~Enemy() {};

	//�s���p�^�[��
	EnemyState EState = EnemyState::Return;
	//�s���ω��^�C�}�[
	int Timer = 600;

	void Update();
	void Draw();
	void ATHit(std::vector<Bullet*>& Bu);

	double PointToLine(Math::Vector2 A, Math::Vector2 B, Math::Vector2 C);
	bool isColliding(Math::Vector2 rectCenter,Math::Vector2 rectSize,float rectDeg,Math::Vector2 circlePos,float circleRadius);
	bool ChainHit(Math::Vector2 ChP);

	bool ChainF = false;
	int ChainCnt = 0;
	//�n�_�E�I�_
	std::vector<Math::Vector2> ChainPosA;
	std::vector<Math::Vector2> ChainPosB;

	std::vector<NumDraw*> Dmgnum;
	Enemy* CEnemy;
	std::vector<Enemy*> ChainEnemy;
	//�`�F�C�����Ă��邩�̃t���O
	bool ChainB = false;
	float AutoDis =0;
	int DisCnt = 0;

	bool Exp = false;
	bool Death = false;
	//��ԕ����œ����蔻����s������
	Cell EHitJ;

	//�E�C���X
	bool Virus = false;
	int VirusCnt = 0;
	//��
	bool Poizon = false;
	int PoizonCnt = 0;

	int HP = 2000;

	bool GravityF = false;
private:
	int TypeNum = 0;
	bool Boss = false;

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
	//��Ԉُ�Update
	void Condisyon();

	//���S���@��{��Exp��true�ɂ���{�X������O����
	void DeathUpdate(Enemy* A);

	void E_Update(int i);

	bool MoveUpdate(int i);
	bool AttackUpdate(int i);
	bool ReturnUpdate(int i);

	void ChangeState(EnemyState E, const int SetTimer = 2400);

	void Update0();
	void Update1();
	void Update2();
	void Update3();
	void Update4();
	void Update5();

	void Atk0();
	void Atk1();
	void Atk2();
	void Atk3();
	void Atk4();
	void Atk5();

	Math::Vector2 Move0();
	Math::Vector2 Move1();
public:
	Math::Vector2 GetPos() { return Pos; }
};
