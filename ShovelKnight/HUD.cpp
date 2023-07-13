#pragma once

#include "pch.h"
#include "HUD.h"
#include "Knight.h"
#include "Texture.h"

HUD::HUD(const Point2f& window):
	m_KnightIsDying{},
	m_DeathScreenColor{Color4f(0,0,0,0)},
	m_pHealthTexture{ new Texture{"Resources/Images/HUD/HealthIcon.png"} },
	m_pGoldTexture{ new Texture{"Resources/Images/HUD/GoldIcon.png"} },
	m_pTextHealth{new Texture{"-Health-", "Resources/Images/HUD/RetroFont.ttf", 30, Color4f(1, 1, 1, 1)}},
	m_pTextGold{ new Texture{"-Gold-", "Resources/Images/HUD/RetroFont.ttf", 30, Color4f(1, 1, 1, 1)} },
	m_pPauseMenuTxt{new Texture{"Resources/Images/HUD/PauseMenu.png"}},
	m_pResumeTxt{ new Texture{"Resources/Images/HUD/Resume.jpg"} },
	m_pOptionsTxt{ new Texture{"Resources/Images/HUD/Options.jpg"} },
	m_pOptionsMenuTxt{ new Texture{"Resources/Images/HUD/OptionsMenu.png"} },
	m_pBackTxt{ new Texture{"Resources/Images/HUD/Back.jpg"} },
	m_pSoundVolumeTxt{ new Texture{"Resources/Images/HUD/SoundVolume.jpg"} },
	m_HudState{HudState::playing},
	m_Volume{10},
	m_IsMouseDown{false}
{
	float menuScale{ 1.5f };
	m_MenuShape.width = m_pPauseMenuTxt->GetWidth() * menuScale;
	m_MenuShape.height = m_pPauseMenuTxt->GetHeight() * menuScale;
	m_MenuShape.left = window.x / 2 - m_MenuShape.width / 2;
	m_MenuShape.bottom = window.y / 2 - m_MenuShape.height / 2;

	//buttons
	m_ResumeShape.width = m_pResumeTxt->GetWidth();
	m_ResumeShape.height = m_pResumeTxt->GetHeight();
	m_ResumeShape.left = m_MenuShape.left + 35;
	m_ResumeShape.bottom = m_MenuShape.bottom + m_MenuShape.height - 130;

	m_OptionsShape.width = m_pOptionsTxt->GetWidth();
	m_OptionsShape.height = m_pOptionsTxt->GetHeight();
	m_OptionsShape.left = m_MenuShape.left + 35;
	m_OptionsShape.bottom = m_ResumeShape.bottom - m_ResumeShape.height - 30;

	m_BackShape = m_ResumeShape;
	m_BackShape.width = m_pBackTxt->GetWidth();

	m_SoundVolumeShape = m_OptionsShape;
	m_SoundVolumeShape.width = m_pSoundVolumeTxt->GetWidth();


	m_Slider.left = m_SoundVolumeShape.left + m_SoundVolumeShape.width + 150;
	m_Slider.bottom = m_SoundVolumeShape.bottom + 5;
	m_Slider.width = 200;
	m_Slider.height = 4;

	m_SliderDragger.width = 5;
	m_SliderDragger.height = 20;
	m_SliderDragger.left = m_Slider.left + m_Slider.width / 2 - m_SliderDragger.width / 2;
	m_SliderDragger.bottom = m_Slider.bottom - m_SliderDragger.height / 2;
}

HUD::~HUD()
{
	delete m_pHealthTexture;
	delete m_pGoldTexture;
	delete m_pTextHealth;
	delete m_pTextGold;
	delete m_pPauseMenuTxt;
	delete m_pResumeTxt;
	delete m_pOptionsTxt;
	delete m_pOptionsMenuTxt;
	delete m_pBackTxt;
	delete m_pSoundVolumeTxt;
}


void HUD::Draw(const Point2f& window, int health, int gold) const
{
	DrawUI(window, health, gold);

	if (m_HudState == HudState::pause)
	{
		DrawPauseScreen(window);
	}
	else if (m_HudState == HudState::options)
	{
		DrawOptionsScreen(window);
	}

	if (m_KnightIsDying)
	{
		utils::SetColor(m_DeathScreenColor);
		utils::FillRect(0, 0, window.x, window.y);
	}

}

void HUD::DrawUI(const Point2f& window, int health, int gold) const
{
	//black rect
	const float height{ 0.085f * window.y };
	utils::SetColor(Color4f(0, 0, 0, 1));
	utils::FillRect(Rectf(0, window.y - height, window.x, height));

	m_pTextHealth->Draw(Point2f(0, window.y - height / 2));
	DrawHealth(window, health);

	m_pTextGold->Draw(Point2f(300, window.y - height / 2));
	const float scale{ 3.3f };

	Rectf destRect{ 300, window.y - height + 5,
		m_pGoldTexture->GetWidth() * scale, m_pGoldTexture->GetHeight() * scale };
	std::string amountGold{ "-" + std::to_string(gold) + "-" };
	Texture textAmountGold{ amountGold, "Resources/Images/HUD/RetroFont.ttf", 30, Color4f(1, 1, 1, 1) };
	m_pGoldTexture->Draw(destRect);
	destRect.left += destRect.width + 10;
	textAmountGold.Draw(Point2f(destRect.left, destRect.bottom - 10));
}

void HUD::DrawHealth(const Point2f& window, int health) const
{
	const float height{ 0.085f * window.y };
	//health
	Rectf srcRect{ 0, m_pHealthTexture->GetHeight(),
		m_pHealthTexture->GetWidth() / 3, m_pHealthTexture->GetHeight() };
	const float scale{ 3 };
	const float offset{ 2 };
	Rectf destRect{ offset, window.y - height + offset, 
		srcRect.width * scale, srcRect.height * scale };
	// 1
	if (health <= 0)
	{
		srcRect.left = srcRect.width * 2;
	}
	else if (health == 1)
	{
		srcRect.left = srcRect.width;
	}
	else
	{
		srcRect.left = 0;
	}
	m_pHealthTexture->Draw(destRect, srcRect);
	// 2
	if (health <= 2)
	{
		srcRect.left = srcRect.width * 2;
	}
	else if (health == 3)
	{
		srcRect.left = srcRect.width;
	}
	else
	{
		srcRect.left = 0;
	}
	destRect.left += srcRect.width * scale + offset;
	m_pHealthTexture->Draw(destRect, srcRect);
	// 3
	if (health <= 4)
	{
		srcRect.left = srcRect.width * 2;
	}
	else if (health == 5)
	{
		srcRect.left = srcRect.width;
	}
	else
	{
		srcRect.left = 0;
	}
	destRect.left += srcRect.width * scale + offset;
	m_pHealthTexture->Draw(destRect, srcRect);
	// 4
	if (health <= 6)
	{
		srcRect.left = srcRect.width * 2;
	}
	else if (health == 7)
	{
		srcRect.left = srcRect.width;
	}
	else
	{
		srcRect.left = 0;
	}
	destRect.left += srcRect.width * scale + offset;
	m_pHealthTexture->Draw(destRect, srcRect);
}

void HUD::DrawPauseScreen(const Point2f& window) const
{
	m_pPauseMenuTxt->Draw(m_MenuShape);
	m_pResumeTxt->Draw(m_ResumeShape);
	m_pOptionsTxt->Draw(m_OptionsShape);
}

void HUD::DrawOptionsScreen(const Point2f& window) const
{
	m_pOptionsMenuTxt->Draw(m_MenuShape);
	m_pBackTxt->Draw(m_BackShape);
	m_pSoundVolumeTxt->Draw(m_SoundVolumeShape);

	utils::SetColor(Color4f(1, 1, 1, 1));
	utils::FillRect(m_Slider);
	utils::FillRect(m_SliderDragger);
}

void HUD::Update(float elapsedSec, Knight& knight)
{
	if (knight.IsDead())
	{
		m_KnightIsDying = true;
		m_DeathScreenColor.a += 0.7f * elapsedSec;
	}
	else
	{
		m_KnightIsDying = false;
	}

	if (m_DeathScreenColor.a >= 1 && m_KnightIsDying)
	{
		m_KnightIsDying = false;
		m_DeathScreenColor.a = 0;
		knight.Respawn();
	}
}

int HUD::GetVolume() const
{
	float volume{}; //a + t(b-a)
	volume = float(std::lerp(0, 100, (m_SliderDragger.left + m_SliderDragger.width / 2) / m_Slider.width ) - 390);
	return int(volume);
}

void HUD::DetectClickOnButtons(const Point2f& mousePos)
{
	if (utils::IsPointInRect(mousePos, m_ResumeShape) &&
		m_HudState == HudState::pause)
	{
		m_HudState = HudState::resume;
	}
	else if(utils::IsPointInRect(mousePos, m_OptionsShape) &&
		m_HudState == HudState::pause)
	{
		m_HudState = HudState::options;
	}
	else if (utils::IsPointInRect(mousePos, m_BackShape) &&
		m_HudState == HudState::options)
	{
		m_HudState = HudState::pause;
	}
}

void HUD::DetectClickOnSlider(const Point2f& mousePos)
{
	if (m_IsMouseDown)
	{
		if (utils::IsPointInRect(mousePos,Rectf(m_SliderDragger.left - m_SliderDragger.width * 5, m_SliderDragger.bottom, 
			m_SliderDragger.width * 10, m_SliderDragger.height)))
		{
			m_SliderDragger.left = mousePos.x - m_SliderDragger.width / 2;
		}
	}
	float center{ m_SliderDragger.left + m_SliderDragger.width / 2 };
	if (center < m_Slider.left)
	{
		m_SliderDragger.left = m_Slider.left - m_SliderDragger.width / 2;
	}
	else if (center > m_Slider.left + m_Slider.width)
	{
		m_SliderDragger.left = m_Slider.left + m_Slider.width - m_SliderDragger.width / 2;
	}
}

void HUD::DetectMouseDown(bool isMouseDown)
{
	m_IsMouseDown = isMouseDown;
}

void HUD::SetHudState(bool& isPlaying)
{
	if (m_HudState == HudState::playing && !isPlaying)
		m_HudState = HudState::pause;
	else if (isPlaying) m_HudState = HudState::playing;

	if (m_HudState == HudState::resume) isPlaying = true;
}
