#include "game.h"
#include <iostream>
#include <sstream>

void Game::Start()
{
	GAME_MSG_1("游戏开始");
	m_ai_givein = false;
	PrintCurrentState();
}

void Game::NextMove()
{
	if( m_cur_game->isYgrittesPlaying )
	{
		m_last_shot = AIMove();
	}
	else
	{
		m_last_shot = PlayerMove();
	}
	m_cur_game->Move( m_last_shot );
	PrintCurrentState();
}

bool Game::HasEnd()
{
	return m_ai_givein || m_cur_game->CheckGameEnd();
}

OneShot Game::AIMove()
{
	GAME_MSG_1("AI 思考中");
	OneShot shot;
	if( !m_cur_game->ThinkForNextMove( shot ) )
	{
		m_ai_givein = true;
	}
	return shot;
}

OneShot Game::PlayerMove()
{
	GAME_MSG_1("玩家选择出牌");
	while( true )
	{
		std::cout<<"输入你要出的牌，0表示不出)。空格键隔开，回车键确认："<<std::endl;
		m_last_shot.vec_card.clear();

		std::string playerStr;
		getline( std::cin, playerStr );

		std::stringstream strStream(playerStr);
		int c;
		while( strStream>> c )
		{
			if( c == 0 )
			{
				m_last_shot = OneShot();
				return m_last_shot;
			}
			m_last_shot.vec_card.push_back( Card( c, Card::SuitType_clubs ) );
		}

		if( !m_last_shot.CalSelfType() )
		{
			GAME_MSG_2("错误的组合！");
		}
		else if( !m_cur_game->JonSnow.isValidShot( m_last_shot ) )
		{
			GAME_MSG_2("没有这张牌！");
		}
		else if( !m_last_shot.IsHigher(m_cur_game->lastShot) )
		{
			GAME_MSG_2("不能压过对方！");
		}
		else
		{
			return m_last_shot;
		}
		std::cout<<"出牌错误，请重试"<<std::endl;
	}
	return OneShot();
}

void Game::PrintCurrentState()
{
	m_cur_game->Print();
}

void Game::PrintResult()
{
	GAME_MSG_1("游戏结束");
	
	PrintCurrentState();

	if( m_ai_givein )
	{
		GAME_MSG_1("AI认输");
	}
	else
	{
		if( m_cur_game->m_curIsMax )
		{
			GAME_MSG_1("AI赢了");
		}
		else
		{
			GAME_MSG_1("你赢了");
		}
	}
}