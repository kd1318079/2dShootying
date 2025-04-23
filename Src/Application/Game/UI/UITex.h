#pragma once

class UITex
{
public:
	UITex();
	~UITex();
	KdTexture* GetTex(int A) { return &UTex[A]; };
	static UITex& GetInstance()
	{
		static UITex instance;
		return instance;
	}
private:
	static const int UIMax = 33 + 1;
	KdTexture UTex[UIMax];

};

UITex::UITex()
{
	for (int i = 0; i < UIMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/UI/UI%d.png", i);
		UTex[i].Load(str);//scene.h‚É‚à‘‚¢‚½’Ê‚èƒLƒƒƒ‰–¼‚Æ“Y‚¦Žš‚ÅƒLƒƒƒ‰•Ê‚ÌTexture‚ð‚Â‚­‚é
	}
}

UITex::~UITex()
{
	for (int i = 0; i < UIMax; i++)
	{
		UTex[i].Release();
	}
}

#define UITEX UITex::GetInstance()
