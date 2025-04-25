#pragma once
#include "Num.h"

class NumDraw
{
public:
	NumDraw(Math::Vector2 A ,int B);
	~NumDraw() {};

	Math::Vector2 Pos;
	int Dmg = 0;
	int Cnt = 0;
	void Draw(int ESize);
	
private:
	KdTexture* NumTex;

};

inline NumDraw::NumDraw(Math::Vector2 A,int B)
{
	Pos = A; 
	Dmg = B;
	Cnt = 60;
	NumTex = NUMTEX.GetTex();
}

inline void NumDraw::Draw(int ESize)
{
	int digCnt = 0;
	int num = Dmg;
	while (num != 0)
	{
		digCnt++;
		num /= 10;
	}

	Math::Rectangle rect;


	float Count = Cnt * 6;
	float Move = 0;

	if (Cnt > 25)  Move = sin(ToRadians(Count));
	Pos.y += Move * 2.3 / 64 * ESize;

	for (int j = 0; j < digCnt; j++)
	{
		int digCnt = 0; // Dmgの桁数を計算
		int num = Dmg;
		do {
			digCnt++;
			num /= 10;
		} while (num != 0);

		

		Math::Rectangle rect;
		for (int j = 0; j < digCnt; j++) {
			int dig = std::pow(10, digCnt - j - 1); // 正しい基数を計算
			int digit = (Dmg / dig) % 10; // 桁を取得

			float B;
			switch (digCnt)
			{
			case 1:
				B = 0;
				break;
			case 2:
				B = 6 - 12 * j;
				break;
			case 3:
				B = 12 - 12 * j;
				break;
			default:
				break;
			}

			//デバッグ用
			if (Cnt == 30)
			{
				int A = 0;
			}

			rect = { 120 * digit,0,120,120 };
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(0.1) * Math::Matrix::CreateTranslation(Pos.x - B, Pos.y, 0));
			SHADER.m_spriteShader.DrawTex(NumTex, rect, 1);
		}
	}
}
