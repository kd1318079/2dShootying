#pragma once

class UITex
{
public:
	UITex();
	~UITex();
	KdTexture* GetTex(int A) { return &UTex[A]; };
	KdTexture* GetPowerTex(int A) { return &PowerTex[A]; };
	static UITex& GetInstance()
	{
		static UITex instance;
		return instance;
	}
private:
	static const int UIMax = 33 + 1;
	static const int PowerMax = 69+1;
	KdTexture UTex[UIMax];
	KdTexture PowerTex[PowerMax];

};

#define UITEX UITex::GetInstance()
