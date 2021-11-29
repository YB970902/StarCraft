#include "stdafx.h"
#include "EffectTest.h"

HRESULT __stdcall EffectTest::Initialize(ID2D1EffectContext* effectContext, ID2D1TransformGraph* transformGraph)
{
    mpEffectContext = effectContext;
    
    ifstream file;
    file.open("Shader/ColorShader.fx", ios_base::binary);
    if (!file.is_open()) { return; }
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    vector<byte>data(fileSize, 0);

    file.read(reinterpret_cast<char*>(&data[0]), fileSize);
    file.close();

    effectContext->LoadPixelShader(TEST_Effect, reinterpret_cast<BYTE*>(&data[0]), static_cast<UINT32>(data.size()));
    //transformGraph->SetOutputNode(this);
	return E_NOTIMPL;
}

HRESULT __stdcall EffectTest::PrepareForRender(D2D1_CHANGE_TYPE changeType)
{
	return E_NOTIMPL;
}

HRESULT __stdcall EffectTest::SetGraph(ID2D1TransformGraph* transformGraph)
{
	return E_NOTIMPL;
}

ULONG __stdcall EffectTest::AddRef(void)
{
	++mRefCount;
	return 0;
}

ULONG __stdcall EffectTest::Release(void)
{
	--mRefCount;
	if (mRefCount == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return ++mRefCount;;
	}
}

HRESULT __stdcall EffectTest::QueryInterface(REFIID riid, void** ppvObject)
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