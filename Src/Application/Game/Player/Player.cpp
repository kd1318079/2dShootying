#include "Player.h"
#include "../../main.h"
#include "PlayerBlockTex.h"
#include "../Game.h"
#include <memory>

void C_Player::Init()
{
	PlayerTex = BlTEX.GetTex(5);
	AimeTex.Load("Texture/Lockon.png");
}

void C_Player::Update()
{
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
	for (auto Main : MainBullet) Main->Draw();
	for (auto Sub : SubBullet) Sub->Draw();
	for (auto SP : SPBullet) SP->Draw();
	for (auto BU : BurstBullet) BU->Draw();

	rect = { 0,0,32,32 };
	SHADER.m_spriteShader.SetMatrix(Mat);
	SHADER.m_spriteShader.DrawTex(PlayerTex, rect);

	rect = { 0,0,64,64 };
	SHADER.m_spriteShader.SetMatrix(AimMat);
	SHADER.m_spriteShader.DrawTex(&AimeTex, rect);
	Math::Matrix A = Math::Matrix::CreateTranslation(0,0,0);


	for (auto Exp : BulletExp) Exp->Draw();

}

void C_Player::Release()
{
	AimeTex.Release();
}

void C_Player::MouseGet()
{
	//マウス座標取得
	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640 - Scroll.x;
	mousePos.y -= 360 + Scroll.y;
	//mousePos.x *= -1;
	mousePos.y *= -1;
}

void C_Player::MovePlayer()
{
	//Aimeを回す処理
	AimeDeg += 3;
	if (AimeDeg >= 360)AimeDeg = 0;
	//機体サイズでスピードを落とす
	PlayerSpd = 3.0;
	float Su = 6;

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
		if (GetAsyncKeyState('A') & 0x8000)
		{
			Move.x -= PlayerSpd;
			if (SHIFT)Move.x -= PlayerSpd * Su;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			Move.x += PlayerSpd;
			if (SHIFT)Move.x += PlayerSpd * Su;
		}
		if (GetAsyncKeyState('W') & 0x8000)
		{
			Move.y += PlayerSpd;
			if (SHIFT)Move.y += PlayerSpd * Su;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			Move.y -= PlayerSpd;
			if (SHIFT)Move.y -= PlayerSpd * Su;
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

	Scroll = Pos;
	ScrollMax();
	Main = Pos - Scroll;

}

void C_Player::PlayerDegSet()
{
	float Deg = ToDegrees(atan2(Pos.y - mousePos.y, Pos.x - mousePos.x)) + 90;
	PlayerDeg = Deg;

}

void C_Player::PlayerAttack()
{
	//完全自動攻撃のフラグ管理
	//if(BulletCnt == Auto)
		AutoAT();
	//弾のクールダウン
	MainBulletTime--;
	SubBulletTime--;
	SPBulletTime--;
	//多くなりすぎた弾を前から順に削除
	ATerase();

	//自動攻撃
	if(MainBulletTime <= 0)MainBulletTime = Attack(MainBullet, BulletCnt,MainBulletTime);

	//サブ攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && SubBulletTime <= 0)
	{
		SubBulletTime = Attack(SubBullet, SubBulletCnt,SubBulletTime);
	}
	//特殊
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
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
	if(MissileM)Attack2(MainBullet, BulletCnt ,&MainBulletTime , &MissileM);
	if(MissileS)Attack2(SubBullet, SubBulletCnt,&SubBulletTime , &MissileS);

	BurstDegF();
	FireExpF();
	if(LaesrF)LaesrFCnt();
	GraviFCnt();

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
	Trans = Math::Matrix::CreateTranslation(mousePos.x -Scroll.x, mousePos.y - Scroll.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(AimeDeg));
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);
	AimMat = Scale * Rota * Trans;

	Trans = Math::Matrix::CreateTranslation(Main.x, Main.y, 0);
	Rota = Math::Matrix::CreateRotationZ(ToRadians(PlayerDeg));
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Scale * Rota * Trans;
}

void C_Player::BurstDegF()
{
	auto BurstFlg = [](std::vector<Bullet*>& Bu) -> void
		{
			for (auto A = Bu.begin(); A != Bu.end(); )
			{
				PLAYER.BurstDeg = 0;
				if ((*A)->BurstF)
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
				++A; // 次の要素へ進む
			}
		};
	BurstFlg(MainBullet);
	BurstFlg(SubBullet);
	BurstFlg(SPBullet);
}

void C_Player::FireExpF()
{
	auto FireFlg = [](std::vector<Bullet*>& Bu) -> void {
		for (auto A = Bu.begin(); A != Bu.end(); ) {
			if ((*A)->FireF) {
				PLAYER.ExpPos = (*A)->GetPos();
				// BulletTypeに応じてExplosionを追加
				if ((*A)->BulletType == Fire)
					PLAYER.BulletExp.push_back(new Explosion(120,2));
				else
					PLAYER.BulletExp.push_back(new Explosion(40));

				// 現在の要素を安全に削除し、イテレーターを更新
				A = Bu.erase(A); // eraseは次の要素を指す有効なイテレーターを返します
			}
			else {
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

void C_Player::GraviFCnt()
{
	if (SPBulletCnt == Gravity)
	{
		for (auto A = SPBullet.begin(); A != SPBullet.end();)
		{
			(*A)->GraCnt--;
			if ((*A)->GraCnt < 0)
			{
				(*A)->GraCnt = 0;
				ExpPos = (*A)->GetPos();
				A = SPBullet.erase(A);
				BulletExp.push_back(new Explosion(120, 5,1));
				continue;
			}
			++A;
		}
	}
}

void C_Player::ScrollMax()
{
	if (Scroll.y > scrolUP) {
		Scroll.y = scrolUP;
	}
	if (Scroll.y < scrolDOWN) {
		Scroll.y = scrolDOWN;
	}
	if (Scroll.x < scrolMIN) {
		Scroll.x = scrolMIN;
	}
	if (Scroll.x > scrolMAX) {
		Scroll.x = scrolMAX;
	}
}

void C_Player::ATerase()
{
	if (MainBullet.size() >= 80)
	{
		while (MainBullet.size() >= 80)
		{
			MainBullet.erase(MainBullet.begin());
		}
	}
	if (SubBullet.size() >= 50)
	{
		while (SubBullet.size() >= 50)
		{
			SubBullet.erase(SubBullet.begin());
		}
	}
	if (SPBullet.size() >= 20)
	{
		while (SPBullet.size() >= 20)
		{
			SPBullet.erase(SPBullet.begin());
		}
	}
	if (BurstBullet.size() >= 120)
	{
		while (BurstBullet.size() >= 120)
		{
			BurstBullet.erase(BurstBullet.begin());
		}
	}
	if (BulletExp.size() >= 200) {
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
		if (MainBullet == Bu)
		{
			if (!MissileM)
			{
				MissileM = true;
				return 0;
			}
		}
		else
		{
			if (!MissileS)
			{
				MissileS = true;
				return 0;
			}
		}
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
		return 10;
	}
	else if (A == Sun)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}
	else if (A == Return)
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
	else if (A == SLight)
	{
		Bu.push_back(new Bullet(A));
		return 10;
	}

	return B;
	
}

void C_Player::Attack2(std::vector<Bullet*>& Bu, int A, int* B, bool* F)
{
	Bu.push_back(new Bullet(A));
	if (*B <= -10)
	{
		*F = false;
		*B = 200;
	}
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

			if (c < *Dis && Cnt < GM.MobEnemy.size()) // 最小値を更新する条件
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
	for (auto A : GM.MobEnemy)
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
		if (DisCnt >= 0 && DisCnt < GM.MobEnemy.size()) {
			auto it = GM.MobEnemy.begin() + DisCnt;
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