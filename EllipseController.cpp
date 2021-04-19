#include "EllipseController.h"

void EllipseController::Initialize(FLOAT x, FLOAT y)
{
	CreateGraphicResources();
	InitializeEllipse(x, y);
}

HRESULT EllipseController::CreateGraphicResources()
{
	HRESULT result = S_OK;

	const D2D1_COLOR_F color = D2D1::ColorF(CIRCLE_BRUSH_COLOR);
	result = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

	if (SUCCEEDED(result))
	{
		const D2D1_COLOR_F color = D2D1::ColorF(CIRCLE_OUTLINE_COLOR);
		result = pRenderTarget->CreateSolidColorBrush(color, &pOutlineBrush);

		if (SUCCEEDED(result))
		{
			pOutlineProp.dashStyle = D2D1_DASH_STYLE_SOLID;
			result = pFactory->CreateStrokeStyle(pOutlineProp, NULL, 0, &pOutline);
		}
	}
	return result;
}

void EllipseController::InitializeEllipse(FLOAT x, FLOAT y)
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), CIRCLE_RADIUS, CIRCLE_RADIUS);
	}
	return;
}

// Прорисовка клиентской области
void EllipseController::Update(FLOAT x, FLOAT y)
{
	ellipse.point.x = x;
	ellipse.point.y = y;

	PAINTSTRUCT ps;
	BeginPaint(m_hwnd, &ps);

	pRenderTarget->BeginDraw();

	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
	pRenderTarget->FillEllipse(ellipse, pBrush);
	pRenderTarget->DrawEllipse(ellipse, pOutlineBrush, STROKE_WIDTH, pOutline);

	HRESULT result = pRenderTarget->EndDraw();

	if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
	{
		DiscardGraphicResources();
	}

	EndPaint(m_hwnd, &ps);
	return;
}

void EllipseController::DiscardGraphicResources()
{
	SafeRelease(&pBrush);
	SafeRelease(&pOutlineBrush);
	SafeRelease(&pOutline);
	return;
}

