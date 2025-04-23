#include "BaseUI.h"
#include "UITex.h"

BaseUI::BaseUI(int i)
{
	UITex = UITEX.GetTex(i);
	Pos = { 0,0 };
}

void BaseUI::Update()
{

}

void BaseUI::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Main.x, Main.y, 0);
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);

	Mat = Scale * Trans;
}
