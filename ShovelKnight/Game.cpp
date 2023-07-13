#pragma once
#include "pch.h"
#include "Game.h"
#include "Level.h"
#include "Knight.h"
#include "Camera.h"
#include "Ladder.h"
#include "Waterfall.h"
#include "Enemy.h"
#include "Beeto.h"
#include "Skeleton.h"
#include "Blorb.h"
#include "Bird.h"
#include "Item.h"
#include "Bubble.h"
#include "Rock.h"
#include "BreakableWall.h"
#include "Gem.h"
#include "DirtPile.h"
#include "Platform.h"
#include "Chest.h"
#include "Checkpoint.h"
#include "MusicSheet.h"
#include "Platter.h"
#include "Chicken.h"
#include "Dragon.h"
#include "ObjectManager.h"
#include "HUD.h"
#include "SoundStream.h"

Game::Game(const Window& window) :
	m_Window{ window },
	m_pLevel{ new Level{} },
	m_pKnight{ new Knight{Point2f(175, 655)}},
	m_pCamera{ new Camera{window.width, window.height} },
	m_pLadder{ new Ladder{} },
	m_pObjectManager{ new ObjectManager{} },
	m_pHUD{ new HUD{Point2f(window.width, window.height) } },
	m_HasKnightAttacked{false},
	m_TimerOutOfBounds{},
	m_pWinScreenTexture{new Texture{"Resources/Images/EndScreen.jpg"}},
	m_Gold{},
	m_IsPlaying{true},
	m_pBackGroundMusic{ new SoundStream{"Resources/Sounds/Strike_The_Earth_Music_Loop.wav"} }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pBackGroundMusic->Play(true);
}

void Game::Cleanup( )
{
	delete m_pLevel;
	delete m_pKnight;
	delete m_pCamera;
	delete m_pLadder;
	delete m_pObjectManager;
	delete m_pHUD;
	DeleteRemainingObjects();
	delete m_pWinScreenTexture;
	delete m_pBackGroundMusic;
}

void Game::Update( float elapsedSec )
{

	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}

	if (m_IsPlaying)
	{
		//objects
		if (m_pCamera->HasStageChanged())
			m_pObjectManager->CreateObjects(m_pItems, m_pEnemies, m_pCamera->GetCurrentStage());
		m_pObjectManager->DeleteObjects(m_pItems, m_pEnemies, m_pKnight->GetShape());


		if (!m_pCamera->GetIsTransitioning() && !m_pCamera->GetIsGameDone())
		{
			//Knight
			m_pKnight->Update(elapsedSec);

			//Enemies
			EnemyUpdate(elapsedSec);

			//Items
			ItemUpdate(elapsedSec);

			//other
			DetectOutOfBounds(elapsedSec);
		}
		m_pCamera->Transition(elapsedSec);

		//Level
		m_pLevel->Update(elapsedSec);
		HandleLevelCollision();
		HandleLadders();
		SetActorsOnGround();
		m_pHUD->Update(elapsedSec, *m_pKnight);

		m_pBackGroundMusic->Resume();
	}
	else 
	{
		m_pBackGroundMusic->Pause();
	}
	m_pHUD->SetHudState(m_IsPlaying);
	UpdateSounds();
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_pLevel->DrawClouds(Rectf(0, 0, m_Window.width, m_Window.height));
	m_pLevel->Draw(m_pCamera->GetTransform(), m_pCamera->GetScale(),
		Point2f(m_pKnight->GetShape().left, m_pKnight->GetShape().bottom));

	glPushMatrix();

	//Camera
	m_pCamera->Transform(m_pKnight->GetShape());

	//Level
	m_pLadder->Draw();

	if (!m_pCamera->GetIsTransitioning() && !m_pCamera->GetIsGameDone())
	{
		//Items
		DrawItems();

		//Enemies
		DrawEnemies();

		//Knight
		m_pKnight->Draw();

	}

	glPopMatrix();

	//HUD
	m_pHUD->Draw(Point2f(m_Window.width, m_Window.height), 
		m_pKnight->GetHealth(), m_Gold);

	DrawEndScreen();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_i:
		PrintInfo();
		break;
	case SDLK_r:
		//in case player gets stuck
		m_pKnight->Respawn();
		break;
	case SDLK_p:
		if (m_IsPlaying) m_IsPlaying = false;
		else m_IsPlaying = true;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;

	m_pHUD->DetectClickOnSlider(Point2f(float(e.x), float(e.y)));
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	m_pHUD->DetectMouseDown(true);
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	m_pHUD->DetectMouseDown(false);
	m_pHUD->DetectClickOnButtons(Point2f(float(e.x), float(e.y)));
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::HandleLevelCollision()
{
	m_pLevel->HandleCollision(m_pKnight->GetShape(), m_pKnight->GetVelocity(), false);
	if (m_pLevel->IsInSpikes(m_pKnight->GetShape())) m_pKnight->Respawn();
}

void Game::HandleLadders()
{
	m_pKnight->SetIsOnLadder(m_pLadder->IsOnLadder(m_pKnight->GetShape(), m_pKnight->GetVelocity()),
		m_pLadder->GetMidPosCurrentLadder());
	m_pLadder->HandleCollision(m_pKnight->GetShape(), m_pKnight->GetVelocity(), m_pKnight->IsClimbing());
	m_pKnight->SetIsOnTopOfLadder(m_pLadder->GetIsOnTopLadder());
}

void Game::SetActorsOnGround()
{
	bool isEnemyOnGround{};
	bool isKnightOnGround{};

	if (m_pLadder->IsOnGround(m_pKnight->GetShape(), m_pKnight->IsClimbing()))
	{
		isKnightOnGround = true;
	}

	if (m_pLevel->IsOnGround(m_pKnight->GetShape())) 
		isKnightOnGround = true;

	for (int i{}; i < m_pItems.size(); ++i)
	{

		if (m_pItems[i])
		{
			if (typeid(*m_pItems[i]) == typeid(Rock))
			{
				Rock* pRock{ static_cast<Rock*>(m_pItems[i]) };
				if (pRock->GetIsActive())
				{
					if (pRock->IsOnGround(m_pKnight->GetShape()))
						isKnightOnGround = true;
				}
			}
			else if (typeid(*m_pItems[i]) == typeid(Platform))
			{
				Platform* pPlatform{ static_cast<Platform*>(m_pItems[i]) };
				if (pPlatform->GetIsActive())
				{
					if (pPlatform->IsOnGround(m_pKnight->GetShape()))
						isKnightOnGround = true;
				}
			}
			else if (typeid(*m_pItems[i]) == typeid(BreakableWall))
			{
				BreakableWall* pBreakableWall{ static_cast<BreakableWall*>(m_pItems[i]) };
				if (pBreakableWall->GetIsActive())
				{
					if (pBreakableWall->IsOnGround(m_pKnight->GetShape()))
						isKnightOnGround = true;
				}
			}
			

			for (int j{}; j < m_pEnemies.size(); ++j)
			{
				if (m_pEnemies[j])
				{
					if (m_pLevel->IsOnGround(m_pEnemies[j]->GetShape()))
						isEnemyOnGround = true;

					else if (typeid(*m_pItems[i]) == typeid(Rock))
					{
						Rock* pRock{ static_cast<Rock*>(m_pItems[i]) };
						if (pRock->GetIsActive())
						{
							if (pRock->IsOnGround(m_pEnemies[j]->GetShape()))
								isEnemyOnGround = true;
						}
					}
					else isEnemyOnGround = false;
				}
				if (m_pEnemies[j]) 
					m_pEnemies[j]->SetIsOnGround(isEnemyOnGround);
			}
		}
	}
	m_pKnight->SetIsOnGround(isKnightOnGround);
}

void Game::DrawEnemies() const
{
	for (int j{}; j < m_pEnemies.size(); ++j)
	{
		if (m_pEnemies[j])
			m_pEnemies[j]->Draw();
	}
}

void Game::EnemyUpdate(float elapsedSec)
{
	for (int j{}; j < m_pEnemies.size(); ++j)
	{
		if (m_pEnemies[j])
		{
			UpdateEnemies(elapsedSec, j);
			HandleHitDetectionEnemies(j);
			HandleLevelCollisionEnemies(j);
			DeleteGoneEnemies(j);
		}
	}
}

void Game::UpdateEnemies(float elapsedSec, int j)
{
	m_pEnemies[j]->Update(elapsedSec);
	m_pEnemies[j]->SpawnGems(m_pItems);
	if (typeid(*m_pEnemies[j]) == typeid(Skeleton))
	{
		Skeleton* pSkeleton{ static_cast<Skeleton*>(m_pEnemies[j]) };
		pSkeleton->UpdateState(elapsedSec, m_pKnight->GetShape());
	}
	else if (typeid(*m_pEnemies[j]) == typeid(Beeto) &&
		!m_pLevel->IsOnGround(m_pEnemies[j]->GetShape()))
	{
		m_pEnemies[j]->GetVelocity().x *= -1;
	}
	else if (typeid(*m_pEnemies[j]) == typeid(Dragon))
	{
		Dragon* pDragon{ static_cast<Dragon*>(m_pEnemies[j]) };
		pDragon->SpawnGemsDragon(m_pItems);
	}
}

void Game::HandleHitDetectionEnemies(int j)
{
	if (typeid(*m_pEnemies[j]) != typeid(Dragon))
	{
		if (utils::IsOverlapping(m_pKnight->GetHurtBox(), m_pEnemies[j]->GetHitBox()) && !m_pEnemies[j]->GetIsDying())
		{
			m_pKnight->GetHit();

		}
		if (utils::IsOverlapping(m_pKnight->GetHitBox(), m_pEnemies[j]->GetHitBox()) && !m_pEnemies[j]->GetIsDying() &&
			!m_pKnight->HasAttacked())
		{
			m_pEnemies[j]->GetHit();
			m_pEnemies[j]->SetKnightIsRight(m_pKnight->IsKnightRight());
			if (m_pKnight->IsJumpAttacking()) m_pKnight->Bounce(440);
		}
	}
	else
	{
		Dragon* pDragon{ static_cast<Dragon*>(m_pEnemies[j]) };
		if (pDragon->IsActive())
		{
			if (pDragon->CheckHit(m_pKnight->GetHitBox(), m_pKnight->HasAttacked()))
				if (m_pKnight->IsJumpAttacking()) m_pKnight->Bounce(380);

			if (pDragon->CheckHitBubbles(m_pKnight->GetHitBox()))
				if (m_pKnight->IsJumpAttacking()) m_pKnight->Bounce(310);

			if (pDragon->Collision(m_pKnight->GetHurtBox()))
				m_pKnight->GetHit();
		}
	}

}

void Game::HandleLevelCollisionEnemies(int j)
{
	if (typeid(*m_pEnemies[j]) != typeid(Bird))
		m_pLevel->HandleCollision(m_pEnemies[j]->GetShape(), m_pEnemies[j]->GetVelocity(), true);
}

void Game::DeleteGoneEnemies(int j)
{
	if (m_pEnemies[j]->GetIsDead())
	{
		delete m_pEnemies[j];
		m_pEnemies[j] = nullptr;
	}
}


void Game::DrawItems() const
{
	for (int i{}; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i])
		{
			m_pItems[i]->Draw();

			/*utils::SetColor(Color4f(1, 0, 0, 0.4f));
			utils::FillRect(m_pItems[i]->GetShape());*/
		}
	}
}

void Game::ItemUpdate(float elapsedSec)
{
	for (int i{}; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i])
		{
			UpdateItems(elapsedSec, i);
			ItemPlayerCollision(elapsedSec, i);
			SolidItemCollision(i);
			ItemLevelCollision(i);
			DeleteGoneItems(i);

			
		}
	}
}

void Game::UpdateItems(float elapsedSec, int i)
{
	m_pItems[i]->Update(elapsedSec);

	if (typeid(*m_pItems[i]) == typeid(Chest))
	{
		Chest* pChest{ static_cast<Chest*>(m_pItems[i]) };
		if (pChest->GetIsOpen())
		{
			m_pItems[i]->SpawnGems(m_pItems, m_pKnight->IsKnightRight());
		}
		if (!pChest->GetIsActive())
			m_pKnight->SetIsLooting(false, pChest->GetIsFacingLeft());
	}
}

void Game::ItemPlayerCollision(float elapsedSec, int i)
{
	//for interactables
	if (!m_pKnight->HasAttacked() && !m_pItems[i]->GetIsPickUp())
	{
		if (utils::IsOverlapping(m_pKnight->GetHitBox(), m_pItems[i]->GetShape()) &&
			m_pItems[i]->GetIsActive())
		{
			if (m_pKnight->IsJumpAttacking())
			{
				if (typeid(*m_pItems[i]) == typeid(Bubble))
				{
					m_pKnight->Bounce(390);
					m_pItems[i]->IsHit();
				}
				else if (typeid(*m_pItems[i]) == typeid(Rock))
				{
					m_pKnight->Bounce(200);
					m_pItems[i]->IsHit();
					m_pItems[i]->SpawnGems(m_pItems, !m_pKnight->IsKnightRight());
				}
			}
			else
			{
				m_pItems[i]->IsHit();
				m_pItems[i]->SpawnGems(m_pItems, !m_pKnight->IsKnightRight());
				if (typeid(*m_pItems[i]) == typeid(Platter))
				{
					Point2f spawnPos{ m_pItems[i]->GetShape().left + m_pItems[i]->GetShape().width / 5,
					m_pItems[i]->GetShape().bottom };
					m_pItems.push_back(new Chicken{ spawnPos });
				}
				else if (typeid(*m_pItems[i]) == typeid(Chest))
				{
					Chest* pChest{ static_cast<Chest*>(m_pItems[i]) };
					m_pKnight->SetIsLooting(pChest->GetIsOpen(), pChest->GetIsFacingLeft());

					if (pChest->GetIsOpen())
					{
						if (pChest->GetIsFacingLeft())
							m_pKnight->GetShape().left = pChest->GetShape().left - m_pKnight->GetShape().width / 2;
						else
							m_pKnight->GetShape().left = pChest->GetShape().left + m_pKnight->GetShape().width / 2;
					}
				}
				else if (typeid(*m_pItems[i]) == typeid(BreakableWall))
				{
					BreakableWall* pBreakableWall{ static_cast<BreakableWall*>(m_pItems[i]) };
					pBreakableWall->SpawnEnemies(m_pEnemies);
					m_pKnight->HasAttacked() = true; //otherwise hits object in the wall in same hit as wall
				}
			}
		}
	}

	if (utils::IsOverlapping(m_pKnight->GetShape(), m_pItems[i]->GetShape()) &&
		m_pItems[i]->GetIsPickUp() && m_pItems[i]->GetIsActive())
	{
		if (typeid(*m_pItems[i]) == typeid(Chicken))
		{
			m_pKnight->Healing(4);
		}
		else if (typeid(*m_pItems[i]) == typeid(Checkpoint))
		{
			const Point2f respawnPos{ m_pItems[i]->GetShape().left, m_pItems[i]->GetShape().bottom };
			m_pKnight->SetRespawnPos(respawnPos);
		}
		else if (typeid(*m_pItems[i]) == typeid(Gem))
		{
			Gem* pGem{ static_cast<Gem*>(m_pItems[i]) };
			m_Gold += pGem->GetValue();
		}
		m_pItems[i]->IsHit();
	}
}

void Game::SolidItemCollision(int i)
{
	if (typeid(*m_pItems[i]) == typeid(Rock))
	{
		Rock* pRock{ static_cast<Rock*>(m_pItems[i])};
		if (pRock->GetIsActive())
		{
			pRock->HandleCollision(m_pKnight->GetShape(), m_pKnight->GetVelocity(), false);

			for (int j{}; j < m_pEnemies.size(); ++j)
			{
				if (m_pEnemies[j])
				{
					pRock->HandleCollision(m_pEnemies[j]->GetShape(), m_pEnemies[j]->GetVelocity(), true);
				}
			}
		}
	}
	else if (typeid(*m_pItems[i]) == typeid(BreakableWall))
	{
		BreakableWall* pWall{ static_cast<BreakableWall*>(m_pItems[i]) };
		if (pWall->GetIsActive())
		{
			pWall->HandleCollision(m_pKnight->GetShape(), m_pKnight->GetVelocity(), false);

			for (int j{}; j < m_pEnemies.size(); ++j)
			{
				if (m_pEnemies[j])
				{
					pWall->HandleCollision(m_pEnemies[j]->GetShape(), m_pEnemies[j]->GetVelocity(), true);
				}
			}
		}
	}
	else if (typeid(*m_pItems[i]) == typeid(Platform))
	{
		Platform* pPlatform{ static_cast<Platform*>(m_pItems[i]) };
		if (pPlatform->GetIsActive())
		{
			pPlatform->HandleCollision(m_pKnight->GetShape(), m_pKnight->GetVelocity(), false);
		}
	}
}

void Game::ItemLevelCollision(int i)
{
	bool gemOnGround{};

	if (typeid(*m_pItems[i]) == typeid(Gem))
	{
		Gem* pGem{ static_cast<Gem*>(m_pItems[i]) };
		m_pLevel->HandleCollision(pGem->GetLevelCollisionShape(), pGem->GetVelocity(), true);
		if (m_pLevel->IsOnGround(pGem->GetLevelCollisionShape())) gemOnGround = true;

		for (int k{}; k < m_pItems.size(); ++k)
		{
			if (m_pItems[k])
			{
				if (typeid(*m_pItems[k]) == typeid(Rock))
				{
					Rock* pRock{ static_cast<Rock*>(m_pItems[k]) };
					if (pRock->GetIsActive())
					{
						pRock->HandleCollision(pGem->GetLevelCollisionShape(), pGem->GetVelocity(), true);
						if (pRock->IsOnGround(pGem->GetLevelCollisionShape()))
							gemOnGround = true;
					}
				}
			}
		}
		pGem->SetIsOnGround(gemOnGround);
	}
	else if (typeid(*m_pItems[i]) == typeid(Chicken))
	{
		Chicken* pChicken{ static_cast<Chicken*>(m_pItems[i]) };
		m_pLevel->HandleCollision(pChicken->GetLevelCollisionShape(), pChicken->GetVelocity(), true);
		if (m_pLevel->IsOnGround(pChicken->GetLevelCollisionShape())) pChicken->SetIsOnGround(true);
	}
}

void Game::DeleteGoneItems(int i)
{
	if (m_pItems[i]->GetIsDead())
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}
}

void Game::DetectOutOfBounds(float elapsedSec)
{
	if (!m_pCamera->GetIsPlayerInBounds())
	{
		m_TimerOutOfBounds += elapsedSec;
		if (m_TimerOutOfBounds > 0.8f) m_pKnight->Respawn();
	}
	else m_TimerOutOfBounds = 0;
}

void Game::DeleteRemainingObjects()
{
	for (int i{}; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]) delete m_pItems[i];
	}
	for (int j{}; j < m_pEnemies.size(); ++j)
	{
		if (m_pEnemies[j]) delete m_pEnemies[j];
	}
}

void Game::DrawEndScreen() const
{
	if (m_pCamera->GetIsGameDone())
	{
		m_pWinScreenTexture->Draw(Rectf(0, 0, m_Window.width, m_Window.height));
	}
}

void Game::UpdateSounds()
{
	m_pKnight->SetVolume(m_pHUD->GetVolume());
	for (int i{}; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i])
		{
			if (typeid(*m_pItems[i]) == typeid(Gem))
			{
				Gem* pGem{ static_cast<Gem*>(m_pItems[i]) };
				pGem->SetVolume(m_pHUD->GetVolume());
			}
		}
	}
	m_pBackGroundMusic->SetVolume(m_pHUD->GetVolume());
}

void Game::PrintInfo()
{
	std::cout << "-- SHOVEL KNIGHT --\n\n" <<
		"use A and D to move left and right\n" <<
		"press K or SPACEBAR to jump\n" <<
		"press J to attack\n" <<
		"press S while jumping to do a jump attack that lets you bounce on objects\n" <<
		"use W and S to climb ladders\n" <<
		"Press P to go to pause/unpause the game\n" <<
		"\nExtra Info: \n" <<
		"when on a ladder, jump to the sides to get off\n" <<
		"the big dragon can only be killed if you hit his head\n" <<
		"if you press attack during a downwards thrust, you go out of the thrust\n" <<
		 "\nHAVE FUN\n";
}