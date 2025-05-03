#include "BaseUI.h"

void BaseUI::MatSet()
{
	Trans = Math::Matrix::CreateTranslation(Pos.x, Pos.y, 0);
	Scale = Math::Matrix::CreateScale(PScale.x, PScale.y, 0);


	Mat = Scale * Trans;
}
