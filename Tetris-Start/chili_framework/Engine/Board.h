#pragma once

#include "Location.h"
#include "Graphics.h"

class Board
{
private:
	struct Tile {
	public:
		bool isOccupy = false;
		Color tileColor;
	};
public:
	Board::Board(Graphics& gfx);
	void DrawCell(const Location& loc, Color c) const;
	void DrawBorder() const;
	void DrawBackground() const;
	void DrawTile() const;
	void MoveRows(int row);
	void ClearRow();
	void SetTile(const Location& loc, Color c);
	int GetGridHeight() const;
	int GetGridWidth() const;
	bool IsTileOccupy(Location loc) const;
	void DrawGameOver() const;
	void DrawGhostTile(Location loc, Color c) const;
	int GetLineCombo() const;
private:
	Graphics& gfx;
	static constexpr int x = 150;
	static constexpr int y = 50;
	static constexpr int width = 10;
	static constexpr int ndisplay_row = 21;
	static constexpr int height = 24;
	static constexpr int nUndisplay_row = height - ndisplay_row;
	static constexpr int dimension = 20;
	static constexpr int borderWidth = 4;
	static constexpr int borderPadding = 2;
	static constexpr int cellPadding = 1;
	static constexpr Color borderColor = Colors::White;
	static constexpr Color backgroundLineColor = Colors::MakeRGB(96, 96, 96);
	static constexpr Color backgroundColor = Colors::MakeRGB(50, 50, 50);
	Tile tiles[width][height];
	int line_combo = 0;
};