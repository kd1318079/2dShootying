#pragma once
#include"../Scene.h"
#include"Player/Player.h"
#include"Enemy/Enemy.h"
#include"../Define.h"

class MainGame
{
	
public:

	void Init();
	void Update();
	void Draw();
	void Release();

	std::vector<Enemy*> AllEnemy;

	MainGame() {};
	~MainGame() {};

	void SceneChangeDraw(int i);
	bool SceneCT = false;
	bool SceneCC = true;
	int SceneCnt = 0;
	int SceneMaxCnt = 0;

	//シーン変更時のタイプ
	int SceneChangeCnt = 0;


	void SetSceneChangeCnt();
	const int SceneMax = 1;

	std::vector<int> Enemy5Cnt;
	std::vector<Math::Vector2> Enemy5PosCnt;

private:
	
	int NowScene = Title;
	bool KeyFlg = false;
	bool SceneCh = false;

	int Count = 0;

	int EnemyInter = 250;

	void SceneUp();

	//シーン遷移の描画
	void SC_Draw0();


	//
	Math::Vector2 BackPos[18][32];



public:
	static MainGame& GetInstance()
	{
		static MainGame instance;
		return instance;
	}

	static C_Player& GetPlayerInstance()
	{
		static C_Player Inst;
		return Inst;
	}
};

#define GM MainGame::GetInstance()
#define PLAYER MainGame::GetPlayerInstance()