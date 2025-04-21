#pragma once

class BulletLoad
{
public:
	BulletLoad();
	~BulletLoad();
	
	KdTexture* GetTex(int A) { return &BuTex[A]; };
	KdTexture* GetLaserTex() { return &LaesrTex; };
	static BulletLoad& GetInstance()
	{
		static BulletLoad instance;
		return instance;
	}
private:
	static const int BMax = 22 + 1;
	KdTexture BuTex[BMax];
	KdTexture LaesrTex;
	
};

inline BulletLoad::BulletLoad()
{
	for (int i = 0; i < BMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/Bullet/BasicBlock1.png");
		BuTex[i].Load(str);//scene.h‚É‚à‘‚¢‚½’Ê‚èƒLƒƒƒ‰–¼‚Æ“Y‚¦Žš‚ÅƒLƒƒƒ‰•Ê‚ÌTexture‚ð‚Â‚­‚é
	}
	LaesrTex.Load("Texture/Bullet/laser.png");
}

inline BulletLoad::~BulletLoad()
{
	for (int i = 0; i < BMax; i++)
	{
		BuTex[i].Release();
	}
	LaesrTex.Release();
}

#define BuTEX BulletLoad::GetInstance()
