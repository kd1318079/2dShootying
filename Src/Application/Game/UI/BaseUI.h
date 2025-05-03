#pragma once

class BaseUI
{
public:
	BaseUI() {};
	~BaseUI() {};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;


protected:

	KdTexture* UITex;
	Math::Rectangle rect;

	Math::Matrix Scale;
	Math::Matrix Trans;
	Math::Matrix Mat;


	Math::Vector2 Pos = {0,0};
	Math::Vector2 Move = {0,0};
	Math::Vector2 PScale = {1,1};

	void MatSet();
};
