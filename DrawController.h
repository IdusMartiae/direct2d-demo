#pragma once

#include "BaseWindow.h"
#include "EllipseController.h"
#include "BitmapController.h"
#include "Physics.h"

class DrawController: public BaseWindow<DrawController>
{
	EllipseController*	pEllipse;
	BitmapController*	pBitmap;
	Physics*			pPhysics;

	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	
	

public:	
	BOOL timerRunning = TRUE;
	D2D1_POINT_2F pPoint;
	BOOL drawingMode = TRUE;

	PCWSTR ClassName() const { return WINDOW_CLASS_NAME; };

	DrawController() : pFactory(NULL), pRenderTarget(NULL), pEllipse(NULL), pBitmap(NULL) {};

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HRESULT Initialize();
	HRESULT CreateGraphicResources();
	
	void Update();
	void Resize();
	void DiscardGraphicResources();

};

