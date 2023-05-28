#pragma once
#include "Graphics.h"
#include <random>

class HoldBoard
{
public:
	HoldBoard(Graphics& gfx, std::mt19937& rng);
	void DrawQueueBorder() const;
	void DrawHoldBorder() const;
	void DrawWaitQueue() const;
	void DrawPiece(int index, int queue_index) const;
	int GetFirstInQueue() const;
	void Follow();
	int GetHoldIndex() const;
	void SetHoldIndex(int current_index);
private:
	Graphics& gfx;
	std::mt19937& rng;
	static constexpr int x = 400;
	static constexpr int y = 50;
	static constexpr int dimension = 50;
	static constexpr int tile_dim = 10;
	static constexpr int gap = 50;
	static constexpr int nQueue = 4;
	static constexpr int height = nQueue;
	static constexpr int width = 1;
	static constexpr int borderWidth = 4;
	static constexpr int borderPadding = 2;
	static constexpr int cellPadding = 1;
	static constexpr Color borderColor = Colors::White;
	int wait_piece[nQueue + 1];
	static constexpr Color pieceColorArr[7] =
	{
		{0,255,255},	//I piece color = Cyan
		{255,128,0},	//L piece color = Orange
		{0,0,204},		//J piece color = Blue
		{255,255,0},	//O piece color = Yellow
		{0,255,0},		//S piece color = Green
		{255,0,0},		//Z piece color = Red
		{102,0,204}		//T piece color = Pupple
	};
};