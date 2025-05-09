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
		EnemyTex[i].Load(str);//scene.hにも書いた通りキャラ名と添え字でキャラ別のTextureをつくる
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
