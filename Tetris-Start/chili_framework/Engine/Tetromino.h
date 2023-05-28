#pragma once

#include "Board.h"
#include "Keyboard.h"
#include "HoldBoard.h"
#include <random>


class Tetromino
{
private:
	bool IsValidPos(Location loc, const Location loc_arr[], Board& brd) const;
	void SetTile(const Location loc[4]);
	void PieceKick(const Location loc_arr[], Board& brd, bool& canKicked);
	void RespawnPiece(Board& brd);
	void SetPieceType(int index);
	Location Ghostpiece_loc(Board& brd) const;
public:
	Tetromino(std::mt19937& rng);
	void DrawPiece(Board& brd) const;
	void RotatePiece(Board& brd, Keyboard& kbd);
	void PlacePiece(Board& brd, HoldBoard& hold_brd);
	void Control(Board& brd, const Keyboard& kbd);
	void InstaDropControl(Board& brd, const Keyboard& kbd, HoldBoard& hold_brd);
	void PieceFall(Board& brd, Keyboard& kbd, HoldBoard& hold_brd);
	bool GetIsGameOver() const;
	void HoldPiece(Keyboard& kbd, Board& brd,HoldBoard& hold_brd);
	void DrawGhostPiece(Board& brd);
	void SetGravity(int level);
private:
	Location tile_loc[4];
	static constexpr Location spawn_loc = { 4,4 };
	Location pivot = spawn_loc;
	static constexpr int nPieces = 7;
	int piece_index;
	static constexpr Color pieceColorArr[nPieces] =
	{
		{0,255,255},	//I piece color = Cyan
		{255,128,0},	//L piece color = Orange
		{0,0,204},		//J piece color = Blue
		{255,255,0},	//O piece color = Yellow
		{0,255,0},		//S piece color = Green
		{255,0,0},		//Z piece color = Red
		{102,0,204}		//T piece color = Pupple
	};
	static constexpr int press_cooldown = 6;
	static constexpr int hold_cooldown = 16;
	bool lastframe_Lkeypress = false;
	bool lastframe_Rkeypress = false;
	int holdL_count = 0;
	int holdR_count = 0;
	bool inhibit_instaPlace = false;
	bool inhibit_rotate = false;
	bool canSpawn = true;
	int gravity = 60;
	int fall_count = gravity;
	static constexpr int placement_delay = 30;
	int place_count = placement_delay;
	static constexpr int spawn_delay = 10;
	int spawn_count = spawn_delay;
	bool isSpawn = true;
	bool inhibitHold = false;
};
