#pragma once
class Knight;
class Texture;
enum class GameState;
class SoundStream;
class SoundEffect;

class HUD final
{
public:
	explicit HUD(const Point2f& window);
	~HUD();
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD(HUD&& other) noexcept;
	HUD& operator=(HUD&& other) noexcept;

	void Draw(const Point2f& window, int health, int gold) const;
	void Update(float elapsedSec, Knight& knight);
	void SetHudState(bool& isPlaying);
	void DetectClickOnButtons(const Point2f& mousePos);
	void DetectClickOnSlider(const Point2f& mousePos);
	void DetectMouseDown(bool isMouseDown);
	int GetVolume() const;

private:
	const Texture* m_pHealthTexture, * m_pGoldTexture,
		* m_pTextHealth, * m_pTextGold,
		*m_pPauseMenuTxt, *m_pResumeTxt, *m_pOptionsTxt,
		*m_pOptionsMenuTxt, *m_pBackTxt, *m_pSoundVolumeTxt;
	Rectf m_MenuShape, m_ResumeShape, m_OptionsShape, 
		m_BackShape, m_SoundVolumeShape;
	Rectf m_Shape;
	Color4f m_DeathScreenColor;
	bool m_KnightIsDying;
	enum class HudState{playing, pause, options, resume};
	HudState m_HudState;

	Rectf m_SliderDragger;
	int m_Volume;
	bool m_IsMouseDown;
	Rectf m_Slider;

	void DrawUI(const Point2f& window, int health, int gold) const;
	void DrawHealth(const Point2f& window, int health) const;
	void DrawPauseScreen(const Point2f& window) const;
	void DrawOptionsScreen(const Point2f& window) const;
};

