#include "HoldBoard.h"
std::uniform_int_distribution<int> indexDist(0, 6);
constexpr int empty = -1;

HoldBoard::HoldBoard(Graphics& gfx, std::mt19937& rng)
	:
	gfx(gfx),
	rng(rng)
{
	for (int i = 0; i < nQueue; ++i)
	{
		wait_piece[i] = indexDist(rng);
	}
	wait_piece[nQueue] = empty;
}

void HoldBoard::DrawQueueBorder() const
{
	int top = y;
	int left = x;
	int bottom = top + height * dimension + 2 * (borderPadding + borderWidth);
	int right = left + width * dimension + 2 * (borderPadding + borderWidth);

	gfx.DrawRect(left, top, right, top + borderWidth, borderColor);
	gfx.DrawRect(left, bottom - borderWidth, right, bottom, borderColor);
	gfx.DrawRect(left, top + borderWidth, left + borderWidth, bottom - borderWidth, borderColor);
	gfx.DrawRect(right - borderWidth, top + borderWidth, right, bottom - borderWidth, borderColor);
}

void HoldBoard::DrawHoldBorder() const
{
	int top = y + height * dimension + 2 * (borderPadding + borderWidth) + gap;
	int left = x;
	int bottom = top + dimension + 2 * (borderPadding + borderWidth);
	int right = left + dimension + 2 * (borderPadding + borderWidth);

	gfx.DrawRect(left, top, right, top + borderWidth, borderColor);
	gfx.DrawRect(left, bottom - borderWidth, right, bottom, borderColor);
	gfx.DrawRect(left, top + borderWidth, left + borderWidth, bottom - borderWidth, borderColor);
	gfx.DrawRect(right - borderWidth, top + borderWidth, right, bottom - borderWidth, borderColor);
}

void HoldBoard::DrawWaitQueue() const
{
	for (int i = 0; i <= nQueue; ++i)
	{
		DrawPiece(wait_piece[i], i);
	}
}

void HoldBoard::DrawPiece(int piece_index, int queue_index) const
{
	int nQueueY = y + borderPadding + borderWidth + dimension / 2 + dimension * queue_index;
	int nQueueX = x + borderPadding + borderWidth + dimension / 2;
	if (queue_index == nQueue)
	{
		nQueueY += 2 * (borderPadding + borderWidth) + gap;
	}
	switch (piece_index)
	{
	case 0:
		gfx.DrawRectDim(nQueueX - tile_dim * 2, nQueueY - tile_dim / 2, tile_dim * 4, tile_dim, pieceColorArr[piece_index]);
		break;
	case 1:
		gfx.DrawRectDim(nQueueX - 3 * tile_dim / 2, nQueueY, 3 * tile_dim, tile_dim, pieceColorArr[piece_index]);
		gfx.DrawRectDim(nQueueX + tile_dim / 2, nQueueY - tile_dim, tile_dim, tile_dim, pieceColorArr[piece_index]);
		break;
	case 2:
		gfx.DrawRectDim(nQueueX - 3 * tile_dim / 2, nQueueY, 3 * tile_dim, tile_dim, pieceColorArr[piece_index]);
		gfx.DrawRectDim(nQueueX - 3 * tile_dim / 2, nQueueY - tile_dim, tile_dim, tile_dim, pieceColorArr[piece_index]);
		break;
	case 3:
		gfx.DrawRectDim(nQueueX - tile_dim, nQueueY - tile_dim, 2 * tile_dim, 2 * tile_dim, pieceColorArr[piece_index]);
		break;
	case 4:
		gfx.DrawRectDim(nQueueX - tile_dim / 2, nQueueY - tile_dim, tile_dim * 2, tile_dim, pieceColorArr[piece_index]);
		gfx.DrawRectDim(nQueueX - 3 * tile_dim / 2, nQueueY, tile_dim * 2, tile_dim, pieceColorArr[piece_index]);
		break;
	case 5:
		gfx.DrawRectDim(nQueueX - 3 * tile_dim / 2, nQueueY - tile_dim, tile_dim * 2, tile_dim, pieceColorArr[piece_index]);
		gfx.DrawRectDim(nQueueX - tile_dim / 2, nQueueY, tile_dim * 2, tile_dim, pieceColorArr[piece_index]);
		break;
	case 6:
		gfx.DrawRectDim(nQueueX - 3 * tile_dim / 2, nQueueY, tile_dim * 3, tile_dim, pieceColorArr[piece_index]);
		gfx.DrawRectDim(nQueueX - tile_dim / 2, nQueueY - tile_dim, tile_dim, tile_dim, pieceColorArr[piece_index]);
		break;
	default:
		break;
	}
}

int HoldBoard::GetFirstInQueue() const
{
	return wait_piece[0];
}

void HoldBoard::Follow()
{
	
	for (int i = 0; i < nQueue - 1; ++i)
	{
		wait_piece[i] = wait_piece[i + 1];
	}
	wait_piece[nQueue - 1] = indexDist(rng);
}

int HoldBoard::GetHoldIndex() const
{
	return wait_piece[nQueue];
}

void HoldBoard::SetHoldIndex(int current_index)
{
	wait_piece[nQueue] = current_index;
}
