#include "Explosion.h"
#include "ExpTexLoad.h"
#include "../../Game/Game.h"

Explosion::Explosion(const int A, const float size,const int ExpType)
{
	ETex = ExpTEX.GetTex(ExpType);
	Pos = PLAYER.ExpPos;
	DeleteCnt = A;
	ExpDelCnt = A;
	PScale = { size,size };
	Size = size;
	EType = ExpType;
	Main = Pos - PLAYER.GetScroll();
	MatSet();
}

void Explosion::Draw()
{
	D3D.SetBlendState(BlendMode::Add);
	if(EType == 0)rect = ExpRect(10);
	if(EType == 1)rect = ExpRect(60);
	if (EType == 2)
	{
		D3D.SetBlendState(BlendMode::Alpha);
		rect = ExpRect(8);
	}
	if (EType == 3)rect = ExpRect(6);
	if (EType == 4)
	{
		D3D.SetBlendState(BlendMode::Alpha);
		rect = ExpRect(16);
	}
	SHADER.m_spriteShader.SetMatrix(Mat);
	SHADER.m_spriteShader.DrawTex(ETex, rect);
	D3D.SetBlendState(BlendMode::Alpha);
}

void Explosion::Update()
{
	DeleteCnt--;
	Main = Pos - PLAYER.GetScroll();
	MatSet();
}

Math::Rectangle Explosion::ExpRect(int B)
{
	Math::Rectangle A;
	int AA = ExpDelCnt / B;
	int AAA = ExpDelCnt - DeleteCnt;
	if (AAA == AA * B)AAA -= 1;

	if (EType == 0 || EType == 2) RectSize = 32;
	if (EType == 1) RectSize = 100;
	if (EType == 3) RectSize = 32;
	if (EType == 4) RectSize = 64;

	A = { AAA / AA * RectSize ,0 ,RectSize, RectSize };

	return A;
}

void Explosion::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Main.x, Main.y, 0);
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Scale * Trans;
}
