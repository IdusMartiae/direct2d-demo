#include "BitmapController.h"

void BitmapController::getFrame()
{
    HRESULT hr = pFactory->CreateDecoderFromFilename(
        FILE_NAME,                      
        NULL,                            
        GENERIC_READ,                    
        WICDecodeMetadataCacheOnDemand,  
        &pDecoder                        
    );

    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }
    return;
}

void BitmapController::scaleImage()
{
    HRESULT hr = pFactory->CreateBitmapScaler(&pIScaler);

    UINT pHeight, pWidth, scaleRate = SCALE_SIZE;
    pFrame->GetSize(&pWidth, &pHeight);

    if (SUCCEEDED(hr))
    {
        hr = pIScaler->Initialize(
            pFrame,                            
            pWidth / scaleRate,                
            pHeight / scaleRate,               
            WICBitmapInterpolationModeFant);   
    }
    return;
}

void BitmapController::ConvertPicture()
{        
    HRESULT hr = pFactory->CreateFormatConverter(&pConverter);

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            1.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (pConverter && !pBitmap)
    {
        pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &pBitmap);
    }

    return;
}

void BitmapController::Initialize()
{
    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pFactory)
    );

    getFrame();
    scaleImage();
    ConvertPicture();
}

void BitmapController::Update(FLOAT x, FLOAT y)
{
    D2D1_RECT_F rc;
    D2D1_SIZE_F size = D2D1::SizeF(PIC_WIDTH, PIC_HEIGHT);
    D2D1_SIZE_F renderSize = pRenderTarget->GetSize();
    FLOAT scaleSize = SCALE_SIZE;

    rc.left = x - (size.width / scaleSize) / 2;
    rc.right = x + (size.width / scaleSize) / 2;
    rc.bottom = y - (size.height / scaleSize) / 2;
    rc.top = y + (size.height / scaleSize) / 2;

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
    pRenderTarget->DrawBitmap(
        pBitmap, 
        &rc, 
        1.0f, 
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
    );
    pRenderTarget->EndDraw();

    return;
}

void BitmapController::DiscardResources()
{
    SafeRelease(&pFactory);
    SafeRelease(&pDecoder);
    SafeRelease(&pFrame);
    SafeRelease(&pIScaler);
    SafeRelease(&pConverter);
    SafeRelease(&pBitmap);
}

