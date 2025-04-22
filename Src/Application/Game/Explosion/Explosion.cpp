#include "Explosion.h"
#include "ExpTexLoad.h"
#include "../../Game/Game.h"

Explosion::Explosion(const int A, const float size,const int ExpType)
{
	ETex = ExpTEX.GetTex(0);
	Pos = PLAYER.ExpPos;
	DeleteCnt = A;
	ExpDelCnt = A;
	PScale = { size,size };
	EType = ExpType;
}



void Explosion::Draw()
{
	D3D.SetBlendState(BlendMode::Add);
	rect = ExpRect(10);
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
	A = { AAA / AA * 32 ,0 ,32, 32 };

	return A;
}

void Explosion::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Main.x, Main.y, 0);
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Scale * Trans;
}
