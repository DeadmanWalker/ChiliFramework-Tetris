/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	brd(gfx),
	piece(rng),
	rng(std::random_device()()),
	hold_brd(gfx,rng)
{

}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (isGameStarted)
	{
		if (!piece.GetIsGameOver())
		{
			piece.RotatePiece(brd, wnd.kbd);
			piece.InstaDropControl(brd, wnd.kbd, hold_brd);
			piece.Control(brd, wnd.kbd);
			piece.HoldPiece(wnd.kbd, brd, hold_brd);
			piece.PieceFall(brd, wnd.kbd, hold_brd);
			
			brd.ClearRow();
			line_cleared += brd.GetLineCombo();
			if (line_cleared >= level_up_req)
			{
				level++;
				level_up_req += 10 * level;
				piece.SetGravity(level);
			}
		}
	}
	else
	{
		isGameStarted = wnd.kbd.KeyIsPressed(VK_RETURN);
	}
}

void Game::ComposeFrame()
{
	brd.DrawBackground();
	if (isGameStarted)
	{
		if (!piece.GetIsGameOver())
		{
			piece.DrawPiece(brd);
			piece.DrawGhostPiece(brd);
			brd.DrawTile();
			hold_brd.DrawWaitQueue();
		}
		else
		{
			brd.DrawGameOver();
		}
	}
	hold_brd.DrawQueueBorder();
	hold_brd.DrawHoldBorder();
	brd.DrawBorder();
}