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
	BlTex[0].Load("Texture/others/Player.png");
	BlTex[1].Load("Texture/others/Aime.png");
	BlTex[2].Load("Texture/others/Block.png");
}

inline BlockLoad::~BlockLoad()
{
	for (int i = 0; i < BMax; i++)
	{
		BlTex[i].Release();
	}

}

#define BlTEX BlockLoad::GetInstance()
