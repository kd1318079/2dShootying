#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	GM.Draw();
}

void Scene::Update()
{
	GM.Update();
}

void Scene::Init()
{
	//CreateConsole();
	GM.Init();
}

void Scene::Release()
{
	GM.Release();
	//DestroyConsole();
}

void Scene::ImGuiUpdate()
{
	//return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);
	static int A = 0;
	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
		ImGui::Text("DisCnt : %d", PLAYER.ECnt);
		ImGui::Text("PlayerSpd : %f", PLAYER.GetSpd());
		ImGui::Text("PlayerSpd : %f", PLAYER.GetPDeg());
		ImGui::Text("ContiCnt : %d", PLAYER.ContiCnt);
		if(GM.AllEnemy.size() == 1)ImGui::Text("Enmey HP : %d", GM.AllEnemy[0]->HP);
	}
	ImGui::End();
}

bool Scene::CellHit(Cell A, Cell B)
{
	if (HitCom(A.Cell1X, A.Cell1Y, B)) return true;
	if (HitCom(A.Cell2X, A.Cell2Y, B)) return true;
	if (HitCom(A.Cell3X, A.Cell3Y, B)) return true;
	if (HitCom(A.Cell4X, A.Cell4Y, B)) return true;

	return false;
}

bool Scene::HitCom(int AX, int AY, Cell B)
{
	if (AX == B.Cell1X && AY == B.Cell1Y)return true;
	if (AX == B.Cell2X && AY == B.Cell2Y)return true;
	if (AX == B.Cell3X && AY == B.Cell3Y)return true;
	if (AX == B.Cell4X && AY == B.Cell4Y)return true;

	return false;
}

bool Scene::HitJudge(Math::Vector2 A, Math::Vector2 B, int size)
{
	float a = A.x - B.x;
	float b = A.y - B.y;
	float c = sqrt(a * a + b * b);
	if (c < size)
	{
		return true;
	}
	return false;
}

void Scene::CreateConsole()
{
	AllocConsole();//コンソールを開く
	freopen_s(&fp_c, "CONOUT$", "w", stdout);
}

void Scene::DestroyConsole()
{
	fclose(fp_c);
	FreeConsole();
	//コンソールを閉じる

}