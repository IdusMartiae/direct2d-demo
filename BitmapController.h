#pragma once

#include "Common.h"

class BitmapController
{
	
	ID2D1HwndRenderTarget* pRenderTarget;
	HWND				   m_hwnd;

	IWICImagingFactory*    pFactory;
	IWICBitmapDecoder*	   pDecoder;
	IWICBitmapFrameDecode* pFrame;

	IWICBitmapScaler*	   pIScaler;
	IWICFormatConverter*   pConverter;

	ID2D1Bitmap*		   pBitmap;

	void getFrame();
	void scaleImage();
	void ConvertPicture();
	void DiscardResources();

public:
	BitmapController(ID2D1HwndRenderTarget* RenderTarget, HWND hwnd)
		: pFactory(NULL), pDecoder(NULL), pFrame(NULL), pIScaler(NULL), pConverter(NULL), pBitmap(NULL)
	{
		pRenderTarget = RenderTarget;
		m_hwnd = hwnd;
	}

	void Initialize();
	void Update(FLOAT x, FLOAT y);

	~BitmapController()
	{
		DiscardResources();
	}
};

