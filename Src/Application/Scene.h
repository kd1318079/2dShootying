#pragma once
#include"Load.h"
#include"Define.h"
#include <thread>

class Scene
{
public:

	void Init();
	void Release();
	void Update();
	void Draw2D();


	FILE* fp_c;
	void CreateConsole();
	void DestroyConsole();

	// GUIèàóù
	void ImGuiUpdate();
	
	bool CellHit(Cell A,Cell B);
	bool HitJudge(Math::Vector2 A, Math::Vector2 B,int size);
private:
	bool HitCom(int AX, int AY, Cell B);

	Scene() {}

public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()
