#include "DrawController.h"

LRESULT DrawController::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_KEYDOWN:
		pPhysics->HandleCommand(uMsg, wParam);
		drawingMode == TRUE ? pEllipse->Update(pPoint.x, pPoint.y) : pBitmap->Update(pPoint.x, pPoint.y);
		break;

	case WM_KEYUP:
		if (wParam == VK_SPACE)
		{
			drawingMode = !drawingMode;
			drawingMode == TRUE ? pEllipse->Update(pPoint.x, pPoint.y) : pBitmap->Update(pPoint.x, pPoint.y);
		}
		else {
			pPhysics->HandleCommand(uMsg, wParam);
		}
		break;

	case WM_MOUSEWHEEL:
		pPhysics->HandleCommand(uMsg, wParam);
		break;

	case WM_PAINT:
		drawingMode == TRUE ? pEllipse->Update(pPoint.x, pPoint.y) : pBitmap->Update(pPoint.x, pPoint.y);
		break;

	case WM_CREATE:
		if (FAILED(Initialize()))
		{
			return -1;
		}
		break;

	case WM_SIZE:
		Resize();
		break;

	case WM_DESTROY:
		timerRunning = FALSE;
		pEllipse->~EllipseController();
		pBitmap->~BitmapController();
		DiscardGraphicResources();
		PostQuitMessage(0);	
		break;

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);

	}
	return 0;
}

HRESULT DrawController::Initialize()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
	CreateGraphicResources();

	pPoint = D2D1::Point2F(pRenderTarget->GetSize().width / 2, pRenderTarget->GetSize().height / 2);

	pPhysics = new Physics(m_hwnd, &drawingMode, &pPoint);

	pEllipse = new EllipseController(pFactory, pRenderTarget, m_hwnd);
	if (pEllipse == NULL) { hr = E_FAIL; }
	pEllipse->Initialize(pPoint.x, pPoint.y);

	pBitmap = new BitmapController(pRenderTarget, m_hwnd);
	if (pBitmap == NULL ) { hr = E_FAIL; }
	pBitmap->Initialize();

	return hr;
}

HRESULT DrawController::CreateGraphicResources()
{
	HRESULT result = S_OK;
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		result = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);
	}
	return result;
}

void DrawController::Update()
{
	pPhysics->simulateInertia();
	drawingMode == TRUE ?
		pEllipse->Update(pPoint.x, pPoint.y) : pBitmap->Update(pPoint.x, pPoint.y);
}

void DrawController::Resize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);

		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void DrawController::DiscardGraphicResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pFactory);
}


