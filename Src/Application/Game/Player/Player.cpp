#include "Player.h"
#include "../../main.h"
#include "PlayerBlockTex.h"
#include "../Game.h"
#include <memory>

void C_Player::ReturnInit()
{
	Alive = true;
	MaxHP = 100;
	HP = MaxHP;
	DEF = 3;

	PlayerSpd = 1;
	DegSpeed = 0.5;

	Wave = 0;
	Level = 1;

	Warp = false;
	BuWarp = false;

	MainBu.clear();
	SubBu.clear();
	
	MainBu.push_back({ Straight,0});

	Drone = 0;
	SPBu = false;

	//攻撃
	BulletAT[Straight] = 10;
	BulletAT[fast] = 10;
	BulletAT[Missile] = 5;
	BulletAT[Auto] = 5;
	BulletAT[HighPower] = 100;
	BulletAT[Burst] = 10;
	BulletAT[BurstP] = 3;
	BulletAT[Penetration] = 3;
	BulletAT[Charge] = 10;
	BulletAT[Fire] = 10;
	BulletAT[Laesr] = 20;
	BulletAT[Gravity] = 20;
	BulletAT[Homing] = 10;
	BulletAT[RailGun] = 10;
	BulletAT[Chain] = 10;
	BulletAT[Virus] = 3;
	BulletAT[Moon] = 10;
	BulletAT[Commet] = 3;
	BulletAT[Sun] = 10;
	BulletAT[Poizon] = 3;
	BulletAT[Conti] = 1;
	BulletAT[SDust] = 30;

	for (int i = 0; i < BulletMax; i++)
	{
		//強化レベルの初期化
		BulletPower[i] = 0;
		//特殊強化のフラグ初期化
		BulletSP[i] = false;
	}


}

void C_Player::Init()
{
	PlayerTex = BlTEX.GetTex(0);
	DegTex = BlTEX.GetTex(1);
	AimeTex.Load("Texture/others/Lockon.png");
	BackTex.Load("Texture/others/BackG.png");
	LevelUpTex.Load("Texture/others/LevelUp.png");
	//LVUP経験値
	EXP_Lv[0] = 300;
	for (int i = 1; i < MaxLv; i++)
	{
		EXPSum += EXP_Lv[i - 1];
		EXP_Lv[i] = EXP_Lv[i - 1] * (float)(2+((i - 2) / (float)(MaxLv - 2)));
	}
}

void C_Player::Update()
{
	//レベルアップ
	LvUp();

	EnemyB();
	//マウス座標ゲット
	MouseGet();
	//自機の移動
	MovePlayer();
	//プレイヤーのdegを決定
	PlayerDegSet();
	//プレイヤーの攻撃(BuleetCntで弾変更)
	PlayerAttack();
	//プレイヤーのセル情報取得
	if (APP.count % 2 == 0)CellGet(Pos,&HitJ);
	//爆発のUpdate
	ExplosionUpdate();
	//自機とマウス座標の決定
	SetMat();

}

void C_Player::Draw()
{
	//弾描画
	for (auto A : EBullet) A->Draw();

	for (auto Main : MainBullet) Main->Draw();
	for (auto Sub : SubBullet) Sub->Draw();
	for (auto SP : SPBullet) SP->Draw();
	for (auto BU : BurstBullet) BU->Draw();

	D3D.SetBlendState(BlendMode::Add);
	rect = { 0,0,640, 640 };
	SHADER.m_spriteShader.SetMatrix(DegMat);
	SHADER.m_spriteShader.DrawTex(DegTex, rect, 0.3, { 0.5,0.2 });
	
	D3D.SetBlendState(BlendMode::Alpha);
	rect = { 0,0,640, 640};
	SHADER.m_spriteShader.SetMatrix(Mat);
	SHADER.m_spriteShader.DrawTex(PlayerTex, rect, 1, {0.5,0.25});


	LevelCnt--;

	
	if (LevelCnt > 0)
	{
		rect = { 0,0,848, 147 };
		float size = 0.2;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(size) * Math::Matrix::CreateTranslation({ Pos.x,Pos.y + 15,0 }));
		SHADER.m_spriteShader.DrawTex(&LevelUpTex, rect,LevelCnt / 60.0f * 2);
	}

	rect = { 0,0,64, 64 };
	SHADER.m_spriteShader.SetMatrix(AimMat);
	SHADER.m_spriteShader.DrawTex(&AimeTex, rect);
	Math::Matrix A = Math::Matrix::CreateTranslation(0,0,0);


	for (auto Exp : BulletExp) if(Exp->EType != 4)Exp->Draw();
	
}

void C_Player::PreDraw()
{
	rect = { 0,0,1280,720 };
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1) * Math::Matrix::CreateTranslation(0 , 0 , 0));
	SHADER.m_spriteShader.DrawTex(&BackTex, rect);
	//グラビティ
	for (auto Exp : BulletExp) if (Exp->EType == 4)Exp->Draw();
}

void C_Player::Release()
{
	AimeTex.Release();
	BackTex.Release();
	MainBullet.clear();
	SubBullet.clear();
	SPBullet.clear();
	EBullet.clear();
	BurstBullet.clear();

	BulletExp.clear();
	MainBu.clear();
	SubBu.clear();

}

void C_Player::EnemyB()
{
	for (auto A = EBullet.begin(); A != EBullet.end(); ) {
		{
			float MaxX = 700;
			float MaxY = 400;
			if ((*A)->Exp)
			{
				PLAYER.ExpPos = (*A)->GetPos();
				A = EBullet.erase(A);
				PLAYER.BulletExp.push_back(new Explosion(30, 2, 0));
			}
			else if ((*A)->GetPos().x > MaxX || (*A)->GetPos().x < -MaxX || (*A)->GetPos().y > MaxY || (*A)->GetPos().x < -MaxX)
			{
				A = EBullet.erase(A);
			}
			else
			{
				(*A)->Update();
				++A; // 次の要素へ進む
			}

		}
	}
}

bool C_Player::DisPlayOutH(Math::Vector2 A)
{
	if (A.y > 360 || A.y < -360)
	{
		return true;
	}
	return false;
}

bool C_Player::DisPlayOutW(Math::Vector2 A)
{
	if (A.x > 640 || A.x < -640)
	{
		return true;
	}
	return false;
}

void C_Player::LevelUpdate(int i)
{
	switch (i)
	{
	case 0:
		break;
	case 1:
		MaxHP += 100;
		break;
	case 2:
		DEF += 2;
		break;
	case 3:
		DegSpeed += 0.25;
		break;
	case 4:
		PlayerSpd += 0.25;
		break;
	case 5:
		MainBu.push_back({ Straight, 0 });
		break;
	case 6:
		SubBu.push_back({ Straight, 0 });
		break;
	case 7:
		SPBu = true;
		break;
	case 8:
		GetBullet.push_back(fast);
		OthersBullet.push_back(fast);
		break;
	case 9:
		GetBullet.push_back(Missile);
		OthersBullet.push_back(Missile);
		break;
	case 10:
		GetBullet.push_back(Auto);
		OthersBullet.push_back(Auto);
		break;
	case 11:
		GetBullet.push_back(Burst);
		OthersBullet.push_back(Burst);
		break;
	case 12:
		GetBullet.push_back(Penetration);
		OthersBullet.push_back(Penetration);
		break;
	case 13:
		GetBullet.push_back(Fire);
		OthersBullet.push_back(Fire);
		break;
	case 14:
		GetBullet.push_back(Homing);
		OthersBullet.push_back(Homing);
		break;
	case 15:
		GetBullet.push_back(Chain);
		OthersBullet.push_back(Chain);
		break;
	case 16:
		GetBullet.push_back(Virus);
		OthersBullet.push_back(Virus);
		break;
	case 17:
		GetBullet.push_back(Moon);
		OthersBullet.push_back(Moon);
		break;
	case 18:
		GetBullet.push_back(Commet);
		OthersBullet.push_back(Commet);
		break;
	case 19:
		GetBullet.push_back(Sun);
		OthersBullet.push_back(Sun);
		break;
	case 20:
		GetBullet.push_back(Poizon);
		OthersBullet.push_back(Poizon);
		break;
	case 21:
		GetBullet.push_back(Conti);
		OthersBullet.push_back(Conti);
		break;
	case 22:
		GetBullet.push_back(HighPower);
		OthersBullet.push_back(HighPower);
		break;
	case 23:
		GetBullet.push_back(Charge);
		OthersBullet.push_back(Charge);
		break;
	case 24:
		GetBullet.push_back(Laesr);
		OthersBullet.push_back(Laesr);
		break;
	case 25:
		GetBullet.push_back(Gravity);
		OthersBullet.push_back(Gravity);
		break;
	case 26:
		GetBullet.push_back(RailGun);
		OthersBullet.push_back(RailGun);
		break;
	case 27:
		BulletPower[Straight]++;
		break;
	case 28:
		BulletPower[fast]++;
		break;
	case 29:
		BulletPower[Missile]++;
		break;
	case 30:
		BulletPower[Auto]++;
		break;
	case 31:
		BulletPower[Burst]++;
		break;
	case 32:
		BulletPower[Penetration]++;
		break;
	case 33:
		BulletPower[Fire]++;
		break;
	case 34:
		BulletPower[Homing]++;
		break;
	case 35:
		BulletPower[Chain]++;
		break;
	case 36:
		BulletPower[Virus]++;
		break;
	case 37:
		BulletPower[Moon]++;
		break;
	case 38:
		BulletPower[Commet]++;
		break;
	case 39:
		BulletPower[Sun]++;
		break;
	case 40:
		BulletPower[Poizon]++;
		break;
	case 41:
		BulletPower[Conti]++;
		break;
	case 42:
		BulletPower[HighPower]++;
		break;
	case 43:
		BulletPower[Charge]++;
		break;
	case 44:
		BulletPower[Laesr]++;
		break;
	case 45:
		BulletPower[Gravity]++;
		break;
	case 46:
		BulletPower[RailGun]++;
		break;
	case 47:
		Warp = true;
		break;
	case 48:
		BuWarp = true;
		break;
	case 49:
		BulletSP[Straight] = true;
		break;
	case 50:
		BulletSP[fast] = true;
		break;
	case 51:
		BulletSP[Missile] = true;
		break;
	case 52:
		BulletSP[Auto] = true;
		break;
	case 53:
		BulletSP[Burst] = true;
		break;
	case 54:
		BulletSP[Penetration] = true;
		break;
	case 55:
		BulletSP[Fire] = true;
		break;
	case 56:
		BulletSP[Homing] = true;
		break;
	case 57:
		BulletSP[Chain] = true;
		break;
	case 58:
		BulletSP[Virus] = true;
		break;
	case 59:
		BulletSP[Moon] = true;
		break;
	case 60:
		BulletSP[Commet] = true;
		break;
	case 61:
		BulletSP[Sun] = true;
		break;
	case 62:
		BulletSP[Poizon] = true;
		break;
	case 63:
		BulletSP[Conti] = true;
		break;
	case 64:
		BulletSP[HighPower] = true;
		break;
	case 65:
		BulletSP[Charge] = true;
		break;
	case 66:
		BulletSP[Laesr] = true;
		break;
	case 67:
		BulletSP[Gravity] = true;
		break;
	case 68:
		BulletSP[RailGun] = true;
		break;
	case 69:
		if (Drone < 3)	Drone++;
		break;

	}
	
	//処理の最後にHPを満タンにする処理を入れる
	HP = MaxHP;
}

bool C_Player::PowerUP(int A)
{
	if (A == 7 && SPBu)return false;
	else if (A >= 8 && A <= 26)
	{
		for (auto i : GetBullet)
		{
			if (A == 8 && i == fast)				return false;
			else if (A == 9 && i == Missile)		return false;
			else if (A == 10 && i == Auto)			return false;
			else if (A == 11 && i == Burst)			return false;
			else if (A == 12 && i == Penetration)	return false;
			else if (A == 13 && i == Fire)			return false;
			else if (A == 14 && i == Homing)		return false;
			else if (A == 15 && i == Chain)			return false;
			else if (A == 16 && i == Virus)			return false;
			else if (A == 17 && i == Moon)			return false;
			else if (A == 18 && i == Commet)		return false;
			else if (A == 19 && i == Sun)			return false;
			else if (A == 20 && i == Poizon)		return false;
			else if (A == 21 && i == Conti)			return false;
			else if (A == 22 && i == HighPower)		return false;
			else if (A == 23 && i == Charge)		return false;
			else if (A == 24 && i == Laesr)			return false;
			else if (A == 25 && i == Gravity)		return false;
			else if (A == 26 && i == RailGun)		return false;
		}
	}
	else if (A >= 27 && A <= 46)
	{
		for (auto i : GetBullet)
		{
			if (A == 27)							return true;
			else if (A == 28 && i == fast)			return true;
			else if (A == 29 && i == Missile)		return true;
			else if (A == 30 && i == Auto)			return true;
			else if (A == 31 && i == Burst)			return true;
			else if (A == 32 && i == Penetration)	return true;
			else if (A == 33 && i == Fire)			return true;
			else if (A == 34 && i == Homing)		return true;
			else if (A == 35 && i == Chain)			return true;
			else if (A == 36 && i == Virus)			return true;
			else if (A == 37 && i == Moon)			return true;
			else if (A == 38 && i == Commet)		return true;
			else if (A == 39 && i == Sun)			return true;
			else if (A == 40 && i == Poizon)		return true;
			else if (A == 41 && i == Conti)			return true;
			else if (A == 42 && i == HighPower)		return true;
			else if (A == 43 && i == Charge)		return true;
			else if (A == 44 && i == Laesr)			return true;
			else if (A == 45 && i == Gravity)		return true;
			else if (A == 46 && i == RailGun)		return true;
		}
		return false;
	}
	else if (A == 47 && Warp)return false;
	else if (A == 48 && BuWarp)return false;
	else if (A >= 49 && A <= 68)
	{
		for (auto i : GetBullet)
		{
			if (A == 49 && !BulletSP[Straight])								return true;
			else if (A == 50 && i == fast		 &&	!BulletSP[fast])		return true;
			else if (A == 51 && i == Missile	 &&	!BulletSP[Missile])		return true;
			else if (A == 52 && i == Auto		 &&	!BulletSP[Auto])		return true;
			else if (A == 53 && i == Burst		 &&	!BulletSP[Burst])		return true;
			else if (A == 54 && i == Penetration && !BulletSP[Penetration])	return true;
			else if (A == 55 && i == Fire		 &&	!BulletSP[Fire])		return true;
			else if (A == 56 && i == Homing		 &&	!BulletSP[Homing])		return true;
			else if (A == 57 && i == Chain		 &&	!BulletSP[Chain])		return true;
			else if (A == 58 && i == Virus		 &&	!BulletSP[Virus])		return true;
			else if (A == 59 && i == Moon		 &&	!BulletSP[Moon])		return true;
			else if (A == 60 && i == Commet		 &&	!BulletSP[Commet])		return true;
			else if (A == 61 && i == Sun		 &&	!BulletSP[Sun])			return true;
			else if (A == 62 && i == Poizon		 &&	!BulletSP[Poizon])		return true;
			else if (A == 63 && i == Conti		 &&	!BulletSP[Conti])		return true;
			else if (A == 64 && i == HighPower	 &&	!BulletSP[HighPower])	return true;
			else if (A == 65 && i == Charge		 &&	!BulletSP[Charge])		return true;
			else if (A == 66 && i == Laesr		 &&	!BulletSP[Laesr])		return true;
			else if (A == 67 && i == Gravity	 &&	!BulletSP[Gravity])		return true;
			else if (A == 68 && i == RailGun	 &&	!BulletSP[RailGun])		return true;
		}
		return false;
	}
	else if (A == 69 && Drone >= 3) return false;
	return true;
}

void C_Player::MouseGet()
{
	//マウス座標取得
	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	//mousePos.x *= -1;
	mousePos.y *= -1;
}

void C_Player::MovePlayer()
{
	//Aimeを回す処理
	AimeDeg += 3;
	if (AimeDeg >= 360)AimeDeg = 0;
	//機体サイズでスピードを落とす
	float Su = 3;

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (!SHIFT1)
		{
			SHIFT = true;
			SHIFT1 = true;
		}
		if (SHIFT1)
		{
			Count++;
			
			if (Count > 10)
			{

				//SHIFT = false;
				Su = 3;
			}
		}
	}
	else
	{
		SHIFT = false;
		SHIFT1 = false;
		if (Count > 10)
		{
			Count = 0;
		}
		Count--;
		if (Count < 0)Count = 0;
	}

	if (!HighP)
	{
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
		else
		{
			if (GetAsyncKeyState('A') & 0x8000)
			{
				Move.x -= PlayerSpd;
				//if (SHIFT)Move.x -= PlayerSpd * Su;
			}
			if (GetAsyncKeyState('D') & 0x8000)
			{
				Move.x += PlayerSpd;
				//if (SHIFT)Move.x += PlayerSpd * Su;
			}
			if (GetAsyncKeyState('W') & 0x8000)
			{
				Move.y += PlayerSpd;
				//if (SHIFT)Move.y += PlayerSpd * Su;
			}
			if (GetAsyncKeyState('S') & 0x8000)
			{
				Move.y -= PlayerSpd;
				//if (SHIFT)Move.y -= PlayerSpd * Su;
			}
		}
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			Math::Vector2 A = { cos(ToRadians(PlayerDeg + 90)) , sin(ToRadians(PlayerDeg + 90)) };
			Move += {(A.x / 10.0f * Su), (A.y / 10.0f * Su)};
		}
		if (SHIFT)
		{
			if (Move.x > PlayerSpd * Su)		Move.x = PlayerSpd * Su;
			else if (Move.x < -PlayerSpd * Su)	Move.x = -PlayerSpd * Su;
			if (Move.y > PlayerSpd * Su)			Move.y = PlayerSpd * Su;
			else if (Move.y < -PlayerSpd * Su)	Move.y = -PlayerSpd * Su;
		}
		else
		{
			if (Move.x > PlayerSpd)				Move.x = PlayerSpd;
			else if (Move.x < -PlayerSpd)		Move.x = -PlayerSpd;
			if (Move.y > PlayerSpd)				Move.y = PlayerSpd;
			else if (Move.y < -PlayerSpd)		Move.y = -PlayerSpd;
		}
	}
	else
	{
		if (HighP)
		{
			if (!HighInit)
			{
				HighInit = false;
				Move = BackMove;
				Move *= BackSpd;
			}
			HighPCnt--;
			if (HighPCnt <= 0)
			{
				HighP = false;
				HighInit = true;
			}
		}
	}

	

	Pos += Move;
	if (HighP)Move *= 0.95;
	else if (!SHIFT)Move *= 0.985;
	else Move *= 0.925;

}

void C_Player::PlayerDegSet() {
	float Deg = ToDegrees(atan2(Pos.y - mousePos.y, Pos.x - mousePos.x)) + 90;

	// 差分の計算
	float diff = Deg - PlayerDeg;

	// 角度範囲補正 (-180～180)
	if (diff > 180) diff -= 360;
	if (diff < -180) diff += 360;

	// 徐々に角度を変化させる
	if (abs(diff) < DegSpeed / 2)
	{
		PlayerDeg = Deg;
	}
	else
	{
		if (diff < 0)PlayerDeg -= DegSpeed;
		else PlayerDeg += DegSpeed;
	}
	// 範囲補正 (0～360度)
	if (PlayerDeg >= 360) PlayerDeg -= 360;
	if (PlayerDeg < 0) PlayerDeg += 360;

}

void C_Player::PlayerAttack()
{
	//完全自動攻撃のフラグ管理
	//if(BulletCnt == Auto)
	AutoAT();
	//弾のクールダウン
	for (auto& A : MainBu)	A.Time--;
	for (auto& A : SubBu)	A.Time--;
	SPBulletTime--;
	//多くなりすぎた弾を前から順に削除
	ATerase();

	//自動攻撃
	for(auto& A : MainBu)if(A.Time <= 0)
		A.Time = Attack(MainBullet, A.Cnt, A.Time);

	//サブ攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		for(auto& A :SubBu)if (A.Time <= 0)A.Time = Attack(SubBullet, A.Cnt,A.Time);
	}
	//特殊
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && SPBu)
	{
		if(SPBulletCnt == Laesr) LaesrCnt = LeasrMax;

		if (SPBulletCnt == Charge)
		{
			ChargeCnt++;
			if (ChargeCnt >= 20)
			{
				ChargeCnt = 0;
				ChargeLv++;
				if (ChargeLv > 10)
				{
					ChargeLv = 10;
				}
			}
		}
		else if (!SPACE)
		{
			SPACE = true;
			if (SPBulletCnt == Laesr)
			{
				if (SPBullet.size() == 0) SPBulletTime = Attack(SPBullet, SPBulletCnt, SPBulletTime);
			}
			else SPBulletTime = Attack(SPBullet, SPBulletCnt, SPBulletTime);
		}
	}
	else
	{
		if (SPBulletCnt == Charge && SPBulletTime <= 0 && (ChargeLv > 0 || ChargeCnt > 0))
		{
			SPBulletTime = Attack(SPBullet, SPBulletCnt, SPBulletTime);
			ChargeLv = 0;
			ChargeCnt = 0;
		}
		else if (SPBulletCnt == Laesr)
		{
			LaesrF = true;
			SPACE = false;
		}
		else if (SPBulletTime <= 0)SPACE = false;
	}

	FireExpF();
	//BurstDegF();
	//GraviFCnt();
	//SterD();

	if(LaesrF)LaesrFCnt();

	for (auto Main : MainBullet)	Main->Update();
	for (auto Sub : SubBullet)		Sub->Update();
	for (auto SP : SPBullet)		SP->Update();
	for (auto BU : BurstBullet)		BU->Update();
}

void C_Player::ExplosionUpdate()
{
	for (auto A : BulletExp)
	{
		A->Update();
	}
}

void C_Player::SetMat()
{
	Trans = Math::Matrix::CreateTranslation(Pos.x, Pos.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(PlayerDeg));
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);
	Mat = Scale * Rota * Trans;

	Trans = Math::Matrix::CreateTranslation(Pos.x, Pos.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(PlayerDeg));
	Scale = Math::Matrix::CreateScale(DegScale.x, DegScale.y, 0);
	DegMat = Scale * Rota * Trans;

	Trans = Math::Matrix::CreateTranslation(mousePos.x, mousePos.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(AimeDeg));
	Scale = Math::Matrix::CreateScale(AimeScale.x, AimeScale.y, 0);
	AimMat = Scale * Rota * Trans;
}

void C_Player::LvUp()
{
	Score += 1000000;
	int A = Level;
	if (Score > EXP_Lv[A + 1])
	{

		Level++;
		int A = rand() % 6;
		if (A == HighPower)A++;

		while (true)
		{
			//0～69
			LevelCnt1 = rand() % 69 + 1;
			if (PowerUP(LevelCnt1))
			{
				while (true)
				{
					//0～69
					LevelCnt2 = rand() % 69 + 1;
					if (PowerUP(LevelCnt2))
					{
						if (LevelCnt1 == LevelCnt2)continue;
						while (true)
						{
							//0～69
							LevelCnt3 = rand() % 69 + 1;
							if (PowerUP(LevelCnt3))
							{
								if (LevelCnt1 == LevelCnt3)continue;
								if (LevelCnt2 == LevelCnt3)continue;
								break;
							}
							else continue;
						}
						break;
					}
					else continue;
				}
				break;
			}
			else continue;
		}
		LevelCnt1;
		LevelCnt2;
		LevelCnt3;

		GM.SetScene(LevelUp);
	}
}

void C_Player::FireExpF()
{
	auto FireFlg = [](std::vector<Bullet*>& Bu) -> void {
		for (auto A = Bu.begin(); A != Bu.end(); ) {
			if ((*A)->FireF) {
				PLAYER.ExpPos = (*A)->GetPos();
				// BulletTypeに応じてExplosionを追加
				if ((*A)->BulletType == Fire)
					PLAYER.BulletExp.push_back(new Explosion(72,5,2));
				else
					PLAYER.BulletExp.push_back(new Explosion(40,1,3));

				// 現在の要素を安全に削除し、イテレーターを更新
				A = Bu.erase(A); // eraseは次の要素を指す有効なイテレーターを返します
			}
			else if ((*A)->BurstF)
			{
				PLAYER.BurstPos = (*A)->GetPos();
				// BuからAを削除
				A = Bu.erase(A); // イテレータを次の要素に進める炸裂
				for (int i = 0; i < PLAYER.BurstMax; i++)
				{
					PLAYER.BurstDeg += 360 / PLAYER.BurstMax;
					PLAYER.BurstBullet.push_back(new Bullet(BurstP));
				}
				continue; // erase後はAをインクリメントしない
			} 
			else if ((*A)->GraF)
			{
				(*A)->GraCnt = 0;
				PLAYER.ExpPos = (*A)->GetPos();
				A = PLAYER.SPBullet.erase(A);
				PLAYER.BulletExp.push_back(new Explosion(128, 5, 4));
				continue;
			}
			else if ((*A)->SDF) {
				PLAYER.ExpPos = (*A)->GetPos();
				// BulletTypeに応じてExplosionを追加
				if ((*A)->BulletType == SDust)
					PLAYER.BulletExp.push_back(new Explosion(120, 10));
				else
					PLAYER.BulletExp.push_back(new Explosion(18, 1, 3));

				// 現在の要素を安全に削除し、イテレーターを更新
				A = Bu.erase(A); // eraseは次の要素を指す有効なイテレーターを返します
			}
			else 
			{
				++A; // 次の要素へ進む
			}
		}
		};
	FireFlg(MainBullet);
	FireFlg(SubBullet);
	FireFlg(SPBullet);
}

void C_Player::LaesrFCnt()
{
	LaesrCnt--;
	if (LaesrCnt <= 0)
	{
		LaesrCnt = 0;
		LaesrF = true;
		SPBullet.clear();
	}
}

void C_Player::ATerase()
{
	if (MainBullet.size() >= 1000)
	{
		while (MainBullet.size() >= 1000)
		{
			MainBullet.erase(MainBullet.begin());
		}
	}
	if (SubBullet.size() >= 1000)
	{
		while (SubBullet.size() >= 1000)
		{
			SubBullet.erase(SubBullet.begin());
		}
	}
	if (SPBullet.size() >= 150)
	{
		while (SPBullet.size() >= 20)
		{
			SPBullet.erase(SPBullet.begin());
		}
	}
	if (BurstBullet.size() >= 1000)
	{
		while (BurstBullet.size() >= 1000)
		{
			BurstBullet.erase(BurstBullet.begin());
		}
	}
	if (BulletExp.size() >= 400) {
		BulletExp.erase(BulletExp.begin(), BulletExp.begin() + (BulletExp.size() - 199));
	}
	for (auto A = BulletExp.begin(); A != BulletExp.end(); ) {
		if (*A == nullptr || (*A)->DeleteCnt <= 0) {
			A = BulletExp.erase(A); // 無効または削除条件を満たす要素を削除
		}
		else {
			++A; // 次の要素へ進む
		}
	}

	//画面外に行くと消える処理
	for (auto A = MainBullet.begin(); A != MainBullet.end(); )
	{
		if (BuWarp)
		{
			if ((*A)->GetPos().x > 640)
			{
				if (!(*A)->WarpW)
				{
					(*A)->SetPos({ (*A)->GetPos().x - 1280,(*A)->GetPos().y });
					(*A)->WarpW = true;
				}
				else
				{
					A = MainBullet.erase(A);
					continue;
				}
			}
			if ((*A)->GetPos().x < -640)
			{
				if (!(*A)->WarpW)
				{
					(*A)->SetPos({ (*A)->GetPos().x + 1280,(*A)->GetPos().y });
					(*A)->WarpW = true;
				}
				else
				{
					A = MainBullet.erase(A);
					continue;
				}
			}
			if ((*A)->GetPos().y > 360)
			{
				if (!(*A)->WarpH)
				{
					(*A)->SetPos({ (*A)->GetPos().x ,(*A)->GetPos().y - 720 });
					(*A)->WarpH = true;
				}
				else
				{
					A = MainBullet.erase(A);
					continue;
				}
			}
			if ((*A)->GetPos().y < -360)
			{
				if (!(*A)->WarpH)
				{
					(*A)->SetPos({ (*A)->GetPos().x ,(*A)->GetPos().y + 720 });
					(*A)->WarpH = true;
				}
				else
				{
					A = MainBullet.erase(A);
					continue;
				}
			}
		}
		else if (DisPlayOutH((*A)->GetPos()) || DisPlayOutW(GetPos()))
		{
			A = MainBullet.erase(A);
			continue;
		}
		++A;
	}
	for (auto A = SubBullet.begin(); A != SubBullet.end(); )
	{
		if (BuWarp)
		{
			{
				if ((*A)->GetPos().x > 640)
				{
					if (!(*A)->WarpW)
					{
						(*A)->SetPos({ (*A)->GetPos().x - 1280,(*A)->GetPos().y });
						(*A)->WarpW = true;
					}
					else
					{
						A = SubBullet.erase(A);
						continue;
					}
				}
				if ((*A)->GetPos().x < -640)
				{
					if (!(*A)->WarpW)
					{
						(*A)->SetPos({ (*A)->GetPos().x + 1280,(*A)->GetPos().y });
						(*A)->WarpW = true;
					}
					else
					{
						A = SubBullet.erase(A);
						continue;
					}
				}
			}
			{
				if ((*A)->GetPos().y > 360)
				{
					if (!(*A)->WarpH)
					{
						(*A)->SetPos({ (*A)->GetPos().x ,(*A)->GetPos().y - 720 });
						(*A)->WarpH = true;
					}
					else
					{
						A = SubBullet.erase(A);
						continue;
					}
				}
				if ((*A)->GetPos().y < -360)
				{
					if (!(*A)->WarpH)
					{
						(*A)->SetPos({ (*A)->GetPos().x ,(*A)->GetPos().y + 720 });
						(*A)->WarpH = true;
					}
					else
					{
						A = SubBullet.erase(A);
						continue;
					}
				}
			}
		}
		else if (DisPlayOutH((*A)->GetPos()) || DisPlayOutW(GetPos()))
		{
			A = SubBullet.erase(A);
			continue;
		}
		++A;
	}
	for (auto A = BurstBullet.begin(); A != BurstBullet.end(); )
	{
		if (BuWarp)
		{
			{
				if ((*A)->GetPos().x > 640)
				{
					if (!(*A)->WarpW)
					{
						(*A)->SetPos({ (*A)->GetPos().x - 1280,(*A)->GetPos().y });
						(*A)->WarpW = true;
					}
					else
					{
						A = BurstBullet.erase(A);
						continue;
					}
				}
				if ((*A)->GetPos().x < -640)
				{
					if (!(*A)->WarpW)
					{
						(*A)->SetPos({ (*A)->GetPos().x + 1280,(*A)->GetPos().y });
						(*A)->WarpW = true;
					}
					else
					{
						A = BurstBullet.erase(A);
						continue;
					}
				}
			}
			{
				if ((*A)->GetPos().y > 360)
				{
					if (!(*A)->WarpH)
					{
						(*A)->SetPos({ (*A)->GetPos().x ,(*A)->GetPos().y - 720 });
						(*A)->WarpH = true;
					}
					else
					{
						A = BurstBullet.erase(A);
						continue;
					}
				}
				if ((*A)->GetPos().y < -360)
				{
					if (!(*A)->WarpH)
					{
						(*A)->SetPos({ (*A)->GetPos().x ,(*A)->GetPos().y + 720 });
						(*A)->WarpH = true;
					}
					else
					{
						A = BurstBullet.erase(A);
						continue;
					}
				}
			}
		}
		else if (DisPlayOutH((*A)->GetPos()) || DisPlayOutW(GetPos()))
		{
			A = BurstBullet.erase(A);
			continue;
		}
		++A;
	}
}

int C_Player::Attack(std::vector<Bullet*>& Bu, int A,int B)
{
	//弾の発射をしてクールタイムをreturnで返す
	if (A == Straight)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == fast)
	{
		Bu.push_back(new Bullet(A));
		return 15;
	}
	else if (A == Missile)
	{
		for (int i = 0; i < ((2 + 2 * PLAYER.BulletPower[Missile])); i++)
			Bu.push_back(new Bullet(A));
		return 30;
	}
	else if (A == Auto)
	{
		if (AutoAttack)
		{
			Bu.push_back(new Bullet(A));
			return 10;
		}
	}
	else if (A == HighPower)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Burst)
	{
		Bu.push_back(new Bullet(A));
		return 20;
	}
	else if (A == BurstP)
	{
		Bu.push_back(new Bullet(A));
		return B;
	}
	else if (A == Penetration)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Charge)
	{
		Bu.push_back(new Bullet(A));
		return 40;
	}
	else if (A == Fire)
	{
		Bu.push_back(new Bullet(A));
		return 20;
	}
	else if (A == Laesr)
	{
		Bu.push_back(new Bullet(A));
		return 40;
	}
	else if (A == Gravity)
	{
		Bu.push_back(new Bullet(A));
		return 40;
	}
	else if (A == Homing)
	{
		Bu.push_back(new Bullet(A));
		return 12;
	}
	else if (A == RailGun)
	{
		Bu.push_back(new Bullet(A));
		return 80;
	}
	else if (A == Chain)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Virus)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Moon)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Commet)
	{
		Bu.push_back(new Bullet(A));
		return 5;
	}
	else if (A == Sun)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Poizon)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Conti)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == SDust)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}

	return B;
}

void C_Player::AutoAT()
{
	Math::Vector2 Up;
	Math::Vector2 Down;
	const int ABC = 64 * 3;
	Up = { Pos.x + ABC,Pos.y + ABC };
	Down = { Pos.x - ABC ,Pos.y - ABC };
	auto Compere = [](Math::Vector2 A, Math::Vector2 B, Math::Vector2 C, float* Dis, int Cnt) -> bool
		{
			if (!(A.x > C.x && A.x < B.x && A.y > C.y && A.y < B.y))
			{
				return false; // 範囲条件を満たさない場合は早めにリターン
			}

			float dx = PLAYER.GetPos().x - A.x;
			float dy = PLAYER.GetPos().y - A.y;
			float c = sqrt(dx * dx + dy * dy);

			if (c < *Dis && Cnt < GM.AllEnemy.size()) // 最小値を更新する条件
			{
				*Dis = c;
				PLAYER.DisCnt = Cnt;
				PLAYER.ReCnt = false;
			}
			return true;
		};

	bool ATT = false;

	AutoDis = FLT_MAX;
	DisCnt = 0;

	ECnt = 0;
	for (auto A : GM.AllEnemy)
	{
		if (!A) continue;
		if (Compere(A->GetPos(), Up, Down, &AutoDis, ECnt))
		{

			ATT = true;
		}
		ECnt++;
	}

	if (ATT)
	{
		if (DisCnt >= 0 && DisCnt < GM.AllEnemy.size()) {
			auto it = GM.AllEnemy.begin() + DisCnt;
			PLAYER.AutoDeg = ToDegrees(atan2(PLAYER.GetPos().y - (*it)->GetPos().y, PLAYER.GetPos().x - (*it)->GetPos().x));


			AutoX = AutoSpd * -cos(ToRadians(AutoDeg));
			AutoY = AutoSpd * -sin(ToRadians(AutoDeg));
			AutoAttack = true;
		}
		return;
	}
	AutoAttack = false;
}

void C_Player::CellGet(Math::Vector2 A, Cell* B)
{
	//おおまかな分割
	int X = A.x / 64;
	int Y = A.y / 72;

	// さらにセル内で4分割（縦横それぞれ2分割）
	int localX = ((int)A.x % 64 + 64) % 64; // X方向のセル内の位置（0 ~ 63）
	int localY = ((int)A.y % 72 + 72) % 72; // Y方向のセル内の位置（0 ~ 71）

	// 4分割の細かい区画を計算（セル内で32×36の区画を識別）
	int SubX = localX / (64 / 2); // 0 ~ 1（縦方向の区画）
	int SubY = localY / (72 / 2); // 0 ~ 1（横方向の区画）
	
	//左上
	if (SubX == 0 && SubY == 0)
	{
		B->Cell1X = X - 1;
		B->Cell1Y = Y - 1;

		B->Cell2X = X;
		B->Cell2Y = Y - 1;

		B->Cell3X = X - 1;
		B->Cell3Y = Y;

		B->Cell4X = X;
		B->Cell4Y = Y;
	}
	//右上
	else if (SubX == 1 && SubY == 0)
	{
		B->Cell1X = X;
		B->Cell1Y = Y - 1;
		
		B->Cell2X = X + 1;
		B->Cell2Y = Y - 1;

		B->Cell3X = X;
		B->Cell3Y = Y;

		B->Cell4X = X + 1;
		B->Cell4Y = Y;
	}
	//左下
	else if (SubX == 0 && SubY == 1)
	{
		B->Cell1X = X - 1;
		B->Cell1Y = Y;

		B->Cell2X = X;
		B->Cell2Y = Y;

		B->Cell3X = X - 1;
		B->Cell3Y = Y+1;

		B->Cell4X = X;
		B->Cell4Y = Y+1;
	}
	//右下
	else if (SubX == 1 && SubY == 1)
	{
		B->Cell1X = X;
		B->Cell1Y = Y;

		B->Cell2X = X + 1;
		B->Cell2Y = Y;

		B->Cell3X = X;
		B->Cell3Y = Y + 1;

		B->Cell4X = X + 1;
		B->Cell4Y = Y + 1;

	}
}