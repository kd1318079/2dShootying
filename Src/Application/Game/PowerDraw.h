#pragma once
#include "UI/UITex.h"
class PowerDraw
{
public:
	PowerDraw();
	~PowerDraw();
	void Draw(int i, Math::Rectangle rect);
	static PowerDraw& GetInstance()
	{
		static PowerDraw instance;
		return instance;
	}
private:
	static const int PowerMax = 69 + 1;
	KdTexture* PowerTex[PowerMax];

};

PowerDraw::PowerDraw()
{
	for (int i = 0; i < PowerMax; i++)
	{
		PowerTex[i] = UITEX.GetPowerTex(i);
	}
}

PowerDraw::~PowerDraw()
{

}

inline void PowerDraw::Draw(int i,Math::Rectangle rect)
{
	SHADER.m_spriteShader.DrawTex(PowerTex[i], rect);

}

#define POWERTEX PowerDraw::GetInstance()
