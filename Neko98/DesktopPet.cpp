// DesktopPet.cpp: implementation of the CDesktopPet class.
//
//////////////////////////////////////////////////////////////////////

#include "DesktopPet.h"
#include "NekoCommon.h" // Added for ICONINFO, BITMAP

//constants
#define UPDATE_COUNTER_MAX 50 //number of redraws until desktop handle is recalculated

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDesktopPet::CDesktopPet() : CDesktopHack(), CPet()
{
	//clear the update counter
	m_nUpdateHWndCounter = 0;

	//set bounds
//	GetDesktopRect(m_rcBounds);
	SetRect(&m_rcBounds, 0, 0, GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1, GetSystemMetrics(SM_CYVIRTUALSCREEN) - 1);
}

CDesktopPet::~CDesktopPet()
{
	RedrawWindow(m_hWndDesktop, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}



//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

void CDesktopPet::Draw(int nImage)
{
	//grab the device context of the desktop
	HDC hDC = GetDC(m_hWndDesktop);

	//draw the icon on it
	DrawIconEx(hDC, m_ptPosition.x, m_ptPosition.y, m_hIcons[nImage], m_sizeImage.cx, m_sizeImage.cy, 0, NULL, DI_NORMAL);

	//release the device context
	ReleaseDC(m_hWndDesktop, hDC);

	//increment the update counter and update if required
	if (++m_nUpdateHWndCounter > UPDATE_COUNTER_MAX) {
		FindDesktopHandle();
//		GetDesktopRect(m_rcBounds);
		SetRect(&m_rcBounds, 0, 0, GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1, GetSystemMetrics(SM_CYVIRTUALSCREEN) - 1);
	}
}

void CDesktopPet::Erase()
{
	//set up rect to be erased
	RECT rcErase = { m_ptOldPosition.x, m_ptOldPosition.y,
		m_ptOldPosition.x + m_sizeImage.cx, m_ptOldPosition.y + m_sizeImage.cy };

	//invalidate it and tell it to update
	InvalidateRect(m_hWndDesktop, &rcErase, TRUE);

	// Now, invalidate footprints
	for (const auto& rcFootprint : m_footprintRects) {
		InvalidateRect(m_hWndDesktop, &rcFootprint, TRUE);
	}
	// Clear the list after erasing
	m_footprintRects.clear();

	UpdateWindow(m_hWndDesktop);
}

void CDesktopPet::DrawOnTarget(int x, int y, HICON hIcon)
{
	// Get icon dimensions
	ICONINFO iconInfo;
	BITMAP bm;
	if (GetIconInfo(hIcon, &iconInfo)) {
		if (iconInfo.hbmMask) { // Check if there is a mask bitmap
			GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bm);
			DeleteObject(iconInfo.hbmMask); // Clean up mask bitmap
		} else { // Fallback if no mask bitmap (e.g. color icon with no mask)
			// If hbmColor exists, try to get dimensions from it
			// This case might happen for icons without a separate mask
			if (iconInfo.hbmColor) {
				GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);
			} else { // Default/fallback size if no bitmap info can be retrieved
				bm.bmWidth = 32; // Default width
				bm.bmHeight = 32; // Default height
			}
		}
		if (iconInfo.hbmColor) { // Clean up color bitmap if it exists
			DeleteObject(iconInfo.hbmColor);
		}

		// Construct a RECT for the footprint
		RECT rcFootprint = { x, y, x + bm.bmWidth, y + bm.bmHeight };

		// Add this rcFootprint to the m_footprintRects list
		m_footprintRects.push_back(rcFootprint);

		// Limit the number of footprints to 10
		if (m_footprintRects.size() > 10) {
			m_footprintRects.erase(m_footprintRects.begin());
		}
	}

	//grab the device context of the desktop
	HDC hDC = GetDC(m_hWndDesktop);

	//draw the icon on it
	DrawIconEx(hDC, x, y, hIcon, 0, 0, 0, NULL, DI_NORMAL);

	//release the device context
	ReleaseDC(m_hWndDesktop, hDC);
}

