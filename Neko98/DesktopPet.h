// DesktopPet.h: interface for the CDesktopPet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKTOPPET_H__A69EBAA3_385D_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_DESKTOPPET_H__A69EBAA3_385D_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector> // Added for std::vector
#include "Pet.h"
#include "DesktopHack.h"

class CDesktopPet : public CDesktopHack, virtual public CPet {
public:
	CDesktopPet();
	virtual ~CDesktopPet();

	virtual void DrawOnTarget(int x, int y, HICON hIcon);

protected:
	int m_nUpdateHWndCounter;
	std::vector<RECT> m_footprintRects; // Added to store footprint rectangles
	virtual void Erase();
	virtual void Draw(int nImage);
};

#endif // !defined(AFX_DESKTOPPET_H__A69EBAA3_385D_11D2_9FF9_00001C192944__INCLUDED_)
