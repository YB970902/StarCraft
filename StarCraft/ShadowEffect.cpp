#include "stdafx.h"
#include "ShadowEffect.h"
#include "EffectReader.h"

#define XML(X) TEXT(#X)

ShadowEffect::ShadowEffect()
    : mRefCount{ 1 }
{

}

ShadowEffect::~ShadowEffect()
{

}

HRESULT ShadowEffect::Register(ID2D1Factory1* pFactory)
{
    PCWSTR pszXml =
        XML(
            <?xml version = '1.0'?>
            <Effect>
                <!--System Properties-->
                <Property name='DisplayName' type='string' value='ColorReplace'/>
                <Property name='Author' type='string' value='Yeong bin'/>
                <Property name='Category' type='string' value='Stylize'/>
                <Property name='Description' type='string' value='Color replace'/>
                <Inputs>
                    <Input name='Source'/>
                </Inputs>
                <!--Custom Properties go here-->
            </Effect>
        );


    return pFactory->RegisterEffectFromString(
        GUID_ShadowPixelShader,
        pszXml,
        nullptr,
        0,
        CreateShadowImpl
    );
}

HRESULT __stdcall ShadowEffect::CreateShadowImpl(IUnknown** ppEffectImpl)
{
    *ppEffectImpl = static_cast<ID2D1EffectImpl*>(new (std::nothrow) ShadowEffect());

    if (*ppEffectImpl == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    else
    {
        return S_OK;
    }
}

HRESULT __stdcall ShadowEffect::Initialize(ID2D1EffectContext* effectContext, ID2D1TransformGraph* transformGraph)
{
    mpEffectContext = effectContext;

    BYTE* pData = nullptr;
    DWORD dwBytes = 0;
    EffectReader::LoadEffectFromFile("ShadowShader.cso", &pData, &dwBytes);

    effectContext->LoadPixelShader(GUID_ShadowPixelShader, pData, dwBytes);
    delete[] pData;

    transformGraph->SetSingleTransformNode(this);

    return S_OK;
}

HRESULT __stdcall ShadowEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType)
{
    SetGraph(mpTransformGraph);

    return S_OK;
}

HRESULT __stdcall ShadowEffect::SetGraph(ID2D1TransformGraph* transformGraph)
{
    return E_NOTIMPL;
}

ULONG __stdcall ShadowEffect::AddRef(void)
{
    ++mRefCount;
    return 0;
}

ULONG __stdcall ShadowEffect::Release(void)
{
    --mRefCount;
    if (mRefCount == 0)
    {
        delete this;
        return 0;
    }
    else
    {
        return mRefCount;
    }
}

HRESULT __stdcall ShadowEffect::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = nullptr;
    HRESULT hr = S_OK;

    if (riid == __uuidof(ID2D1EffectImpl))
    {
        *ppvObject = reinterpret_cast<ID2D1EffectImpl*>(this);
    }
    else if (riid == __uuidof(ID2D1DrawTransform))
    {
        *ppvObject = (ID2D1DrawTransform*)this;
    }
    else if (riid == __uuidof(ID2D1Transform))
    {
        *ppvObject = (ID2D1Transform*)this;
    }
    else if (riid == __uuidof(ID2D1TransformNode))
    {
        *ppvObject = (ID2D1TransformNode*)this;
    }
    else if (riid == __uuidof(IUnknown))
    {
        *ppvObject = this;
    }
    else
    {
        hr = E_NOINTERFACE;
    }

    if (*ppvObject != nullptr)
    {
        AddRef();
    }

    return hr;
}

UINT32 __stdcall ShadowEffect::GetInputCount() const
{
    return 1;
}

HRESULT __stdcall ShadowEffect::MapOutputRectToInputRects(const D2D1_RECT_L* outputRect, D2D1_RECT_L* inputRects, UINT32 inputRectsCount) const
{
    if (inputRectsCount != 1)
    {
        return E_INVALIDARG;
    }

    inputRects[0] = *outputRect;
    return S_OK;
}

HRESULT __stdcall ShadowEffect::MapInputRectsToOutputRect(const D2D1_RECT_L* inputRects, const D2D1_RECT_L* inputOpaqueSubRects, UINT32 inputRectCount, D2D1_RECT_L* outputRect, D2D1_RECT_L* outputOpaqueSubRect)
{
    if (inputRectCount != 1)
    {
        return E_INVALIDARG;
    }

    *outputRect = inputRects[0];
    mInputRect = inputRects[0];

    ZeroMemory(outputOpaqueSubRect, sizeof(*outputOpaqueSubRect));
    return S_OK;
}

HRESULT __stdcall ShadowEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, D2D1_RECT_L* invalidOutputRect) const
{
    *invalidOutputRect = mInputRect;

    return S_OK;
}

HRESULT __stdcall ShadowEffect::SetDrawInfo(ID2D1DrawInfo* drawInfo)
{
    mpDrawInfo = drawInfo;
    //mpEffectContext->GetDpi(&mDpi, &mDpi);

    return drawInfo->SetPixelShader(GUID_ShadowPixelShader);
}
