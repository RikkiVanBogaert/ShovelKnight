#pragma once
#include <vector>

class Texture;
class Level;
class Knight;
class Camera;
class Enemy;
class Ladder;
class Item;
class Dragon;
class ObjectManager;
class HUD;
class SoundStream;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Level* m_pLevel;
	Knight* m_pKnight;
	Camera* m_pCamera;
	Ladder* m_pLadder;
	std::vector <Item*> m_pItems;
	std::vector <Enemy*> m_pEnemies;

	ObjectManager* m_pObjectManager;
	HUD* m_pHUD;

	bool m_HasKnightAttacked;
	float m_TimerOutOfBounds;
	Texture* m_pWinScreenTexture;
	int m_Gold;

	bool m_IsPlaying;

	SoundStream* m_pBackGroundMusic;
	bool m_IsMouseDown;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void HandleLevelCollision();
	void HandleLadders();
	void SetActorsOnGround();

	void EnemyUpdate(float elapsedSec);

	void DrawEnemies() const;
	void UpdateEnemies(float elapsedSec, int j);
	void HandleHitDetectionEnemies(int j);
	void HandleLevelCollisionEnemies(int j);
	void DeleteGoneEnemies(int j);

	void ItemUpdate(float elapsedSec);

	void DrawItems() const;
	void UpdateItems(float elapsedSec, int i);
	void ItemPlayerCollision(float elapsedSec, int i);
	void SolidItemCollision(int i);
	void ItemLevelCollision(int i);
	void DeleteGoneItems(int i);

	void DetectOutOfBounds(float elapsedSec);
	void DeleteRemainingObjects();
	void DrawEndScreen() const;
	void UpdateSounds();

	void PrintInfo();
};



