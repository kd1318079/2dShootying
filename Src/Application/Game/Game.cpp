#include "Game.h"

void MainGame::Init()
{
	PLAYER.Init();
	MobEnemy.push_back(new Enemy(0));
}

void MainGame::Update()
{
	PLAYER.Update();
	for (auto it = MobEnemy.begin(); it != MobEnemy.end(); )
	{
		if ((*it)->Death) {
			delete* it;
			*it = nullptr;
			it = MobEnemy.erase(it);
		}
		else {
			(*it)->Update();
			++it;
		}
	}
	for (auto it = BossEnemy.begin(); it != BossEnemy.end(); ) {
		if ((*it)->Death) {
			delete* it;
			*it = nullptr;
			it = BossEnemy.erase(it); // eraseは削除後の次のイテレータを返す
		}
		else {
			(*it)->Update();
			++it; // 次の要素へ移動
		}
	}

	if (MobEnemy.empty())
	{
		if (MobEnemy.size() == 0)
		{
			for (int i = 0; i < 200; i++)
				MobEnemy.push_back(new Enemy(0));
		}
	}



}

void MainGame::Draw()
{
	for (auto A : MobEnemy) A->Draw();
	for (auto A : BossEnemy) A->Draw();
	PLAYER.Draw();
}

void MainGame::Release()
{
	PLAYER.Release();

}
