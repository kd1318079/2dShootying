#pragma once

class Explosion
{
public:
	Explosion(const int A = 30, const float size = 1, const int ExpType = 0);
	~Explosion() {};

	void Draw();
	void Update();
	
	//���ŃJ�E���g
	int DeleteCnt = 0;
	bool GravityHole = false;
	int EType = 0;
	int RectSize = 0;
	int Size = 0;
private:
	int ExpCnt = 0;
	//���ł܂ł̃t���[��(�Œ�)
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
public:
	Math::Vector2 GetPos() { return Pos; };
	int  GetRectSize() { return RectSize; };
	int  GetSize() { return Size; };
};