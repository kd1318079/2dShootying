#pragma once
#include "../BaseUI.h"

class NumDraw;

class ScoreUI :public BaseUI
{
public:

	ScoreUI() {}
	~ScoreUI() {}

	void Init() override;
	void Update()override;
	void Draw()override;
	void Release()override;

	NumDraw* Num;
private:

};
