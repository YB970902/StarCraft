#include "stdafx.h"
#include "ColorReplaceEffect.h"
#include "EffectReader.h"

#define XML(X) TEXT(#X)

ColorReplaceEffect::ColorReplaceEffect()
    : mRefCount{ 1 }
{
    
}

ColorReplaceEffect::~ColorReplaceEffect()
{
    
}

HRESULT ColorReplaceEffect::Register(ID2D1Factory1* pFactory)
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
                <Property name='groupColor' type='vector3'>
                    <Property name='DisplayName' type='string' value='groupColor'/>
                    <Property name='Default' type='vector3' value='(1.0, 0.0, 0.0)'/>
                </Property>
            </Effect>
        );

    const D2D1_PROPERTY_BINDING bindings[] =
    {
       D2D1_VALUE_TYPE_BINDING(TEXT("groupColor"), &SetGroupColor, &GetGroupColor),
    };

    return pFactory->RegisterEffectFromString(
        GUID_ColorReplacePixelShader,
        pszXml,
        bindings,
        ARRAYSIZE(bindings),
        CreateColorReplaceImpl
    );
}

HRESULT __stdcall ColorReplaceEffect::CreateColorReplaceImpl(IUnknown** ppEffectImpl)
{
    *ppEffectImpl = static_cast<ID2D1EffectImpl*>(new (std::nothrow) ColorReplaceEffect());

    if (*ppEffectImpl == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    else
    {
        return S_OK;
    }
}

HRESULT __stdcall ColorReplaceEffect::Initialize(ID2D1EffectContext* effectContext, ID2D1TransformGraph* transformGraph)
{
    mpEffectContext = effectContext;
    
    BYTE* pData = nullptr;
    DWORD dwBytes = 0;
    EffectReader::LoadEffectFromFile("ColorReplaceShader.cso", &pData, &dwBytes);

    effectContext->LoadPixelShader(GUID_ColorReplacePixelShader, pData, dwBytes);
    delete[] pData;

    transformGraph->SetSingleTransformNode(this);

	return S_OK;
}

HRESULT __stdcall ColorReplaceEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType)
{
    SetGraph(mpTransformGraph);
    
	return mpDrawInfo->SetPixelShaderConstantBuffer(reinterpret_cast<BYTE*>(&mConstants), sizeof(mConstants));
}

HRESULT __stdcall ColorReplaceEffect::SetGraph(ID2D1TransformGraph* transformGraph)
{
	return E_NOTIMPL;
}

ULONG __stdcall ColorReplaceEffect::AddRef(void)
{
	++mRefCount;
	return 0;
}

ULONG __stdcall ColorReplaceEffect::Release(void)
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

HRESULT __stdcall ColorReplaceEffect::QueryInterface(REFIID riid, void** ppvObject)
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

UINT32 __stdcall ColorReplaceEffect::GetInputCount() const
{
    return 1;
}

HRESULT __stdcall ColorReplaceEffect::MapOutputRectToInputRects(const D2D1_RECT_L* outputRect, D2D1_RECT_L* inputRects, UINT32 inputRectsCount) const
{
    if (inputRectsCount != 1)
    {
        return E_INVALIDARG;
    }
    
    inputRects[0] = *outputRect;
    return S_OK;
}

HRESULT __stdcall ColorReplaceEffect::MapInputRectsToOutputRect(const D2D1_RECT_L* inputRects, const D2D1_RECT_L* inputOpaqueSubRects, UINT32 inputRectCount, D2D1_RECT_L* outputRect, D2D1_RECT_L* outputOpaqueSubRect)
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

HRESULT __stdcall ColorReplaceEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, D2D1_RECT_L* invalidOutputRect) const
{
    *invalidOutputRect = mInputRect;

    return S_OK;
}

HRESULT __stdcall ColorReplaceEffect::SetDrawInfo(ID2D1DrawInfo* drawInfo)
{
    mpDrawInfo = drawInfo;
    //mpEffectContext->GetDpi(&mDpi, &mDpi);

    return drawInfo->SetPixelShader(GUID_ColorReplacePixelShader);
}

HRESULT ColorReplaceEffect::SetGroupColor(D2D_VECTOR_3F row)
{
    mConstants.groupColor = row;
    return S_OK;
}

D2D_VECTOR_3F ColorReplaceEffect::GetGroupColor() const
{
    return mConstants.groupColor;
}
