#pragma once

class ExpLoad
{
public:
	ExpLoad();
	~ExpLoad();

	KdTexture* GetTex(int A) { return &ExTex[A]; };
	static ExpLoad& GetInstance()
	{
		static ExpLoad instance;
		return instance;
	}
private:
	static const int EMax = 30 + 1;
	KdTexture ExTex[EMax];

};

inline ExpLoad::ExpLoad()
{
	for (int i = 0; i < EMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/Explosion/Exp%d.png", i);
		ExTex[i].Load(str);//scene.h‚É‚à‘‚¢‚½’Ê‚èƒLƒƒƒ‰–¼‚Æ“Y‚¦Žš‚ÅƒLƒƒƒ‰•Ê‚ÌTexture‚ð‚Â‚­‚é
	}
}

inline ExpLoad::~ExpLoad()
{
	for (int i = 0; i < EMax; i++)
	{
		ExTex[i].Release();
	}

}

#define ExpTEX ExpLoad::GetInstance()
