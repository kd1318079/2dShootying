#pragma once

class EnrmyLoad
{
public:
	EnrmyLoad();
	~EnrmyLoad();

	KdTexture* GetTex(int A) { return &EnemyTex[A]; };
	static EnrmyLoad& GetInstance()
	{
		static EnrmyLoad instance;
		return instance;
	}
private:
	static const int EMax = 2 + 1;
	KdTexture EnemyTex[EMax];

};

inline EnrmyLoad::EnrmyLoad()
{
	for (int i = 0; i < EMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/Enemy/enemy.png");
		EnemyTex[i].Load(str);//scene.h‚É‚à‘‚¢‚½’Ê‚èƒLƒƒƒ‰–¼‚Æ“Y‚¦Žš‚ÅƒLƒƒƒ‰•Ê‚ÌTexture‚ð‚Â‚­‚é
	}
}

inline EnrmyLoad::~EnrmyLoad()
{
	for (int i = 0; i < EMax; i++)
	{
		EnemyTex[i].Release();
	}

}

#define ENEMYTEX EnrmyLoad::GetInstance()
