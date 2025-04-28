#include "Enemy.h"
#include "EnemyTexLoad.h"
#include "../../Game/Game.h"
#include "../../main.h"
#include "../../Scene.h"

Enemy::Enemy(int A)
{
	//Aは敵の種類を指定
	TypeNum = A;
	ATK = 1;
	DEF = 1;
	ETex = ENEMYTEX.GetTex(0);
	Pos = PosSet();
	Move = { 0,0 };
	PScale = { 0.8,0.8 };
	EState = EnemyState::Move;
	SetStetas(A);
	MatSet();
}

Enemy::Enemy(int A, Math::Vector2 B)
{
	//Aは敵の種類を指定
	TypeNum = A;
	ATK = 1;
	DEF = 1;
	ETex = ENEMYTEX.GetTex(0);
	Pos = B;
	Move = { 0,0 };
	PScale = { 0.8,0.8 };
	EState = EnemyState::Move;
	SetStetas(A);
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
			//自分が入ってるためまず消す
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

	E_Update(TypeNum);
	

	//Move変更あり
	if(!Exp)if (APP.count % 4 == 0) for (auto A : PLAYER.BulletExp)
	{
		if (SCENE.HitJudge(Pos, A->GetPos(), A->GetRectSize() * A->GetSize() / 1.5))
		{
			//ダメージ
			if (A->EType == 2)
			{
				HP-= 3;
				Dmgnum.push_back(new NumDraw({ Pos.x ,Pos.y }, 3));
			}
			//グラビティ
			if (A->EType == 4)
			{
				float Deg = ToDegrees(atan2(A->GetPos().y - Pos.y, A->GetPos().x - Pos.x)) - 90;
				//Move = { 5,5 };
				Math::Vector2 Vec;
				Vec.x = cos(ToRadians(Deg + 90));
				Vec.y = sin(ToRadians(Deg + 90));
				Move += Vec * 6;
				//Move *= -1;
				GravityF = true;
			}
			
			//HPが0以下になった時
			DeathUpdate(this);
		}
	}
	for (auto A = Dmgnum.begin(); A != Dmgnum.end(); )
	{
		(*A)->Cnt--;
		if ((*A)->Cnt < 0)
		{
			A = Dmgnum.erase(A); // eraseは次の要素を指す有効なイテレーターを返します
		}
		else ++A;
	}

	if (Dmgnum.size() >= 10)
	{
		Dmgnum.erase(Dmgnum.begin()); // eraseは次の要素を指す有効なイテレーターを返します
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

	for (auto A : Dmgnum)A->Draw(Size * PScale.x);
	Hit = false;

	
}

void Enemy::ATHit(std::vector<Bullet*>& Bu)
{
    for (auto A = Bu.begin(); A != Bu.end(); )
    {
        if (SCENE.CellHit(EHitJ, (*A)->GetCell()))
        {
			if (SCENE.HitJudge(Pos, (*A)->GetPos(), Size * PScale.x))
			{
				//ダメージ計算
				float Dmg = (*A)->GetATK() - DEF;
				//連続の時
				if ((*A)->BulletType == Conti)
				{
					PLAYER.ContiCnt++;
					Dmg += PLAYER.ContiCnt / 20;
				}
				else PLAYER.ContiCnt = 0;

				//感染時
				if (Virus)Dmg = (*A)->GetATK() * 1.2 - DEF;
				//守備無視
				if ((*A)->MoonF) Dmg = (*A)->GetATK();
				
				//状態異常※ウイルス
				if ((*A)->VirusF)
				{
					Virus = true;
					VirusCnt = 500;
				}
				//状態異常※毒
				if ((*A)->PoizonF)
				{
					Poizon = true;
					PoizonCnt = 30;
				}

				//最低値保証
				if (Dmg <= 0)Dmg = 1;
				HP -= Dmg;
				Dmgnum.push_back(new NumDraw({Pos.x ,Pos.y}, Dmg));

				//エネルギー回復
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
				// BuからAを削除

				if((*A)->BulletType == Fire) PLAYER.BulletExp.push_back(new Explosion(72, 5, 2));
				else if ((*A)->BulletType == Gravity) PLAYER.BulletExp.push_back(new Explosion(128, 5, 4));
				else if((*A)->BulletType == SDust)PLAYER.BulletExp.push_back(new Explosion(60,10));
				else PLAYER.BulletExp.push_back(new Explosion(18, 1, 3));

				DeathUpdate(this);
				if ((*A)->BulletType == Penetration || (*A)->BulletType == RailGun);
				else 
				{
					// BuからAを削除
					A = Bu.erase(A); // イテレータを次の要素に進める
					continue; // erase後はAをインクリメントしない
				}
			}
        }
        ++A; // 次の要素へ進む
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

	// 向いてる方向
	Math::Vector2 forward = { cosT, sinT };

	// 自機（発射位置）からforward方向にhalfHずらして、レーザーの中心
	Math::Vector2 rectCenter = {
    playerPos.x - forward.x * (rectSize.x / 2.0f),
    playerPos.y - forward.y * (rectSize.y / 2.0f)
	};

	// 敵の位置をレーザーのローカル座標に変換（逆回転）
	float dx = circlePos.x - playerPos.x;
	float dy = circlePos.y - playerPos.y;

	float localX = dx * cosT + dy * sinT;
	float localY = -dx * sinT + dy * cosT;

	// 内積！
	/*Math::Vector2 _playerDir	= forward;
	_playerDir.Normalize();
	Math::Vector2 _toEnemyDir	= GetPos() - PLAYER.GetPos();
	_toEnemyDir.Normalize();
	float _dot = _toEnemyDir.Dot(_playerDir);*/
	//if(_dot > 0.82f)
	{
		// レーザー範囲内 & 後ろには当たり判定なし
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
				return false; // 範囲条件を満たさない場合は早めにリターン
			}

			float dx = ChP.x - A.x;
			float dy = ChP.y - A.y;
			float c = sqrt(dx * dx + dy * dy);
			std::cout << "距離" << c << " Cnt"<< Cnt << std::endl;
			if (c < *Dis && c != 0) // 最小値を更新する条件
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
	for (auto A : GM.AllEnemy)
	{
		if (!A) continue;
		//if (A == this)continue;
		if (Compere(A->GetPos(), Up, Down, &AutoDis, ECnt,this,ChP,&DisCnt))
		{
			ATT = true;
		}
		ECnt++;
	}
	/*auto Aa = GM.AllEnemy.begin() + DisCnt;
	CEnemy = *Aa;*/
	auto Aa = GM.AllEnemy.begin() + DisCnt;
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
	Rota = Math::Matrix::CreateRotationZ(ToRadians(Deg - 90));
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Rota * Scale * Trans;
}

Math::Vector2 Enemy::PosSet()
{
	Math::Vector2 A;
	float Y = sin(rand() % 360);

	if (rand() % 2) A = { 640 + Size / 2.0f,Y * 300 };
	else A = { -640 - Size / 2.0f,Y * 300 };

	//A = {(float)cos(rand() % 360) * 400,(float)cos(rand() % 360) * 200};
	return A;
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

void Enemy::E_Update(int i)
{
	Timer--;
	if (!Exp)
	{
		switch (EState)
		{
		case EnemyState::Move:	 if (MoveUpdate(TypeNum))		ChangeState(EnemyState::Attack);	break;		//初期値からの移動
		case EnemyState::Attack:
		{
			//ボスの時は移動か攻撃を続ける
			if (Boss)
			{
				if (AttackUpdate(TypeNum)) 
				{
					ChangeState(EnemyState::Move, 600);
					BossF = false;
				}
				break;
			}
			//ボス以外の時は画面外に
			if (AttackUpdate(TypeNum))	ChangeState(EnemyState::Return, 600);
			break;
		}
		case EnemyState::Return: if (ReturnUpdate(TypeNum))	ChangeState(EnemyState::Dead);			break;		//攻撃した後画面外に
		case EnemyState::Dead:	 if (Timer <= 0)Death = true;										break;		//画面外で消す処理
		}
	}
}

bool Enemy::MoveUpdate(int i)
{
	switch(i)
	{
	case 0:
		Update0();
		break;
	case 1:
		Update1();
		break;
	case 2:
		Update1();
		break;
	case 3:
		Update2();
		break;
	case 4:
		Update5();
		break;
	case 5:
		Update4();
		break;
	case 6:
		Update6();
		break;
	case 7:
		Update7();
		break;
	case 8:
		Update8();
		break;

	}

	if (Timer < 0) return true;
	else return false;
}

bool Enemy::AttackUpdate(int i)
{
	switch (i)
	{
	case 0:
		Atk0();
		break;
	case 1:
		Atk0();
		break;
	case 2:
		Atk1();
		break;
	case 3:
		Atk2();
		break;
	case 4:
		Atk3();
		break;
	case 5:
		Atk4();
		break;
	case 6:
		Atk6();
		break;
	case 7:
		Atk7();
		break;
	case 8:
		Atk8();
		break;
	}
	if (Timer < 0) return true;
	else return false;
}

bool Enemy::ReturnUpdate(int i)
{
	float Deg = atan2(Pos.y, Pos.x);
	Math::Vector2 A;
	A.x = cos(Deg);
	A.y = sin(Deg);
	Move = { 0.8,0.8 };
	Move *= A;

	if (Timer < 0) return true;
	else return false;
}

void Enemy::ChangeState(EnemyState E,const int SetTimer)
{
	EState = E;
	Timer = SetTimer;
}

void Enemy::Update0()
{
	Move = Move0();
}

void Enemy::Update1()
{
	Move = Move1(true);
}

void Enemy::Update2()
{
	Move = Move2(1.5);
}

void Enemy::Update3()
{
	Move = Move3();
}

void Enemy::Update4()
{
	if (Timer > 1975)
	{
		if (!ERe)
		{
			ERe = true;
			DegC = rand() % 360;
		}
		Deg = DegC;
		Move = { 1,1 };
		Math::Vector2 Vec = { cos(ToRadians(DegC)) , sin(ToRadians(DegC)) };
		Move *= Vec;
	}
	else
	{
		if (Timer % 180 == 0)
		{
			float PDeg = ToDegrees(atan2(PLAYER.GetPos().y - Pos.y, PLAYER.GetPos().x - Pos.x));
			Deg = ToRadians(PDeg);
			PLAYER.EBullet.push_back(new EnemyBullet(Pos, 0, 2, 3));
			Move = { 0,0 };
		}
		Timer+=2;
		if (Timer >= 750)Timer = 300;
	}
}

void Enemy::Update5()
{
	Move = Move3();
}

void Enemy::Update6()
{
	Move = Move6();
}

void Enemy::Update7()
{
}

void Enemy::Update8()
{
}

void Enemy::Atk0()
{
	//Move = Move0();
	Move = { 0,0 };
	if (Timer % 100 == 0)
	{
		PLAYER.EBullet.push_back(new EnemyBullet(Pos, 0, 2, 3));
	}
}

void Enemy::Atk1()
{
	if (BulletF1)
	{
		float Spd;
		if (Timer > 760) Spd = -0.8;
		else
		{
			BulletF1 = false;
			Spd = 12;
			Timer = 250;
		}
		Deg = atan2(PLAYER.GetPos().y - Pos.y,PLAYER.GetPos().x - Pos.x);
		Math::Vector2 Vec = { (float)cos(Deg),(float)sin(Deg) };
		Move = { Spd ,Spd };
		Move *= Vec;
	}
	else
	{
		Move *= 0.988;
	}

}

void Enemy::Atk2()
{
	//何もしない
}

void Enemy::Atk3()
{
	if (Timer % 1500 == 0)
	{
		float PDeg = ToDegrees(atan2(PLAYER.GetPos().y - Pos.y, PLAYER.GetPos().x - Pos.x));
		Deg = ToRadians(PDeg);

		PLAYER.EBullet.push_back(new EnemyBullet(Pos, 0, 2, 3));
		int A = rand() % 3 + 1;

		GM.Enemy5Cnt.push_back(A);
		GM.Enemy5PosCnt.push_back(Pos);

	}

	Timer += 2;
}

void Enemy::Atk4()
{

}

void Enemy::Atk5()
{

}

void Enemy::Atk6()
{
	Deg = ToDegrees(atan2(PLAYER.GetPos().y - Pos.y,PLAYER.GetPos().x - Pos.x));
	
	if (Timer % 1000 == 500)
	{
		PLAYER.EBullet.push_back(new EnemyBullet(Pos, 0, 2, 3, Deg));
		for (int i = 0; i < 15; i++)
		{
			float DegS = 0.7;
			float A = i * DegS;

			PLAYER.EBullet.push_back(new EnemyBullet(Pos, 0, 2, 3, Deg - A));
			PLAYER.EBullet.push_back(new EnemyBullet(Pos, 0, 2, 3, Deg + A));
		}
	}
	Deg = ToRadians(Deg);
}

void Enemy::Atk7()
{
}

void Enemy::Atk8()
{
}

Math::Vector2 Enemy::Move0()
{
	float Deg = ToDegrees(atan2(Pos.y, Pos.x));				//自分の角度
	float Rad = sqrt(Pos.x * Pos.x + Pos.y * Pos.y);	//半径
	float DegSp = rand() % 5 / 10 + 0.2;								//回転角度
	//移動先を少し内側に
	float RadS = 0.2;
	Rad -= RadS;

	float RadMax = 200;
	if (!GravityF)
	{
		if (Rad < RadMax)Rad = RadMax;
	}
	else
	{
		if (Rad >= RadMax)
		{
			Rad = RadMax;
		}
		else Rad += RadS * 1.4;
	}
	//移動先のPosを取得する
	//角度を足す
	Deg += DegSp;
	if (Deg >= 360)Deg-= 360;

	//移動先のPosへのベクトルの作成
	Math::Vector2 A;
	A.x = cos(ToRadians(Deg));
	A.y = sin(ToRadians(Deg));

	//半径とベクトルを合成して移動先のPosの取得
	Math::Vector2 Pos2 = A * Rad;

	// 既存の座標に円運動を加える
	return Pos2 - Pos;
}

Math::Vector2 Enemy::Move1(const bool A)
{
	if (!BulletF1)
	{
		BulletF1 = true;
		if (Pos.x > 0)		MovePlas.x = -4;
		else if (Pos.x < 0)	MovePlas.x = 4;
	}
	if (Pos.y > 300)		MovePlas.y = -5;
	else if (Pos.y < -300)	MovePlas.y = 5;
	
	float B = 0.9 + rand() % 10 / 100 / 2;
	if (Pos.x <= 580 && Pos.x >= -580) MovePlas *= B;
	if (fabs(MovePlas.x) <= 0.05)
	{
		if (Timer > 400)
		{
			if (!A)Timer = 120;
			else Timer = 20;
		}
		MovePlas = { 0,0 };
	}
	return MovePlas;
}

Math::Vector2 Enemy::Move2(const float Spd)
{
	float PDeg = ToDegrees(atan2(PLAYER.GetPos().y - Pos.y, PLAYER.GetPos().x - Pos.x));

	// 差分の計算
	float diff = PDeg - Deg;

	// 角度範囲補正 (-180～180)
	if (diff > 180) diff -= 360;
	if (diff < -180) diff += 360;

	// 徐々に角度を変化させる
	if (abs(diff) < DegSpeed / 2)
	{
		Deg - PDeg;
	}
	else
	{
		if (diff < 0)Deg -= DegSpeed;
		else Deg += DegSpeed;
	}
	// 範囲補正 (0～360度)
	if (Deg >= 360) Deg -= 360;
	if (Deg < 0) Deg += 360;

	Math::Vector2 A;
	A = { Spd,Spd };
	A *= {cos(ToRadians(Deg)), sin(ToRadians(Deg))};

	Timer += 2;
	if (Timer > 3000)Timer = 0;
	return A;
}

Math::Vector2 Enemy::Move3()
{
	if (!BulletF1)
	{
		BulletF1 = true;
		if (Pos.x > 0)		MovePlas.x = -4;
		else if (Pos.x < 0)	MovePlas.x = 4;
	}
	if (Pos.y > 300)		MovePlas.y = -5;
	else if (Pos.y < -300)	MovePlas.y = 5;

	float B = 0.9 + rand() % 10 / 100 / 2;
	if (Pos.x <= 520 && Pos.x >= -520) MovePlas *= B;
	if (fabs(MovePlas.x) <= 0.05)
	{
		if (Timer > 400)
		{
			Timer = 120;
		}
		MovePlas = { 0,0 };
	}
	return MovePlas;
}

Math::Vector2 Enemy::Move6()
{
	Math::Vector2 A;
	Deg = atan2(-Pos.y, -Pos.x);
	Math::Vector2 Vec = { (float)cos(Deg),(float)sin(Deg) };
	A = Vec * 2;

	Deg = ToDegrees(Deg);
	if (!BossF) if (fabs(Pos.x) <= 1 && fabs(Pos.y) <= 1)
	{
		BossF = true;
		Timer = 10;
	}
	if (BossF)A = { 0,0 };
	return A;
}

Math::Vector2 Enemy::Move7()
{
	return Math::Vector2();
}

Math::Vector2 Enemy::Move8()
{
	return Math::Vector2();
}

void Enemy::SetStetas(int A)
{
	switch (A)
	{
	case 0:
		HP = 50;
		ATK = 2;
		DEF = 3;
		break;
	case 1:
		HP = 30;
		ATK = 2;
		DEF = 3;
		break;
	case 2:
		HP = 10;
		ATK = 2;
		DEF = 3;
		break;
	case 3:
		HP = 20;
		ATK = 2;
		DEF = 3;
		break;
	case 4:
		HP = 100;
		ATK = 2;
		DEF = 3;
		break;
	case 5:
		HP = 5;
		ATK = 2;
		DEF = 2;
		break;
	case 6:
		HP = 2000;
		ATK = 2;
		DEF = 2;
		Boss = true;
		break;
	case 7:
		HP = 2000;
		ATK = 2;
		DEF = 2;
		Boss = true;
		break;
	case 8:
		HP = 2000;
		ATK = 2;
		DEF = 2;
		Boss = true;
		break;
	}
}

