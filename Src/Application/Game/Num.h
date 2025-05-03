#pragma once

class Num
{
public:
	Num();
	~Num();

	KdTexture* GetTex() { return &NumTex; };
	KdTexture* GetSTex() { return &NumSTex; };
	KdTexture* GetSteTex() { return &NumSteTex; };
	static Num& GetInst()
	{
		static Num instance;
		return instance;
	}
private:
	KdTexture NumTex;
	KdTexture NumSTex;
	KdTexture NumSteTex;

};

inline Num::Num()
{
	NumTex.Load("Texture/others/Num.png");
	NumSTex.Load("Texture/UI/ScoreNum.png");
	NumSteTex.Load("Texture/UI/StetasNum.png");
}

inline Num::~Num()
{
	NumTex.Release();
	NumSTex.Release();
	NumSteTex.Release();

}

#define NUMTEX Num::GetInst()
