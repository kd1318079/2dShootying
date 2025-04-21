#pragma once
#include"../Scene.h"
#include"Player/Player.h"
#include"Enemy/Enemy.h"

class MainGame
{
	
public:

	void Init();
	void Update();
	void Draw();
	void Release();

	std::vector<Enemy*> MobEnemy;
	std::vector<Enemy*> BossEnemy;

	MainGame() {};
	~MainGame() {};

private:
	

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