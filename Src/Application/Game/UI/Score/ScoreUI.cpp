#include "ScoreUI.h"
#include "../../NumDraw.h"
#include "../../Game.h"
#include "../UITex.h"


void ScoreUI::Init()
{
	Pos = { -100,320 };	
	UITex = UITEX.GetTex(1);
	Num = new NumDraw( { 20 , 315 },PLAYER.Score);
}

void ScoreUI::Update()
{
	Num->Cnt++;
	Num->Dmg = PLAYER.Score;
	PScale = { 1,0.8};
	MatSet();
}

void ScoreUI::Draw()
{
	rect = { 0,0,203,77 };
	SHADER.m_spriteShader.SetMatrix(Mat);
	SHADER.m_spriteShader.DrawTex(UITex, rect);

	Num->Draw(0, true);
}

void ScoreUI::Release()
{
}
