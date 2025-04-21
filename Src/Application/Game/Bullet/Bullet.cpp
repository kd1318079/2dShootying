#include "Bullet.h"
#include "BulletTexLoad.h"
#include "../../Game/Game.h"
#include "../../main.h"
Bullet::Bullet(int A)
{
	Pos = PLAYER.GetPos();
	Deg = PLAYER.GetPDeg();
	TexNo = A;
	M.x = cos(ToRadians(PLAYER.GetPDeg() + 90));
	M.y = sin(ToRadians(PLAYER.GetPDeg() + 90));
	PlayerDeg = PLAYER.GetPDeg();
	MoveRad = M;
	BTex = BuTEX.GetTex(A);
	BulletType = A;
	if (BulletType == Commet)
	{
		M.x = cos(ToRadians(PLAYER.GetPDeg() + 90 + (rand() % 16 - 8)));
		M.y = sin(ToRadians(PLAYER.GetPDeg() + 90 + (rand() % 16 - 8)));
	}

	//�����l�̐ݒ�
	switch (BulletType)
	{
	case Straight:
		ATStraight(M);
		break;
	case fast:
		ATFast(M);
		break;
	case Missile:
		ATMissile(M);
		break;
	case Auto:
		ATAuto(M);
		break;
	case HighPower:
		ATHighPower(M);
		break;
	case Burst:
		ATBurst(M);
		break;
	case BurstP:
		ATBurstP(M);
		break;
	case Penetration:
		ATPenetration(M);
		break;
	case Charge:
		ATCharge(M);
		break;
	case Fire:
		ATFire(M);
		break;
	case Laesr:
		ATLaesr(M);
		break;
	case Gravity:
		ATGravity(M);
		break;
		break;
	case Homing:
		ATHoming(M);
		break;
	case RailGun:
		ATRailGun(M);
		break;
	case Chain:
		ATChain(M);
		break;
	case Virus:
		ATVirus(M);
		break;
	case Moon:
		ATMoon(M);
		break;
	case Commet:
		ATCommet(M);
		break;
	case Sun:
		ATSun(M);
		break;
	case Return:
		ATReturn(M);
		break;
	case Poizon:
		ATPoizon(M);
		break;
	case Conti:
		ATConti(M);
		break;
	case SDust:
		ATSDust(M);
		break;
	case SLight:
		ATSLight(M);
		break;
	default:
		break;
	}
}

void Bullet::Draw()
{
	rect = { 0,0,32,32 };
	if (TexNo == Laesr)
	{
		rect = { 0,0,64,1280 };
		D3D.SetBlendState(BlendMode::Add);
		SHADER.m_spriteShader.SetMatrix(Mat);
		SHADER.m_spriteShader.DrawTex(BTex, rect,PLAYER.LaesrCnt / 30.0);
	}
	else
	{
		D3D.SetBlendState(BlendMode::Alpha);
		SHADER.m_spriteShader.SetMatrix(Mat);
		SHADER.m_spriteShader.DrawTex(BTex, rect);
	}
	D3D.SetBlendState(BlendMode::Alpha);

}

void Bullet::Update()
{
	if (BulletType == Missile)
	{
		MissileCnt++;
		if (MissileCnt == 20)
		{
			MemDeg = ToDegrees(atan2(Pos.y - PLAYER.GetMousePos().y, Pos.x - PLAYER.GetMousePos().x)) + 90;
			MoveDeg = (MemDeg - Deg) / 20.0;
			Move = { 0,0 };
		}
		if(MissileCnt >= 20 &&MissileCnt <= 40)
		{
			Deg += MoveDeg;
			if (MissileCnt == 40)
			{
				Deg = MemDeg;
			}
		}
		if (MissileCnt == 40)
		{
			Math::Vector2 M;

			Move = { MissileSpd,MissileSpd };
			M.x = cos(ToRadians(Deg + 90));
			M.y = sin(ToRadians(Deg + 90));
			Move *= M;
		}
		if (MissileCnt >= 41)
		{
			Move *= 1.04;
		}
	}
	if (APP.count % 2 == 0)CELLGET(Pos, &BHitJ);
	if (BulletType == Burst || BulletType == Fire)DecreCh();
	if (BulletType == Homing && !HomingF)
	{
		for (auto A : GM.MobEnemy)
		{
			if (SCENE.CellHit(A->EHitJ, GetCell()))
			{
				if (SCENE.HitJudge(Pos, A->GetPos(), 64*8))
				{
					HomingF = true;
					HomingEnemy = A;
					HomingCnt = 180;
					break;
				}
			}
		}
	}
	else if (BulletType == Homing && HomingF && HomingCnt > 0)
	{
		if (HomingEnemy != nullptr) {
			float Spd = 8.0f; // �e�̑��x
			HomingCnt--;      // �z�[�~���O�񐔂�����

			// �^�[�Q�b�g�̊p�x���v�Z
			float targetAngle = ToDegrees(atan2(HomingEnemy->GetPos().y - Pos.y, HomingEnemy->GetPos().x - Pos.x));

			// �^�[�Q�b�g������ł��Ȃ��ꍇ�̏���
			if (!HomingEnemy->Exp) {
				PlayerDeg = targetAngle; // �^�[�Q�b�g�����Ɋp�x��ݒ�
				Deg = (PlayerDeg - 90); // �e�̈ړ��������v�Z
			}
			else
			{
				if (HomingEnemy->Exp) {
					// �^�[�Q�b�g�������ɂȂ����ꍇ�̏���
					HomingEnemy = nullptr;
					Move = { 8.0f, 8.0f }; // �f�t�H���g�̒��i����
					Move *= HomingDir;
					HomingF = false;
					Deg = (PlayerDeg); // �e�̈ړ��������v�Z
					return; // �������I��
				}
			}

			// �ړ��x�N�g���̌v�Z
			Math::Vector2 A;
			A.x = cos(ToRadians(PlayerDeg)); // �i�s������X����
			A.y = sin(ToRadians(PlayerDeg)); // �i�s������Y����

			// �ړ��ʂ�ݒ�
			Move = { Spd, Spd };
			Move *= A;           // �����ɑ��x��K�p

		}
		else {
			// �^�[�Q�b�g���Ȃ��ꍇ�̏���
			Move = { 8.0f, 0.0f }; // �f�t�H���g����i���i�j
		}
	}
	

	Pos += Move;	
	Main = Pos - PLAYER.GetScroll();
	//���[�U�[��������
	if (BulletType == Laesr)
	{
		Pos = PLAYER.GetPos();
		Main = PLAYER.GetMain();
		Math::Vector2 M;
		M.x = cos(ToRadians(PLAYER.GetPDeg() + 90));
		M.y = sin(ToRadians(PLAYER.GetPDeg() + 90));
		Pos += M * 640;
		Main += M * 640;
		Deg = PLAYER.GetPDeg();
	}
	MatSet();
}

void Bullet::ATStraight(Math::Vector2 A)
{
	float Spd;
	Spd = 8;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATFast(Math::Vector2 A)
{
	float Spd;
	Spd = 20;
	
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATMissile(Math::Vector2 A)
{
	Attack = 3;
	if (PLAYER.MissileCnt < 5)MissileLeft = true;
	PLAYER.MissileCnt++;
	if (PLAYER.MissileCnt >= 9)PLAYER.MissileCnt = 0;
	if (MissileLeft)
	{
		A.x = cos(ToRadians(PLAYER.GetPDeg() + 180));
		A.y = sin(ToRadians(PLAYER.GetPDeg() + 180));
		Deg += 90;
	}
	else
	{
		A.x = cos(ToRadians(PLAYER.GetPDeg()));
		A.y = sin(ToRadians(PLAYER.GetPDeg()));
		Deg -= 90;
	}
	Move = { MissileSpd,MissileSpd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
}

void Bullet::ATAuto(Math::Vector2 A)
{
	//�������g�p
	Move = { PLAYER.AutoX,PLAYER.AutoY };
	AutoDeg = PLAYER.AutoDeg;
}

void Bullet::ATHighPower(Math::Vector2 A)
{
	float Spd;
	Spd = 3;
	
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//�m�b�N�o�b�N
	PLAYER.BackMove = -Move;
	PLAYER.HighP = true;
	PLAYER.HighInit = false;
	PLAYER.HighPCnt = 30;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;

}

void Bullet::ATBurst(Math::Vector2 A)
{
	float Spd;
	Spd = 10;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	BurstCnt = 30;
}

void Bullet::ATBurstP(Math::Vector2 A)
{
	float Spd;
	Spd = 10;
	Pos = PLAYER.BurstPos;
	BurstDeg = PLAYER.BurstDeg;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATPenetration(Math::Vector2 A)
{
	float Spd;
	Spd = 10;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATCharge(Math::Vector2 A)
{
	float Spd;
	Spd = 5 + PLAYER.ChargeLv * 4;
	
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATFire(Math::Vector2 A)
{
	float Spd;
	Spd = 10;
	
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	FireCnt = 20;
}

void Bullet::ATLaesr(Math::Vector2 A)
{
	Attack = 2;
	Pos = PLAYER.GetPos();
	Deg = PLAYER.GetPDeg();
	BTex = BuTEX.GetLaserTex();
	PLAYER.LaesrCnt = 30;
}

void Bullet::ATGravity(Math::Vector2 A)
{
	float Spd;
	Spd = 4;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	GraCnt = 80;
}

void Bullet::ATHoming(Math::Vector2 A)
{
	float Spd;
	Spd = 6;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATRailGun(Math::Vector2 A)
{
	float Spd;
	Spd = 30;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATChain(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 10;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATVirus(Math::Vector2 A)
{
	Attack = 10;
	float Spd;
	Spd = 6;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	VirusF = true;
}

void Bullet::ATMoon(Math::Vector2 A)
{
	Attack = 5;
	float Spd;
	Spd = 10;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	MoonF = true;
}

void Bullet::ATCommet(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 10;
	Spd += (rand() % 10 - 5) / 100; 
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;

	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATSun(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 6;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	SunF = true;
}

void Bullet::ATReturn(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 6;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATPoizon(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 6;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
	PoizonF = true;
}

void Bullet::ATConti(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 6;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATSDust(Math::Vector2 A)
{
	Attack = 2;
	float Spd;
	Spd = 20;
	Move = { Spd,Spd };
	//�ړ��ʂɊp�x����Z
	Move *= A;
	HomingDir = A;
	//���@�̈ړ��Ɏ኱�e�����󂯂�悤��
	Move += PLAYER.GetMove() / 10;
}

void Bullet::ATSLight(Math::Vector2 A)
{
	
}

void Bullet::DecreCh()
{
	BurstCnt--;
	FireCnt--;
	if (BurstCnt <= 0 && BulletType == Burst)
	{
		BurstF = true;
	}
	if (FireCnt <= 0 && BulletType == Fire)
	{
		FireF = true;
	}

}

void Bullet::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Main.x, Main.y, 0);

	Rota = Math::Matrix::CreateRotationZ(ToRadians(Deg));
	if(BulletType == Auto) 
		Rota = Math::Matrix::CreateRotationZ(ToRadians(AutoDeg + 90));
	if (BulletType == BurstP)
		Rota = Math::Matrix::CreateRotationZ(ToRadians(BurstDeg -90));

	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Rota * Scale * Trans;
}
