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
	//�n�_�E�I�_
	std::vector<Math::Vector2> ChainPosA;
	std::vector<Math::Vector2> ChainPosB;
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
	//��Ԉُ�Update
	void Condisyon();

	//���S���@��{��Exp��true�ɂ���{�X������O����
	void DeathUpdate(Enemy* A);
public:
	Math::Vector2 GetPos() { return Pos; }
};
