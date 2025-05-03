#pragma once

class Num
{
public:
	Num();
	~Num();

	KdTexture* GetTex() { return &NumTex; };
	KdTexture* GetSTex() { return &NumSTex; };
	static Num& GetInst()
	{
		static Num instance;
		return instance;
	}
private:
	KdTexture NumTex;
	KdTexture NumSTex;

};

inline Num::Num()
{
	NumTex.Load("Texture/others/Num.png");
	NumSTex.Load("Texture/UI/ScoreNum.png");
}

inline Num::~Num()
{
	NumTex.Release();

}

#define NUMTEX Num::GetInst()
