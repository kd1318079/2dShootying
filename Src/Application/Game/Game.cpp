#include "Game.h"
#include "Player/PlayerBlockTex.h"
#include "../main.h"
#include "PowerDraw.h"

void MainGame::Init()
{
	PLAYER.Init();
	m_UI.Init();
	TitleTex.Load("Texture/others/Title.png");
	StartTex.Load("Texture/others/TitleS.png");
	QuitTex.Load("Texture/others/TitleQ.png");
	TitleTextTex.Load("Texture/others/TitleText.png");
	SterTex.Load("Texture/others/Ster.png");
	PowerTex.Load("Texture/others/PowerUP.png");
	StetasTex.Load("Texture/others/Stetas.png");
	StetasSPTex.Load("Texture/others/StetasSP.png");
	for (int i = 0; i < BulletMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/BulletIcon/Bullet%d.png", i);
		BulletIconTex[i].Load(str);//scene.hにも書いた通りキャラ名と添え字でキャラ別のTextureをつくる
	}

	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			BackPos[i][j] = { (float) - 640 + 40 * j , (float)-340.0 + 40 * i};
		}
	}

	SteNum[0] = new NumDraw({-470 , 285}, 0);
	SteNum[1] = new NumDraw({-470 , 240}, 0);
	SteNum[2] = new NumDraw({-470 , 195}, 0);
	SteNum[3] = new NumDraw({-470 , 150}, 0);
	SteNum[4] = new NumDraw({-470 , 102}, 0);
	SteNum[5] = new NumDraw({-470 , 48}, 0);

}

void MainGame::Update()
{
	m_UI.Update();
	Count++;
	SteNumUpdate();
	MouseGet();
	if(NowScene == Title)
	{
		TitleUpdate();
	}
	else if (NowScene == Game)
	{
		PLAYER.Update();
		for (auto it = AllEnemy.begin(); it != AllEnemy.end(); )
		{
			if ((*it)->Death) {
				delete* it;
				*it = nullptr;
				it = AllEnemy.erase(it);
			}
			else {
				(*it)->Update();
				++it;
			}
		}
		if (!Enemy5PosCnt.empty())
		{
			for (auto A : Enemy5PosCnt)
			{
				for (auto B : Enemy5Cnt)
				{
					for (int i = 0; i < B; i++)
					{
						AllEnemy.push_back(new Enemy(5, A));
					}
				}
			}
			Enemy5Cnt.clear();
			Enemy5PosCnt.clear();
		}
		//敵がいなくなるとwaveを一つあげて敵の出現
		WaveUp();
	}
	else if (NowScene == Result)
	{

	}
	else if (NowScene == LevelUp || NowScene == Select)
	{
		PowerUpdate();
	}




}

void MainGame::Draw()
{
	if (NowScene == Title)
	{
		//背景描画
		TitleDraw();
	}
	else if (NowScene == Game || NowScene == LevelUp || NowScene == Select)
	{
		PLAYER.PreDraw();
		for (auto A : AllEnemy) A->Draw();
		PLAYER.Draw();
		m_UI.Draw();
		if(NowScene == LevelUp || NowScene == Select)PowerUPDraw();
	}
	else if (NowScene == Result)
	{

	}

	SceneUp();
	//シーン変更の際の描画
	SceneChangeDraw(SceneChangeCnt);
	
}

void MainGame::Release()
{
	PLAYER.Release();
	m_UI.Release();
	TitleTex.Release();
	TitleTextTex.Release();
	StartTex.Release();
	QuitTex.Release();
	SterTex.Release();
	PowerTex.Release();
	StetasTex.Release();
	StetasSPTex.Release();
	for (int i = 0; i < BulletMax; i++)
	{
		BulletIconTex[i].Release();
	}
}

void MainGame::SceneUp()
{


	if (GetAsyncKeyState(VK_TAB))
	{
		if (!KeyFlg1)
		{
			KeyFlg1 = true;
			if (NowScene == Game)
			{
				NowScene = Select;
			}
			else if (NowScene == Select)
			{
				NowScene = Game;
			}
		} 
	}
	else KeyFlg1 = false;


	if (GetAsyncKeyState(VK_LBUTTON))
	{
		//Bulletの入れ替え
		if (NowScene == LevelUp || NowScene == Select)
		{
			if(MouseIconCnt == 0) ReplaUpdate();

		}

		if (!KeyFlg && !SceneCh)
		{
			KeyFlg = true;
			switch (NowScene)
			{
			case Title:
				//スタート
				if (MousePosHit({ 0,-100 },85 , 460))
				{
					SceneCh = true;
					SetSceneChangeCnt();
				}
				else if((MousePosHit({ 0,-210}, 85, 460)))
				{
					exit(0);
				}
				break;
			case Game:
				//今だけ死んだ時に押すとに
				if(!PLAYER.Alive) SetSceneChangeCnt();


				break;
			case Result:
				SetSceneChangeCnt();

				break;
			case LevelUp:
				//左
				if (MousePosHit({ Power1Pos.x,Power1Pos.y }, 480, 220))
				{
					PLAYER.LevelUpdate(PLAYER.LevelCnt1);
				}
				//中
				else if (MousePosHit({ Power2Pos.x,Power2Pos.y }, 480, 220))
				{
					PLAYER.LevelUpdate(PLAYER.LevelCnt2);
				}
				//右
				else if (MousePosHit({ Power3Pos.x,Power3Pos.y }, 480, 220))
				{
					PLAYER.LevelUpdate(PLAYER.LevelCnt3);
				}
				//それ以外の場所ならbreak で返す
				else break;
				
				NowScene = Game;
				PLAYER.LevelCnt = 60;
				break;
			}
		}
	}
	else
	{
		KeyFlg = false;
		if (MouseIconCnt != 0) ReplaPostUpdate();
	}
	if (SceneCT && !SceneCC)
	{
		switch (NowScene)
		{
		case Title:
			NowScene = Game;
			PLAYER.ReturnInit();
			break;
		case Game:
			NowScene = Result;

			break;
		case Result:
			NowScene = Title;

			break;

		}
		SceneCC = true;
	}


}

void MainGame::SC_Draw0()
{
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			Math::Rectangle rect;
			if (SceneCnt > 0)
			{
				long A = abs(SceneMaxCnt - SceneCnt);
				long B = abs(SceneMaxCnt - SceneCnt) - 40;
				if (B < 0)B = 0;

				if (i % 2 == 0 && j % 2 == 1) rect = { 0,0,A, 40 };
				if (i % 2 == 1 && j % 2 == 0) rect = { 0,0,A, 40 };
				if (i % 2 == 0 && j % 2 == 0) rect = { 0,0,B, 40 };
				if (i % 2 == 1 && j % 2 == 1) rect = { 0,0,B, 40 };
			}
			else 
			{
				long A = (40 - abs(SceneCnt));
				long B = (80 - abs(SceneCnt));
				if (A < 0)A = 0;
				if (B < 0)B = 0;
				if (i % 2 == 0 && j % 2 == 1) rect = { 0,0,A, 40 };
				if (i % 2 == 1 && j % 2 == 0) rect = { 0,0,A, 40 };
				if (i % 2 == 0 && j % 2 == 0) rect = { 0,0,B, 40 };
				if (i % 2 == 1 && j % 2 == 1) rect = { 0,0,B, 40 };
			}
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(BackPos[i][j].x, BackPos[i][j].y, 0));
			SHADER.m_spriteShader.DrawTex(BlTEX.GetTex(2), rect, 1, { 0,0.5 });
		}
	}

}

void MainGame::SteNumUpdate()
{
	for (int i = 0; i < 6; i++)
	{
		SteNum[i]->Cnt++;
	}
	SteNum[0]->Dmg = PLAYER.Wave;
	SteNum[1]->Dmg = PLAYER.Level;
	SteNum[2]->Dmg = PLAYER.GetMaxHP();
	SteNum[3]->Dmg = PLAYER.GetDEF();
	SteNum[4]->Dmg = PLAYER.GetSpd() * 100;
	SteNum[5]->Dmg = PLAYER.GetDegSpd() * 100;

}

void MainGame::SteNumDraw()
{
	for (int i = 0; i < 6; i++)
	{
		SteNum[i]->Draw(0, false , true);
	}
}

void MainGame::TitleUpdate()
{
	if (rand() % 50 == 0)
	{
		SterCnt.push_back({rand() % 1280 - 640.0f,360.0f});
	}
	for (auto A = SterCnt.begin(); A != SterCnt.end();)
	{
		(*A).x -= 0.5;
		(*A).y -= 5 + rand() % 5;
		if ((*A).y < -360)
		{
			A = SterCnt.erase(A);
			continue;
		}else ++A;
	}

}

void MainGame::TitleDraw()
{

	Math::Rectangle rect = { 0,0,1280,720 };
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1) * Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(&TitleTex, rect);
	//流れ星
	D3D.SetBlendState(BlendMode::Add);
	for (auto A : SterCnt)
	{
		Math::Rectangle rect = { 0,0,1,1 };
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(2) * Math::Matrix::CreateTranslation(A.x, A.y, 0));
		SHADER.m_spriteShader.DrawTex(&SterTex, rect);
	}

	D3D.SetBlendState(BlendMode::Alpha);
	rect = { 0,0,1280,720 };
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1) * Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(&TitleTextTex, rect);
	Math::Color C = { 1,1,1,1 };
	
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1) * Math::Matrix::CreateTranslation(0, 0, 0));

	if (MousePosHit({ 0,-100 }, 85, 460))
	{
		C = { 0.97f, 1.00f, 0.52f ,1 };
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1.05) * Math::Matrix::CreateTranslation(0, 0, 0));
	}
	SHADER.m_spriteShader.DrawTex_Color(&StartTex, rect,C);

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1) * Math::Matrix::CreateTranslation(0, 0, 0));

	C = { 1,1,1,1 };
	if (MousePosHit({ 0,-210 }, 85, 460))
	{
		C = { 0.97f, 1.00f, 0.52f ,1 };
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1.05) * Math::Matrix::CreateTranslation(0, 0, 0));
	}
	SHADER.m_spriteShader.DrawTex_Color(&QuitTex, rect,C);
}

void MainGame::PowerUpdate()
{
	//アイコンとの当たり判定
	//基準
	Math::Vector3 Sta = { -640 + 24 + 12, 360 - 664 + 12 ,0 };
	//弾アイコン描画

	for (int i = 0; i < PLAYER.OthersBullet.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;

		if (Repla == -1 || Repla == i)
		{
			if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
			{
				Repla = i;
				break;
			}
			else Repla = -1;
		}
	}
	//Main
	Sta.y += 240 - 4;
	for (int i = 0; i < PLAYER.MainBu.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		if (ReplaMain == -1 || ReplaMain == i)
		{
			if (MouseIconCnt == HighPower || MouseIconCnt == Gravity || MouseIconCnt == Charge
				|| MouseIconCnt == Laesr || MouseIconCnt == RailGun); else
			{
				if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
				{
					ReplaMain = i;
					break;
				}
				else ReplaMain = -1;
			}
		}
	}
	//Sub
	Sta.y -= 120 + 4;
	for (int i = 0; i < PLAYER.SubBu.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		if (MouseIconCnt == HighPower || MouseIconCnt == Gravity || MouseIconCnt == Charge
			|| MouseIconCnt == Laesr || MouseIconCnt == RailGun); else
		{
			if (ReplaSub == -1 || ReplaSub == i)
			{
				if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
				{
					ReplaSub = i;
					break;
				}
				else ReplaSub = -1;
			}
		}
	}
}

void MainGame::PowerUPDraw()
{
	//背景の黒
	Math::Rectangle rect = { 0,0 ,1280,720 };
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation({0,0,0}));
	SHADER.m_spriteShader.DrawTex_Color(&SterTex, rect, {0,0,0,0.8});

	//ステータス部分
	rect = { 0,0 ,400,720 };
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(StetasPos));
	SHADER.m_spriteShader.DrawTex(&StetasTex, rect, 1, { 0,0.5 });
	if(PLAYER.SPBu)	SHADER.m_spriteShader.DrawTex(&StetasSPTex, rect, 1, { 0,0.5 });

	SteNumDraw();

	rect = { 0,0 ,24,24 };
	//基準
	Math::Vector3 Sta = { -640 + 24 + 12, 360 - 664 +12 ,0};
	//弾アイコン描画

	for (int i = 0;i < PLAYER.OthersBullet.size();i++ )
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		float Size = 1;
		if (Repla == i)Size *= 1.2;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(BPos));
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[0], rect, 1);
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[PLAYER.OthersBullet[i]], rect, 1);
	}
	//Main
	Sta.y += 240 - 4;
	for (int i = 0; i < PLAYER.MainBu.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		float Size = 1;
		if (ReplaMain == i)Size *= 1.2;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(BPos));
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[0], rect, 1);
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[PLAYER.MainBu[i].Cnt], rect, 1);
	}
	//Sub
	Sta.y -= 120 + 4;
	for (int i = 0; i < PLAYER.SubBu.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		float Size = 1;
		if (ReplaSub == i)Size *= 1.2;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(BPos));
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[0], rect, 1);
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[PLAYER.SubBu[i].Cnt], rect, 1);
	}

	if (PLAYER.SPBu)
	{
		float Size = 1;
		Sta = { -264,-300,0 };
		if (MousePosHit({ Sta.x,Sta.y }, 24, 24))Size = 1.2;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(Sta));
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[0], rect, 1);
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[PLAYER.SPBulletCnt], rect, 1);
	}
	//強化背景
	//強化内容

	//レベルアップ時のみ
	rect = { 0,0 ,500,1000 };
	if (NowScene == LevelUp)
	{
		float Size = 0.5;

		if (MousePosHit({ Power1Pos.x,Power1Pos.y }, 480, 220)) Size = 0.6;
		else Size = 0.5;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(Power1Pos));
		SHADER.m_spriteShader.DrawTex(&PowerTex, rect);
		POWERTEX.Draw(PLAYER.LevelCnt1, rect);

		if (MousePosHit({ Power2Pos.x,Power2Pos.y }, 480, 220)) Size = 0.6;
		else Size = 0.5;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(Power2Pos));
		SHADER.m_spriteShader.DrawTex(&PowerTex, rect);
		POWERTEX.Draw(PLAYER.LevelCnt2, rect);

		if (MousePosHit({ Power3Pos.x,Power3Pos.y }, 480, 220)) Size = 0.6;
		else Size = 0.5;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(Size) * Math::Matrix::CreateTranslation(Power3Pos));
		SHADER.m_spriteShader.DrawTex(&PowerTex, rect);
		POWERTEX.Draw(PLAYER.LevelCnt3, rect);
	}

	if (MouseIconCnt != 0)
	{
		rect = { 0,0 ,24,24 };
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(mousePos.x,mousePos.y,0));
		SHADER.m_spriteShader.DrawTex(&BulletIconTex[MouseIconCnt], rect, 1, {0,1});
	}

}

void MainGame::ReplaUpdate()
{
	//アイコンとの当たり判定
	//基準
	Math::Vector3 Sta = { -640 + 24 + 12, 360 - 664 + 12 ,0 };
	//弾アイコン描画

	for (int i = 0; i < PLAYER.OthersBullet.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
		{
			MouseIconCnt = PLAYER.OthersBullet[i];
			PLAYER.OthersBullet[i] = 0;
			break;
		}
	}
	//Main
	Sta.y += 240 - 4;
	for (int i = 0; i < PLAYER.MainBu.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
		{
			MouseIconCnt = PLAYER.MainBu[i].Cnt;
			PLAYER.MainBu[i].Cnt = 0;
			break;
		}
	}
	//Sub
	Sta.y -= 120 + 4;
	for (int i = 0; i < PLAYER.SubBu.size(); i++)
	{
		Math::Vector3 BPos = Sta;
		BPos.x += i % 9 * 32;
		if (i >= 9)BPos.y -= 32;
		if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
		{
			MouseIconCnt = PLAYER.SubBu[i].Cnt;
			PLAYER.SubBu[i].Cnt = 0;
			break;
		}
	}
	//SP
	if (MousePosHit({ -264,-300  }, 24, 24))
	{
		MouseIconCnt = PLAYER.SPBulletCnt;
		PLAYER.SPBulletCnt = 0;
	}

}

void MainGame::ReplaPostUpdate()
{
	//基準
	Math::Vector3 Sta = { -640 + 24 + 12, 360 - 664 + 12 ,0 };
	//弾アイコン描画

	if (MouseIconCnt != HighPower && MouseIconCnt != Charge && MouseIconCnt != Laesr
		&& MouseIconCnt != Gravity && MouseIconCnt != RailGun)
	{
		for (int i = 0; i < PLAYER.OthersBullet.size(); i++)
		{
			Math::Vector3 BPos = Sta;
			BPos.x += i % 9 * 32;
			if (i >= 9)BPos.y -= 32;
			if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
			{
				for (int j = 0; j < PLAYER.OthersBullet.size(); j++)
				{
					if (PLAYER.OthersBullet[j] == 0)
					{
						PLAYER.OthersBullet[j] = PLAYER.OthersBullet[i];
						break;
					}
				}
				PLAYER.OthersBullet[i] = MouseIconCnt;
				MouseIconCnt = 0;
				break;
			}
		}
		//Main
		Sta.y += 240 - 4;
		for (int i = 0; i < PLAYER.MainBu.size(); i++)
		{
			Math::Vector3 BPos = Sta;
			BPos.x += i % 9 * 32;
			if (i >= 9)BPos.y -= 32;
			if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
			{
				for (int j = 0; j < PLAYER.OthersBullet.size(); j++)
				{
					if (PLAYER.OthersBullet[j] == 0)
					{
						PLAYER.OthersBullet[j] = PLAYER.MainBu[i].Cnt;
						break;
					}
				}
				PLAYER.MainBu[i].Cnt = MouseIconCnt;
				MouseIconCnt = 0;
				break;
			}
		}
		//Sub
		Sta.y -= 120 + 4;
		for (int i = 0; i < PLAYER.SubBu.size(); i++)
		{
			Math::Vector3 BPos = Sta;
			BPos.x += i % 9 * 32;
			if (i >= 9)BPos.y -= 32;
			if (MousePosHit({ BPos.x, BPos.y }, 24, 24))
			{
				for (int j = 0; j < PLAYER.OthersBullet.size(); j++)
				{
					if (PLAYER.OthersBullet[j] == 0)
					{
						PLAYER.OthersBullet[j] = PLAYER.SubBu[i].Cnt;
						break;
					}
				}
				PLAYER.SubBu[i].Cnt = MouseIconCnt;
				MouseIconCnt = 0;
				break;
			}
		}
	}
	//SP
	if (MousePosHit({ -264,-300 }, 24, 24))
	{
		for (int j = 0; j < PLAYER.OthersBullet.size(); j++)
		{
			if (PLAYER.OthersBullet[j] == 0)
			{
				PLAYER.OthersBullet[j] = PLAYER.SPBulletCnt;
				break;
			}
		}
		PLAYER.SPBulletCnt = MouseIconCnt;
		MouseIconCnt = 0;
	}
	if (MouseIconCnt != 0)
	{
		for (int i = 0; i < PLAYER.OthersBullet.size(); i++)
		{
			if (PLAYER.OthersBullet[i] == 0)
			{
				PLAYER.OthersBullet[i] = MouseIconCnt;
				break;
			}
		}
	}
	MouseIconCnt = 0;
}

void MainGame::SceneChangeDraw(int i)
{
	if (SceneCnt > -SceneMaxCnt)
	{
		SceneCnt--;
		switch (i)
		{
		case 0:
			SC_Draw0();
			break;
		}
		if (SceneCnt == 0)
		{
			SceneCT = true;
			SceneCC = false;
			SceneCh = false;
		}
	}
}

void MainGame::SetSceneChangeCnt()
{
	SceneChangeCnt = rand() % SceneMax;
	SceneCT = false;
	switch (SceneChangeCnt)
	{
	default:
		SceneCnt = 120;
		break;
	}

	SceneMaxCnt = SceneCnt;
}

void MainGame::WaveUp()
{
	if (AllEnemy.empty())
	{
		if (AllEnemy.size() == 0)
		{
			PLAYER.Wave++;
			EnemyPop();
		}
	}
}

void MainGame::EnemyPop()
{
	int AllCost = PLAYER.Wave * 2 + 1;
	while (AllCost > 0)
	{
		int Cost = 0;
		int A = rand() % 7;
		A = 3;
		while (1)
		{
			A = rand() % 7;
			if (A == 5)A = 4;
			Cost = CostSet(A);
			if (Cost <= AllCost)break;
		}
		AllEnemy.push_back(new Enemy(A));
		AllCost -= CostSet(A);
	}
}

int  MainGame::CostSet(int i)
{
	switch (i)
	{
	case 0:case 1:case2:case3:
	return 1;
		break;
	case 4:
		return 5;
		break;
	case 6:case 7:case 8:
		return 10;
		break;
	default:
		return 1;
		break;
	}
}

bool MainGame::MousePosHit(Math::Vector2 HitPos, float MaxH, float MaxW)
{
	Math::Vector2 A = {(float)mousePos.x, (float)mousePos.y};
	if (A.x < HitPos.x + MaxW / 2 && A.x > HitPos.x - MaxW / 2)
	{
		if (A.y < HitPos.y + MaxH / 2 && A.y > HitPos.y - MaxH / 2)
		{
			return true;
		}
	}

	return false;
}

void MainGame::MouseGet()
{
	//マウス座標取得
	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	mousePos.y *= -1;
}
