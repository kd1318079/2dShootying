#include "Game.h"
#include "Player/PlayerBlockTex.h"

void MainGame::Init()
{
	PLAYER.Init();
	AllEnemy.push_back(new Enemy(0));

	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			BackPos[i][j] = { (float) - 640 + 40 * j , (float)-340.0 + 40 * i};
		}
	}
}

void MainGame::Update()
{

	if(NowScene == Title)
	{
		//îwåiï`âÊ
		PLAYER.PreDraw();


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
		if (AllEnemy.empty())
		{
			if (AllEnemy.size() == 0)
			{
				for (int i = 0; i < 100; i++)
					AllEnemy.push_back(new Enemy(0));
			}
		}
	}
	else if (NowScene == Result)
	{

	}



}

void MainGame::Draw()
{
	if (NowScene == Title)
	{

	}
	else if (NowScene == Game)
	{
		PLAYER.PreDraw();
		for (auto A : AllEnemy) A->Draw();
		PLAYER.Draw();
	}
	else if (NowScene == Result)
	{

	}

	SceneUp();
	//ÉVÅ[ÉìïœçXÇÃç€ÇÃï`âÊ
	SceneChangeDraw(SceneChangeCnt);
}

void MainGame::Release()
{
	PLAYER.Release();

}

void MainGame::SceneUp()
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (!KeyFlg)
		{
			KeyFlg = true;
			switch (NowScene)
			{
			case Title:
				//NowScene = Game;
				SetSceneChangeCnt();
				//exit(0);
				break;
			case Game:
				//ç°ÇæÇØéÄÇÒÇæéûÇ…âüÇ∑Ç∆Ç…
				if(!PLAYER.Alive) SetSceneChangeCnt();


				break;
			case Result:
				SetSceneChangeCnt();


				break;
			}
		}
	}
	else KeyFlg = false;

	if (SceneCT && !SceneCC)
	{
		switch (NowScene)
		{
		case Title:
			NowScene = Game;
			PLAYER.ReturnInit();
			//exit(0);
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
