#pragma once

class BlockLoad
{
public:
	BlockLoad();
	~BlockLoad();

	KdTexture* GetTex(int A) { return &BlTex[A]; };
	static BlockLoad& GetInstance()
	{
		static BlockLoad instance;
		return instance;
	}
private:
	static const int BMax = 30 + 1;
	KdTexture BlTex[BMax];

};

inline BlockLoad::BlockLoad()
{
	BlTex[0].Load("Texture/Player.png");
	BlTex[1].Load("Texture/Aime.png");
	//for (int i = 0; i < BMax; i++)
	//{
	//	char str[100];
	//	sprintf_s(str, sizeof(str), "Texture/Parts/BasicBlock%d.png", i);
	//	BlTex[i].Load(str);//scene.h‚É‚à‘‚¢‚½’Ê‚èƒLƒƒƒ‰–¼‚Æ“Y‚¦Žš‚ÅƒLƒƒƒ‰•Ê‚ÌTexture‚ð‚Â‚­‚é
	//}
	//BlTex[BMax - 1].Load("Texture/Parts/WaponBase.png");
}

inline BlockLoad::~BlockLoad()
{
	for (int i = 0; i < BMax; i++)
	{
		BlTex[i].Release();
	}

}

#define BlTEX BlockLoad::GetInstance()
