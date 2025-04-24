#include "Enemy.h"
#include "EnemyTexLoad.h"
#include "../../Game/Game.h"
#include "../../main.h"
#include "../../Scene.h"

Enemy::Enemy(int A)
{
	//A�͓G�̎�ނ��w��
	ATK = 1;
	DEF = 1;
	ETex = ENEMYTEX.GetTex(0);
	Pos = {(float)(rand() % 1280 - 640),(float)(rand() % 720 - 360)};
	Move = { 0,0 };
	PScale = { 1,1 };
	MatSet();
}

void Enemy::Update()
{
	if (APP.count % 2 == 0)CELLGET(Pos, &EHitJ);
	if (!Hit && !Exp)
	{
		if (APP.count % 2 == 0)ATHit(PLAYER.MainBullet);
		if (APP.count % 2 == 0)ATHit(PLAYER.SubBullet);
		if (PLAYER.SPBulletCnt != Laesr) ATHit(PLAYER.SPBullet);
		if (APP.count % 2 == 0)ATHit(PLAYER.BurstBullet);

		if (PLAYER.SPBulletCnt == Laesr)
			if (APP.count % 2 == 0)
			{
				for (auto A : PLAYER.SPBullet)
				{
					if (isColliding(A->GetPos(), {1280,64},PLAYER.GetPDeg(), Pos, 24))
					{
						Dmg = A->GetATK() - DEF;
						HP -= Dmg;
						Hit = true;
						
						Dmgnum.push_back(new NumDraw({ Pos.x ,Pos.y }, Dmg));
						DeathUpdate(this);

					}

				}
			}
	}
	if (ChainF)
	{
		std::cout << " " << std::endl;
		for (int i = 0; i < ChainCnt; i++)
		{
			if (ChainHit(ChainPosA[ChainPosA.size() - 1]));
			else ChainF = false;
		}
		if (ChainF)
		{
			//�����������Ă邽�߂܂�����
			//ChainEnemy.erase(ChainEnemy.begin());
			for (auto A : ChainEnemy)
			{
				std::cout << A->Pos.x << " " << A->Pos.y << std::endl;
				ChainCnt = 0;
				Dmg--;
				if (Dmg <= 0)Dmg = 1;
				A->HP -= Dmg;
				DeathUpdate(A);
				A->Hit = true;
				Dmgnum.push_back(new NumDraw({ Pos.x ,Pos.y }, Dmg));

			}
			ChainEnemy.clear();
			ChainF = false;
		}
	}

	if (Exp)Move = { 0,0 };
	Pos += Move;
	if (Exp)
	{
		if (ExpCnt <= 0)
		{
			ExpCnt = 60;
			PLAYER.ExpPos = Pos;
			PLAYER.BulletExp.push_back(new Explosion(ExpCnt, 2,1));
		}
		ExpCnt--;
		if (ExpCnt <= 0)
		{
			ExpCnt = 0;
			Death = true;
		}
	}

	//Move�ύX����
	if(!Exp)if (APP.count % 3 == 0) for (auto A : PLAYER.BulletExp)
	{
		if (SCENE.HitJudge(Pos, A->GetPos(), A->GetRectSize() * A->GetSize() / 1.5))
		{
			//�_���[�W
			if (A->EType == 2)
			{
				HP-= 2;
			}
			//�O���r�e�B
			if (A->EType == 4)
			{
				float Deg = ToDegrees(atan2(A->GetPos().y - Pos.y, A->GetPos().x - Pos.x)) - 90;
				Move = { 5,5 };
				Math::Vector2 Vec;
				Vec.x = cos(ToRadians(Deg + 90));
				Vec.y = sin(ToRadians(Deg + 90));
				Move *= Vec;
				
			}
			//HP��0�ȉ��ɂȂ�����
			DeathUpdate(this);
		}
	}
	for (auto A = Dmgnum.begin(); A != Dmgnum.end(); )
	{
		(*A)->Cnt--;
		if ((*A)->Cnt < 0)
		{
			A = Dmgnum.erase(A); // erase�͎��̗v�f���w���L���ȃC�e���[�^�[��Ԃ��܂�
		}
		else ++A;
	}

	if (Dmgnum.size() >= 10)
	{
		Dmgnum.erase(Dmgnum.begin()); // erase�͎��̗v�f���w���L���ȃC�e���[�^�[��Ԃ��܂�
	}

	Condisyon();

	MatSet();
}

void Enemy::Draw()
{
	rect = { 0,0,64,64 };
	SHADER.m_spriteShader.SetMatrix(Mat);
	if (!Exp)
	{
		if (!Hit)SHADER.m_spriteShader.DrawTex(ETex, rect);
	}

	for (auto A : Dmgnum)A->Draw();
	Hit = false;
}

void Enemy::ATHit(std::vector<Bullet*>& Bu)
{
    for (auto A = Bu.begin(); A != Bu.end(); )
    {
        if (SCENE.CellHit(EHitJ, (*A)->GetCell()))
        {
			if (SCENE.HitJudge(Pos, (*A)->GetPos(), Size))
			{
				//�_���[�W�v�Z
				float Dmg = (*A)->GetATK() - DEF;
				//�A���̎�
				if ((*A)->BulletType == Conti)
				{
					PLAYER.ContiCnt++;
					Dmg += PLAYER.ContiCnt / 20;
				}
				else PLAYER.ContiCnt = 0;

				//������
				if (Virus)Dmg = (*A)->GetATK() * 1.2 - DEF;
				//�������
				if ((*A)->MoonF) Dmg = (*A)->GetATK();
				
				//��Ԉُ큦�E�C���X
				if ((*A)->VirusF)
				{
					Virus = true;
					VirusCnt = 500;
				}
				//��Ԉُ큦��
				if ((*A)->PoizonF)
				{
					Poizon = true;
					PoizonCnt = 30;
				}

				//�Œ�l�ۏ�
				if (Dmg <= 0)Dmg = 1;
				HP -= Dmg;
				Dmgnum.push_back(new NumDraw({Pos.x ,Pos.y}, Dmg));

				//�G�l���M�[��
				if ((*A)->SunF)
				{
					PLAYER.SetEnergy(PLAYER.GetEnergy() + (int)Dmg);
				}


				Hit = true;
				if ((*A)->BulletType == Chain)
				{
					ChainF = true;
					ChainCnt = 5;
					ChainEnemy.clear();
					ChainPosA.clear();
					ChainPosB.clear();
					Dmg = (*A)->GetATK();
					ChainPosA.push_back(Pos);
					ChainEnemy.push_back(this);
				}
				PLAYER.ExpPos = (*A)->GetPos();
				// Bu����A���폜

				if((*A)->BulletType == Fire) PLAYER.BulletExp.push_back(new Explosion(72, 5, 2));
				else if ((*A)->BulletType == Gravity) PLAYER.BulletExp.push_back(new Explosion(128, 5, 4));
				else if((*A)->BulletType == SDust)PLAYER.BulletExp.push_back(new Explosion(60,10));
				else PLAYER.BulletExp.push_back(new Explosion(18, 1, 3));

				DeathUpdate(this);
				if ((*A)->BulletType == Penetration || (*A)->BulletType == RailGun);
				else 
				{
					// Bu����A���폜
					A = Bu.erase(A); // �C�e���[�^�����̗v�f�ɐi�߂�
					continue; // erase���A���C���N�������g���Ȃ�
				}
			}
        }
        ++A; // ���̗v�f�֐i��
    }
}

double Enemy::PointToLine(Math::Vector2 P, Math::Vector2 A, Math::Vector2 B)
{
	double dx = B.x - A.x;
	double dy = B.y - A.y;
	if (dx == 0 && dy == 0) return std::hypot(P.x - A.x, P.y - A.y);

	double t = ((P.x - A.x) * dx + (P.y - A.y) * dy) / (dx * dx + dy * dy);
	t = std::max(0.0, std::min(1.0, t));

	double nearestX = A.x + t * dx;
	double nearestY = A.y + t * dy;

	return std::hypot(P.x - nearestX, P.y - nearestY);
}

bool Enemy::isColliding(Math::Vector2 playerPos,Math::Vector2 rectSize,float rectDeg,Math::Vector2 circlePos,float circleRadius) {
	float theta = (rectDeg + 90) * M_PI / 180.0f;
	float cosT = cosf(theta);
	float sinT = sinf(theta);

	float halfW = rectSize.x / 2.0f;
	float halfH = rectSize.y / 2.0f;

	// �����Ă����
	Math::Vector2 forward = { cosT, sinT };

	// ���@�i���ˈʒu�j����forward������halfH���炵�āA���[�U�[�̒��S
	Math::Vector2 rectCenter = {
    playerPos.x - forward.x * (rectSize.x / 2.0f),
    playerPos.y - forward.y * (rectSize.y / 2.0f)
	};

	// �G�̈ʒu�����[�U�[�̃��[�J�����W�ɕϊ��i�t��]�j
	float dx = circlePos.x - playerPos.x;
	float dy = circlePos.y - playerPos.y;

	float localX = dx * cosT + dy * sinT;
	float localY = -dx * sinT + dy * cosT;

	// ���ρI
	/*Math::Vector2 _playerDir	= forward;
	_playerDir.Normalize();
	Math::Vector2 _toEnemyDir	= GetPos() - PLAYER.GetPos();
	_toEnemyDir.Normalize();
	float _dot = _toEnemyDir.Dot(_playerDir);*/
	//if(_dot > 0.82f)
	{
		// ���[�U�[�͈͓� & ���ɂ͓����蔻��Ȃ�
		if (localX >= -halfW + circleRadius && localX <= halfW - circleRadius &&
			localY >= -halfH - circleRadius && localY <= halfH + circleRadius
			) {
			return true;
		}
	}
	return false;
}

bool Enemy::ChainHit(Math::Vector2 ChP)
{
	Math::Vector2 Up;
	Math::Vector2 Down;
	const int ABC = 64 * 3;
	Up = { ChP.x + ABC,ChP.y + ABC };
	Down = { ChP.x - ABC ,ChP.y - ABC };
	auto Compere = [](Math::Vector2 A, Math::Vector2 B, Math::Vector2 C, float* Dis, int Cnt, Enemy* E,Math::Vector2 ChP,int* DCnt) -> bool
		{
			if (!(A.x >= C.x && A.x <= B.x && A.y >= C.y && A.y <= B.y))
			{
				return false; // �͈͏����𖞂����Ȃ��ꍇ�͑��߂Ƀ��^�[��
			}

			float dx = ChP.x - A.x;
			float dy = ChP.y - A.y;
			float c = sqrt(dx * dx + dy * dy);
			std::cout << "����" << c << " Cnt"<< Cnt << std::endl;
			if (c < *Dis && c != 0) // �ŏ��l���X�V�������
			{
				*Dis = c;
				*DCnt = Cnt;
				std::cout << "DisCnt: " << *DCnt << std::endl;
			}
			return true;
		};

	bool ATT = false;

	AutoDis = FLT_MAX;
	DisCnt = 0;

	int ECnt = 0;
	for (auto A : GM.MobEnemy)
	{
		if (!A) continue;
		//if (A == this)continue;
		if (Compere(A->GetPos(), Up, Down, &AutoDis, ECnt,this,ChP,&DisCnt))
		{
			ATT = true;
		}
		ECnt++;
	}
	/*auto Aa = GM.MobEnemy.begin() + DisCnt;
	CEnemy = *Aa;*/
	auto Aa = GM.MobEnemy.begin() + DisCnt;
	CEnemy = *Aa;

	if (ATT)
	{
		if (std::find(ChainEnemy.begin(), ChainEnemy.end(), CEnemy) == ChainEnemy.end())
		{
			ChainEnemy.push_back(CEnemy);
			ChainPosB.push_back(CEnemy->GetPos());
			CEnemy = nullptr;
			ATT = false;
		}
	}
	if (ChainPosB.empty())return false;
	
	auto it = ChainPosB.end() - 1;
	if (std::find(ChainPosA.begin(), ChainPosA.end(), *it) == ChainPosA.end())
		ChainPosA.push_back(*it);
	return true;
}

void Enemy::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Pos.x, Pos.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(Deg));
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Rota * Scale * Trans;
}

void Enemy::Condisyon()
{
	if (Virus)
	{
		VirusCnt--;
		if (VirusCnt < 0)Virus = false;
	}
	if (Poizon)
	{
		HP--;
		DeathUpdate(this);

		PoizonCnt--;
		if (PoizonCnt < 0)Poizon = false;
	}
}

void Enemy::DeathUpdate(Enemy* A)
{
	if (A->HP <= 0)
	{
		A->HP = 0;
		A->Exp = true;
	}
}
