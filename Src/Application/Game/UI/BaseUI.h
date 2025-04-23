#pragma once

class BaseUI
{
public:
	BaseUI(int i);
	~BaseUI() {};

	virtual void Init() = 0;
	void Update();
	virtual void Draw() = 0;
	virtual void Release() = 0;


protected:

	KdTexture* UITex;
	Math::Rectangle rect;

	Math::Matrix Scale;
	Math::Matrix Trans;
	Math::Matrix Mat;

	void MatSet();

	Math::Vector2 Main;
	Math::Vector2 Pos;
	Math::Vector2 Move;
	Math::Vector2 PScale;


};
