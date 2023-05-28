#include "Tetromino.h"
#include <assert.h>

				//[][][][]
static const Location I[4] = { {-1,0},{0,0},{1,0},{2,0} }; 
				//    []
				//[][][]
static const Location L[4] = { {-1,0},{0,0},{1,0},{1,-1} };
				//[]
				//[][][]	
static const Location J[4] = { {-1,-1},{-1,0},{0,0},{1,0} };
				//[][]
				//[][]
static const Location O[4] = { {0,0},{1,0},{0,-1},{1,-1} };
				//  [][]
				//[][]
static const Location S[4] = { {-1,0},{0,0},{0,-1},{1,-1} };
				//[][]
				//  [][]
static const Location Z[4] = { {-1,-1},{0,-1},{0,0},{1,0} };
				//  []
				//[][][]
static const Location T[4] = { {-1,0},{0,0},{0,-1},{1,0} };

Tetromino::Tetromino(std::mt19937& rng)
{
	std::uniform_int_distribution<int> indexDist(0, nPieces - 1);
	int index = indexDist(rng);
	SetPieceType(index);
}

void Tetromino::SetPieceType(int index)
{	
	piece_index = index;
	switch (index) {
	case 0:
		SetTile(I);
		break;
	case 1:
		SetTile(L);
		break;
	case 2:
		SetTile(J);
		break;
	case 3:
		SetTile(O);
		break;
	case 4:
		SetTile(S);
		break;
	case 5:
		SetTile(Z);
		break;
	case 6:
		SetTile(T);
		break;
	}
}

void Tetromino::RespawnPiece(Board& brd)
{
	pivot = spawn_loc;
	if (!IsValidPos(pivot, tile_loc, brd))
	{
		PieceKick(tile_loc, brd, canSpawn);
	}
	isSpawn = canSpawn;
}

void Tetromino::SetTile(const Location loc[4])
{
	if (canSpawn)
	{
		for (int i = 0; i < 4; i++)
		{
			tile_loc[i] = loc[i];
		}
	}
}

void Tetromino::DrawPiece(Board& brd) const
{
	if (canSpawn)
	{
		if (place_count % 3 < 1)
		{
			for (int i = 0; i < 4; ++i)
			{
				brd.DrawCell(pivot + tile_loc[i], pieceColorArr[piece_index]);
			}
		}
	}
}

void Tetromino::RotatePiece(Board& brd, Keyboard& kbd)
{
	Location rotated_loc[4];
	bool isRotated = false;
	double IandO_rotationX = 0;
	double IandO_rotationY = 0;
	if (piece_index == 3 || piece_index == 0)
	{
		IandO_rotationX = 0.5;
		IandO_rotationY = 0.5;
		if (piece_index == 3)
		{
			IandO_rotationY = -IandO_rotationY;
		}
	}

	if (kbd.KeyIsPressed(VK_UP))
	{
		if (!inhibit_rotate)
		{
			for (int i = 0; i < 4; ++i)
			{
				rotated_loc[i].y = (int)((tile_loc[i].x - IandO_rotationX) + IandO_rotationY);
				rotated_loc[i].x = (int)(-(tile_loc[i].y - IandO_rotationY) + IandO_rotationX);
			}
			isRotated = true;
			inhibit_rotate = true;
		}
	}
	else if (kbd.KeyIsPressed('Z'))
	{
		if (!inhibit_rotate)
		{
			for (int i = 0; i < 4; ++i)
			{
				rotated_loc[i].y = (int)(-(tile_loc[i].x - IandO_rotationX) + IandO_rotationY);
				rotated_loc[i].x = (int)((tile_loc[i].y - IandO_rotationY) + IandO_rotationX);
			}
			isRotated = true;
			inhibit_rotate = true;
		}
	}
	else inhibit_rotate = false;

	if (isRotated)
	{
		if (IsValidPos(pivot, rotated_loc, brd))
		{
			for (int i = 0; i < 4; i++)
			{
				tile_loc[i] = rotated_loc[i];
			}
			place_count = placement_delay;
		}
		else
		{
			bool isKicked = true;
			PieceKick(rotated_loc, brd, isKicked);
			if (isKicked)
			{
				for (int i = 0; i < 4; i++)
				{
					tile_loc[i] = rotated_loc[i];
				}
				place_count = placement_delay;
			}
		}
	}
}

void Tetromino::PlacePiece(Board& brd, HoldBoard& hold_brd)
{
	isSpawn = false;
	if (spawn_count <= 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			brd.SetTile(pivot + tile_loc[i], pieceColorArr[piece_index]);
		}
		place_count = placement_delay;
		fall_count = gravity;
		inhibitHold = false;
		SetPieceType(hold_brd.GetFirstInQueue());
		hold_brd.Follow();
		RespawnPiece(brd);
		spawn_count = spawn_delay;
	}

}

void Tetromino::Control(Board& brd, const Keyboard& kbd)
{
	if (isSpawn)
	{
		if (kbd.KeyIsPressed(VK_RIGHT) && holdR_count <= 0)
		{
			if (IsValidPos({ pivot.x + 1,pivot.y }, tile_loc, brd))
			{
				pivot.Add({ 1,0 });
				if (!lastframe_Rkeypress)
				{
					holdR_count = hold_cooldown;
				}
				else holdR_count = press_cooldown;
			}
		}
		else if (lastframe_Rkeypress && !kbd.KeyIsPressed(VK_RIGHT))
		{
			holdR_count += press_cooldown - hold_cooldown;
		}

		if (kbd.KeyIsPressed(VK_LEFT) && holdL_count <= 0)
		{
			if (IsValidPos({ pivot.x - 1,pivot.y }, tile_loc, brd))
			{
				pivot.Add({ -1,0 });
				if (!lastframe_Lkeypress)
				{
					holdL_count = hold_cooldown;
				}
				else holdL_count = press_cooldown;
			}
		}
		else if (lastframe_Lkeypress && !kbd.KeyIsPressed(VK_LEFT))
		{
			holdL_count += press_cooldown - hold_cooldown;
		}

		if (holdR_count > 0) --holdR_count;
		if (holdL_count > 0) --holdL_count;
		lastframe_Rkeypress = kbd.KeyIsPressed(VK_RIGHT);
		lastframe_Lkeypress = kbd.KeyIsPressed(VK_LEFT);
	}
	else
	{
		spawn_count--;
		inhibit_rotate = true;
	}
}

void Tetromino::InstaDropControl(Board& brd, const Keyboard& kbd, HoldBoard& hold_brd)
{
	if (kbd.KeyIsPressed(VK_SPACE))
	{
		if (!inhibit_instaPlace)
		{
			pivot = Ghostpiece_loc(brd);
			PlacePiece(brd, hold_brd);
			inhibit_instaPlace = true;
			place_count = placement_delay;
		}
	}
	else inhibit_instaPlace = false;
}

void Tetromino::PieceKick(const Location loc_arr[], Board& brd, bool& canKicked)
{
	if (IsValidPos({ pivot.x - 1,pivot.y }, loc_arr, brd))
	{
		pivot.Add({ -1, 0 });
	}
	else if (IsValidPos({ pivot.x + 1,pivot.y }, loc_arr, brd))
	{
		pivot.Add({ 1, 0 });
	}
	else if (IsValidPos({ pivot.x,pivot.y + 1 }, loc_arr, brd))
	{
		pivot.Add({ 0,1 });
	}
	else if (IsValidPos({ pivot.x,pivot.y - 1 }, loc_arr, brd))
	{
		pivot.Add({ 0,-1 });
	}
	else if (IsValidPos({ pivot.x - 1,pivot.y + 1 }, loc_arr, brd))
	{
		pivot.Add({ -1,1 });
	}
	else if (IsValidPos({ pivot.x + 1,pivot.y + 1 }, loc_arr, brd))
	{
		pivot.Add({ 1,1 });
	}
	else if (IsValidPos({ pivot.x - 1,pivot.y - 1 }, loc_arr, brd))
	{
		pivot.Add({ -1,-1 });
	}
	else if (IsValidPos({ pivot.x + 1,pivot.y - 1 }, loc_arr, brd))
	{
		pivot.Add({ 1,-1 });
	}
	else if (piece_index == 0)
	{
		if (IsValidPos({ pivot.x - 2,pivot.y }, loc_arr, brd))
		{
			pivot.Add({ -2,0 });
		}
		else if (IsValidPos({ pivot.x + 2,pivot.y }, loc_arr, brd))
		{
			pivot.Add({ 2,0 });
		}
		else if (IsValidPos({ pivot.x,pivot.y - 2 }, loc_arr, brd))
		{
			pivot.Add({ 0,-2 });
		}
		else if (IsValidPos({ pivot.x,pivot.y + 2 }, loc_arr, brd))
		{
			pivot.Add({ 0,2 });
		}
	}
	else canKicked = false;
}

void Tetromino::PieceFall(Board& brd, Keyboard& kbd,HoldBoard& hold_brd)
{
	if (pivot.y < Ghostpiece_loc(brd).y)
	{
		if (fall_count <= 0)
		{
			pivot.Add({ 0, 1 });
			fall_count = gravity;
		}
		else --fall_count;

		if (kbd.KeyIsPressed(VK_DOWN))
		{
			fall_count -= 5;
		}
	}
	else
	{
		if (place_count <= 0 || !isSpawn)
		{
			PlacePiece(brd, hold_brd);
		}
		else --place_count;
	}
}

bool Tetromino::GetIsGameOver() const
{
	for (int i = 0; i < 4; ++i)
	{
		if ((pivot + tile_loc[i]).y < 0)
		{
			return true;
		}
	}
	return (!canSpawn);
}

void Tetromino::HoldPiece(Keyboard& kbd, Board& brd, HoldBoard& hold_brd)
{
	if (kbd.KeyIsPressed('C'))
	{
		if (!inhibitHold)
		{
			int tempt = piece_index;
			if (hold_brd.GetHoldIndex() == -1)
			{
				piece_index = hold_brd.GetFirstInQueue();
			}
			else
			{
				piece_index = hold_brd.GetHoldIndex();
			}
			hold_brd.SetHoldIndex(tempt);
			isSpawn = false;
			SetPieceType(piece_index);
			RespawnPiece(brd);
			inhibitHold = true;
			place_count = placement_delay;
			fall_count = gravity;
		}
	}
}

void Tetromino::DrawGhostPiece(Board& brd)
{
	if (isSpawn)
	{
		Location ghostpiece = Ghostpiece_loc(brd);
		for (int i = 0; i < 4; ++i)
		{
			brd.DrawGhostTile(ghostpiece + tile_loc[i], pieceColorArr[piece_index]);
		}
	}
}

void Tetromino::SetGravity(int level)
{
	gravity = (int)(pow((0.8f - (level - 1) * 0.007f), (level - 1)) * 60);
}

bool Tetromino::IsValidPos(Location loc, const Location loc_arr[], Board& brd) const
{
	for (int i = 0; i < 4; i++)
	{
		if(brd.IsTileOccupy(loc + loc_arr[i])) return false;
		if (loc.x + loc_arr[i].x < 0 || loc.x + loc_arr[i].x >= brd.GetGridWidth()) return false;
		if (loc.y + loc_arr[i].y < 0 || loc.y + loc_arr[i].y >= brd.GetGridHeight()) return false;
	}
	return true;
}

Location Tetromino::Ghostpiece_loc(Board& brd) const
{
	Location ghostpiece_loc = pivot;
	while (ghostpiece_loc.y < brd.GetGridHeight())
	{
		if(!IsValidPos({ ghostpiece_loc.x, ghostpiece_loc.y + 1 }, tile_loc, brd)) return ghostpiece_loc;
		ghostpiece_loc.y += 1;
	}
	return ghostpiece_loc;
}
