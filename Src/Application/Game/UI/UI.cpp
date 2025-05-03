#include "UI.h"
#include "Score/ScoreUI.h"

void UI::Init()
{
	//ƒXƒRƒAUI‚ğì‚é
	std::shared_ptr<ScoreUI> m_SUI = std::make_shared<ScoreUI>();
	m_SUI->Init();

	
	
	
	//ì‚Á‚½•¨‚ğ’Ç‰Á‚µ‚Ä‚¢‚­
	m_UI.push_back(m_SUI);


}

void UI::Draw()
{
	for (auto A : m_UI) (*A).Draw();
}

void UI::Update()
{
	for (auto A : m_UI) (*A).Update();
}

void UI::Release()
{

}
