#pragma once

class Explosion
{
public:
	Explosion(const int A = 30, const float size = 1, const int ExpType = 0);
	~Explosion() {};

	void Draw();
	void Update();
	
	int DeleteCnt = 0;
	bool GravityHole = false;
	int EType = 0;
private:
	int ExpCnt = 0;
	int ExpDelCnt = 0;
	Math::Rectangle ExpRect(int B);

	Math::Vector2 Pos;
	Math::Vector2 Main;
	Math::Vector2 PScale = {1,1};

	KdTexture* ETex;
	
	Math::Rectangle rect;

	Math::Matrix Scale;
	Math::Matrix Trans;
	Math::Matrix Mat;

	void MatSet();
};