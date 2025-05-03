#pragma once
class BaseUI;

class UI
{
public:
	UI() {};
	~UI() {};

	void Init();
	void Draw();
	void Update();
	void Release();

private:
	std::vector<std::shared_ptr<BaseUI>> m_UI;

};