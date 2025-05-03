#pragma once
#include "../Scene.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "../Define.h"
#include "UI/UI.h"

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

	UI m_UI;

	void SetSceneChangeCnt();
	const int SceneMax = 1;

	std::vector<int> Enemy5Cnt;
	std::vector<Math::Vector2> Enemy5PosCnt;

	void WaveUp();
	void EnemyPop();

	int  CostSet(int i);
	POINT mousePos = {0,0};
	//MaxHとMaxWは基準(HitPos)から半分の距離をとって範囲とする
	bool MousePosHit(Math::Vector2 HitPos, float MaxH ,float MaxW);

	void SetScene(int i) { NowScene = i; }
	int GetScene() { return NowScene; }
private:
	void MouseGet();


	int NowScene = Title;
	bool KeyFlg = false;
	bool KeyFlg1 = false;
	bool SceneCh = false;

	int Count = 0;

	int EnemyInter = 250;

	void SceneUp();

	//シーン遷移の描画
	void SC_Draw0();

	//
	Math::Vector2 BackPos[18][32];

	KdTexture TitleTex;
	KdTexture TitleTextTex;
	KdTexture StartTex;
	KdTexture QuitTex;
	KdTexture SterTex;
	KdTexture PowerTex;
	KdTexture StetasTex;
	KdTexture StetasSPTex;
	
	KdTexture BulletIconTex[BulletMax];


	NumDraw* SteNum[6];
	void SteNumUpdate();
	void SteNumDraw();

	void TitleUpdate();
	void TitleDraw();
	std::vector<Math::Vector2> SterCnt;

	void PowerUpdate();
	void PowerUPDraw();
	//触れているアイコンの番号を保存
	int Repla = -1;
	int ReplaMain = -1;
	int ReplaSub = -1;

	void ReplaUpdate();
	void ReplaPostUpdate();

	int MouseIconCnt = 0;
	Math::Vector3 Power1Pos = { -50,-50,0 };
	Math::Vector3 Power2Pos = { 200,-50,0 };
	Math::Vector3 Power3Pos = { 450,-50,0 };
	
	Math::Vector3 StetasPos = { -640,0,0 };

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