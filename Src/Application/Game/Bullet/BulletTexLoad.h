#pragma once

class BulletLoad
{
public:
	BulletLoad();
	~BulletLoad();
	
	KdTexture* GetTex(int A) { return &BuTex[A]; };
	KdTexture* GetLaserTex() { return &LaesrTex; };

	KdTexture* GetETex(int A) { return &EBuTex[A]; };
	static BulletLoad& GetInstance()
	{
		static BulletLoad instance;
		return instance;
	}
private:
	static const int BMax = 22 + 1;
	KdTexture BuTex[BMax];
	KdTexture LaesrTex;
	
	static const int EBMax = 22 + 1;
	KdTexture EBuTex[EBMax];

	
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
	
	for (int i = 0; i < EBMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/Bullet/EBullet1.png");
		EBuTex[i].Load(str);//scene.h‚É‚à‘‚¢‚½’Ê‚èƒLƒƒƒ‰–¼‚Æ“Y‚¦Žš‚ÅƒLƒƒƒ‰•Ê‚ÌTexture‚ð‚Â‚­‚é
	}
}

inline BulletLoad::~BulletLoad()
{
	for (int i = 0; i < BMax; i++)
	{
		BuTex[i].Release();
	}
	LaesrTex.Release();
	for (int i = 0; i < EBMax; i++)
	{
		EBuTex[i].Release();
	}
}

#define BuTEX BulletLoad::GetInstance()
