#include "Player.h"
#include "../../main.h"
#include "PlayerBlockTex.h"
#include "../Game.h"
#include <memory>

void C_Player::ReturnInit()
{
	Alive = true;
}

void C_Player::Init()
{
	PlayerTex = BlTEX.GetTex(0);
	DegTex = BlTEX.GetTex(1);
	AimeTex.Load("Texture/others/Lockon.png");
	BackTex.Load("Texture/others/BackG.png");
}

void C_Player::Update()
{
	EnemyB();

	//�}�E�X���W�Q�b�g
	MouseGet();
	//���@�̈ړ�
	MovePlayer();
	//�v���C���[��deg������
	PlayerDegSet();
	//�v���C���[�̍U��(BuleetCnt�Œe�ύX)
	PlayerAttack();
	//�v���C���[�̃Z�����擾
	if (APP.count % 2 == 0)CellGet(Pos,&HitJ);
	//������Update
	ExplosionUpdate();
	//���@�ƃ}�E�X���W�̌���
	SetMat();

}

void C_Player::Draw()
{
	//�e�`��
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
	//�O���r�e�B
	for (auto Exp : BulletExp) if (Exp->EType == 4)Exp->Draw();
}

void C_Player::Release()
{
	AimeTex.Release();
	BackTex.Release();
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
				++A; // ���̗v�f�֐i��
			}

		}
	}
}

void C_Player::MouseGet()
{
	//�}�E�X���W�擾
	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	//mousePos.x *= -1;
	mousePos.y *= -1;
}

void C_Player::MovePlayer()
{
	//Aime���񂷏���
	AimeDeg += 3;
	if (AimeDeg >= 360)AimeDeg = 0;
	//�@�̃T�C�Y�ŃX�s�[�h�𗎂Ƃ�
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

	// �����̌v�Z
	float diff = Deg - PlayerDeg;

	// �p�x�͈͕␳ (-180�`180)
	if (diff > 180) diff -= 360;
	if (diff < -180) diff += 360;

	// ���X�Ɋp�x��ω�������
	if (abs(diff) < DegSpeed / 2)
	{
		PlayerDeg = Deg;
	}
	else
	{
		if (diff < 0)PlayerDeg -= DegSpeed;
		else PlayerDeg += DegSpeed;
	}
	// �͈͕␳ (0�`360�x)
	if (PlayerDeg >= 360) PlayerDeg -= 360;
	if (PlayerDeg < 0) PlayerDeg += 360;

}

void C_Player::PlayerAttack()
{
	//���S�����U���̃t���O�Ǘ�
	//if(BulletCnt == Auto)
		AutoAT();
	//�e�̃N�[���_�E��
	MainBulletTime--;
	SubBulletTime--;
	SPBulletTime--;
	//�����Ȃ肷�����e��O���珇�ɍ폜
	ATerase();

	//�����U��
	if(MainBulletTime <= 0)MainBulletTime = Attack(MainBullet, BulletCnt,MainBulletTime);

	//�T�u�U��
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && SubBulletTime <= 0)
	{
		SubBulletTime = Attack(SubBullet, SubBulletCnt,SubBulletTime);
	}
	//����
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

void C_Player::FireExpF()
{
	auto FireFlg = [](std::vector<Bullet*>& Bu) -> void {
		for (auto A = Bu.begin(); A != Bu.end(); ) {
			if ((*A)->FireF) {
				PLAYER.ExpPos = (*A)->GetPos();
				// BulletType�ɉ�����Explosion��ǉ�
				if ((*A)->BulletType == Fire)
					PLAYER.BulletExp.push_back(new Explosion(72,5,2));
				else
					PLAYER.BulletExp.push_back(new Explosion(40,1,3));

				// ���݂̗v�f�����S�ɍ폜���A�C�e���[�^�[���X�V
				A = Bu.erase(A); // erase�͎��̗v�f���w���L���ȃC�e���[�^�[��Ԃ��܂�
			}
			else if ((*A)->BurstF)
			{
				PLAYER.BurstPos = (*A)->GetPos();
				// Bu����A���폜
				A = Bu.erase(A); // �C�e���[�^�����̗v�f�ɐi�߂��y��
				for (int i = 0; i < PLAYER.BurstMax; i++)
				{
					PLAYER.BurstDeg += 360 / PLAYER.BurstMax;
					PLAYER.BurstBullet.push_back(new Bullet(BurstP));
				}
				continue; // erase���A���C���N�������g���Ȃ�
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
				// BulletType�ɉ�����Explosion��ǉ�
				if ((*A)->BulletType == SDust)
					PLAYER.BulletExp.push_back(new Explosion(120, 10));
				else
					PLAYER.BulletExp.push_back(new Explosion(18, 1, 3));

				// ���݂̗v�f�����S�ɍ폜���A�C�e���[�^�[���X�V
				A = Bu.erase(A); // erase�͎��̗v�f���w���L���ȃC�e���[�^�[��Ԃ��܂�
			}
			else 
			{
				++A; // ���̗v�f�֐i��
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
	if (MainBullet.size() >= 150)
	{
		while (MainBullet.size() >= 80)
		{
			MainBullet.erase(MainBullet.begin());
		}
	}
	if (SubBullet.size() >= 150)
	{
		while (SubBullet.size() >= 50)
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
	if (BurstBullet.size() >= 150)
	{
		while (BurstBullet.size() >= 120)
		{
			BurstBullet.erase(BurstBullet.begin());
		}
	}
	if (BulletExp.size() >= 400) {
		BulletExp.erase(BulletExp.begin(), BulletExp.begin() + (BulletExp.size() - 199));
	}
	for (auto A = BulletExp.begin(); A != BulletExp.end(); ) {
		if (*A == nullptr || (*A)->DeleteCnt <= 0) {
			A = BulletExp.erase(A); // �����܂��͍폜�����𖞂����v�f���폜
		}
		else {
			++A; // ���̗v�f�֐i��
		}
	}
}

int C_Player::Attack(std::vector<Bullet*>& Bu, int A,int B)
{
	//�e�̔��˂����ăN�[���^�C����return�ŕԂ�
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
		*B = 30;
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
				return false; // �͈͏����𖞂����Ȃ��ꍇ�͑��߂Ƀ��^�[��
			}

			float dx = PLAYER.GetPos().x - A.x;
			float dy = PLAYER.GetPos().y - A.y;
			float c = sqrt(dx * dx + dy * dy);

			if (c < *Dis && Cnt < GM.AllEnemy.size()) // �ŏ��l���X�V�������
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
	//�����܂��ȕ���
	int X = A.x / 64;
	int Y = A.y / 72;

	// ����ɃZ������4�����i�c�����ꂼ��2�����j
	int localX = ((int)A.x % 64 + 64) % 64; // X�����̃Z�����̈ʒu�i0 ~ 63�j
	int localY = ((int)A.y % 72 + 72) % 72; // Y�����̃Z�����̈ʒu�i0 ~ 71�j

	// 4�����ׂ̍��������v�Z�i�Z������32�~36�̋������ʁj
	int SubX = localX / (64 / 2); // 0 ~ 1�i�c�����̋��j
	int SubY = localY / (72 / 2); // 0 ~ 1�i�������̋��j
	
	//����
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
	//�E��
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
	//����
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
	//�E��
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