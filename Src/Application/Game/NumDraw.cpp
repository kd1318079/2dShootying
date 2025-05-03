#include "NumDraw.h"
#include "../Game/Game.h"

NumDraw::NumDraw(Math::Vector2 A, int B, const int Timer)
{
	Pos = A;
	Dmg = B;
	Cnt = Timer;
	NumTex = NUMTEX.GetTex();
	ScoreTex = NUMTEX.GetSTex();
	StetasTex = NUMTEX.GetSteTex();
}



void NumDraw::Draw(int ESize, const bool A, const bool C)
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
	if(GM.GetScene() == Game)if(!A) Pos.y += Move * 2.3 / 64 * ESize;

	for (int j = 0; j < digCnt; j++)
	{
		int digCnt = 0; // Dmg‚ÌŒ…”‚ðŒvŽZ
		int num = Dmg;
		do {
			digCnt++;
			num /= 10;
		} while (num != 0);


		Math::Rectangle rect;
		for (int j = 0; j < digCnt; j++) {
			int dig = std::pow(10, digCnt - j - 1); // ³‚µ‚¢Šî”‚ðŒvŽZ
			int digit = (Dmg / dig) % 10; // Œ…‚ðŽæ“¾

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
			if (A)
			{
				float size = 0.4;
				B = 102 * size * j;
				rect = { 120 * digit,0,120,120 };
				SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(size) * Math::Matrix::CreateTranslation(Pos.x + B, Pos.y, 0));
				SHADER.m_spriteShader.DrawTex(ScoreTex, rect, 1);
			}
			else if (C)
			{
				float size = 0.4;
				B = 102 * size * j;
				rect = { 120 * digit,0,120,120 };
				SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(size) * Math::Matrix::CreateTranslation(Pos.x + B, Pos.y, 0));
				SHADER.m_spriteShader.DrawTex(StetasTex, rect, 1);
			}
			else 
			{
				rect = { 120 * digit,0,120,120 };
				SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(0.1) * Math::Matrix::CreateTranslation(Pos.x - B, Pos.y, 0));
				SHADER.m_spriteShader.DrawTex(NumTex, rect, 1);
			}
		}
	}
}
