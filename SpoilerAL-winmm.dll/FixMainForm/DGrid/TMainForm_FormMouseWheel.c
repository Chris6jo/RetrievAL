#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __fastcall _TMainForm_FormMouseWheel(TMainForm *_this, LPVOID Sender, int Shift, BOOLEAN *Handled, POINT *MousePos, int WheelDelta)
{
	HWND DGridHandle;
	RECT rect;

	DGridHandle = TWinControl_GetHandle(_this->DGrid);
	if (GetWindowRect(DGridHandle, &rect) && PtInRect(&rect, *MousePos))
	{
		long lStyle;

		*Handled = TRUE;
		lStyle = GetWindowLongA(DGridHandle, GWL_STYLE);
		if (lStyle & WS_VSCROLL)
		{
			int topRow;

			topRow = _this->DGrid->TopRow - WheelDelta / 120;
			if (topRow > 0)
			{
				int clientRows;

				clientRows = TDrawGrid_GetClientHeight(_this->DGrid) / _this->DGrid->DefaultRowHeight;
				if (_this->DGrid->RowCount > clientRows)
				{
					int maxTopRow;

					maxTopRow = _this->DGrid->RowCount - clientRows;
					if (topRow > maxTopRow)
					{
						topRow = maxTopRow;
					}
				}
				else
				{
					topRow = 0;
				}
			}
			else
			{
				topRow = 0;
			}
			TDrawGrid_SetTopRow(_this->DGrid, topRow);
		}
		else if (lStyle & WS_HSCROLL)
		{
			SCROLLINFO si;

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE | SIF_POS;
			if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
			{
				int clientWidth;

				clientWidth = TDrawGrid_GetClientWidth(_this->DGrid);
				if (clientWidth)
				{
					si.nPos -= MulDiv(MulDiv(GetSystemMetrics(SM_CXHTHUMB), WheelDelta, 120), si.nMax - si.nMin, clientWidth);
					if (si.nPos < si.nMin)
						si.nPos = si.nMin;
					else if (si.nPos > si.nMax)
						si.nPos = si.nMax;
					SendMessage(DGridHandle, WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, si.nPos), (LPARAM)NULL);
				}
			}
		}
	}
	else if (!GetWindowRect(TWinControl_GetHandle(_this->PageCtrl), &rect) || !PtInRect(&rect, *MousePos))
	{
		*Handled = TRUE;
	}
}
