#include "UITex.h"

UITex::UITex()
{
	for (int i = 0; i < UIMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/UI/UI%d.png", i);
		UTex[i].Load(str);//scene.h�ɂ��������ʂ�L�������ƓY�����ŃL�����ʂ�Texture������
	}
	for (int i = 0; i < PowerMax; i++)
	{
		char str[100];
		sprintf_s(str, sizeof(str), "Texture/UI/Power%d.png", i);
		PowerTex[i].Load(str);//scene.h�ɂ��������ʂ�L�������ƓY�����ŃL�����ʂ�Texture������
	}


}

UITex::~UITex()
{
	for (int i = 0; i < UIMax; i++)
	{
		UTex[i].Release();
	}
	for (int i = 0; i < PowerMax; i++)
	{
		PowerTex[i].Release();
	}
}
