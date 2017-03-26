#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_DrawTree(TMainForm *_this, LPVOID DestCanvas, long LeftOffset, long TopOffset, BOOL IgnoreDebugString)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        span;
	int        i, end;
	LPVOID     ssgActionListner;

	DGridHandle = TWinControl_GetHandle(_this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
	{
		int pos;

		pos = si.nPos - si.nMin;
		if (pos)
		{
			int range;

			range = si.nMax - si.nMin;
			if (range)
			{
				LeftOffset -= MulDiv(_this->DGrid->DefaultColWidth - clientWidth, pos, range);
			}
		}
	}

	span = _this->DGrid->DefaultRowHeight;
	rect.left = LeftOffset;
	rect.right = _this->DGrid->DefaultColWidth + LeftOffset;
	if (DestCanvas)
	{
		rect.top = TopOffset;
	}
	else
	{
		rect.top = 0;
		DestCanvas = _this->DGrid->Canvas;
	}
	rect.bottom = rect.top + span;

	i = _this->DGrid->TopRow;
	end = clientHeight / _this->DGrid->DefaultRowHeight;
	if (clientHeight % _this->DGrid->DefaultRowHeight)
		end++;
	end += i;
	if (end > _this->DGrid->RowCount)
		end = _this->DGrid->RowCount;
	if (IgnoreDebugString)
	{
		ssgActionListner = _this->ssgCtrl.ssgActionListner;
		_this->ssgCtrl.ssgActionListner = NULL;
	}
	for (end--; i < end; i++)
	{
		TMainForm_DrawTreeCell(_this, DestCanvas, i, &rect);
		rect.top += span;
		rect.bottom += span;
	}
	rect.bottom = rect.top + TDrawGrid_GetRowHeights(_this->DGrid, end);
	TMainForm_DrawTreeCell(_this, DestCanvas, end, &rect);
	if (IgnoreDebugString)
		_this->ssgCtrl.ssgActionListner = ssgActionListner;
}
