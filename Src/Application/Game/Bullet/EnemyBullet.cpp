#include "EnemyBullet.h"
#include "BulletTexLoad.h"
#include "../../Game/Game.h"
#include "../../main.h"

EnemyBullet::EnemyBullet(Math::Vector2 P,int A, int Atk, float SPD)
{
	Pos = P;
	EBType = A;
	ATK = Atk;
	//�����X�s�[�h
	Move = { SPD,SPD };
	//���ˎ��̎��@�Ƃ̊p�x
	float Radi = atan2(PLAYER.GetPos().y - Pos.y, PLAYER.GetPos().x - Pos.x);
	//�x�N�g��
	Deg = ToDegrees(Radi);
	Math::Vector2 Vec = {cos(Radi), sin(Radi)};

	Move *= Vec;
	BTex = BuTEX.GetETex(0);
	MatSet();
}

EnemyBullet::EnemyBullet(Math::Vector2 P, int A, int Atk, float SPD, float deg)
{
	Pos = P;
	EBType = A;
	ATK = Atk;
	//�����X�s�[�h
	Move = { SPD,SPD };
	//���ˎ��̎��@�Ƃ̊p�x
	float Radi = ToRadians(deg);
	//�x�N�g��
	Deg = ToDegrees(Radi);
	Math::Vector2 Vec = { cos(Radi), sin(Radi) };

	Move *= Vec;
	BTex = BuTEX.GetETex(0);
	MatSet();
}

void EnemyBullet::Draw()
{
	rect = { 0,0,64,64 };
	
	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(Mat);
	SHADER.m_spriteShader.DrawTex(BTex, rect , 1);
	D3D.SetBlendState(BlendMode::Alpha);
}

void EnemyBullet::Update()
{
	//�Z���̒l�Q�b�g
	if (APP.count % 2 == 0)CELLGET(Pos, &EBHitJ);

	if (SCENE.CellHit(PLAYER.HitJ, EBHitJ))
	{
		if (SCENE.HitJudge(Pos, PLAYER.GetPos(), 64))
		{

			float Dmg =ATK - PLAYER.GetDEF();
			PLAYER.SetHP(PLAYER.GetHP() - Dmg);
			
			//������t���O�Ɣ����̃t���O
			Exp = true;
		}	
	}
	Pos += Move;
	MatSet();
}

void EnemyBullet::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Pos.x, Pos.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(Deg - 90));
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Rota * Scale * Trans;
}

