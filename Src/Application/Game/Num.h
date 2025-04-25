#pragma once

class Num
{
public:
	Num();
	~Num();

	KdTexture* GetTex() { return &NumTex; };
	static Num& GetInst()
	{
		static Num instance;
		return instance;
	}
private:
	KdTexture NumTex;

};

inline Num::Num()
{
	NumTex.Load("Texture/others/Num.png");
}

inline Num::~Num()
{
	NumTex.Release();

}

#define NUMTEX Num::GetInst()
