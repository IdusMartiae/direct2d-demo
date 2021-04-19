#pragma once

#include "Common.h"

class EllipseController
{
	HWND m_hwnd;

	ID2D1Factory*					pFactory;
	ID2D1HwndRenderTarget*			pRenderTarget;
	
	ID2D1SolidColorBrush*			pBrush;
	ID2D1SolidColorBrush*			pOutlineBrush;
	ID2D1StrokeStyle*				pOutline;
	D2D1_ELLIPSE					ellipse;
	D2D1_STROKE_STYLE_PROPERTIES	pOutlineProp;

	HRESULT CreateGraphicResources();
	void InitializeEllipse(FLOAT x, FLOAT y);
	void DiscardGraphicResources();
	
	

public:
	EllipseController(
		ID2D1Factory* Factory,
		ID2D1HwndRenderTarget* RenderTarget,
		HWND hwnd
		) 
	{
		pFactory = Factory;
		pRenderTarget = RenderTarget;
		m_hwnd = hwnd;
	};

	void Initialize(FLOAT x, FLOAT y);

	void Update(FLOAT x, FLOAT y);

	~EllipseController()
	{
		DiscardGraphicResources();
	}
	
};

