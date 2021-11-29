#pragma once
DEFINE_GUID(TEST_Effect, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01);

class EffectTest : public ID2D1EffectImpl
{
private:
	EffectTest();
	LONG mRefCount = 0;
	ID2D1EffectContext* mpEffectContext = nullptr;
public:
	virtual HRESULT __stdcall Initialize(ID2D1EffectContext* effectContext, ID2D1TransformGraph* transformGraph) override;
	virtual HRESULT __stdcall PrepareForRender(D2D1_CHANGE_TYPE changeType) override;
	virtual HRESULT __stdcall SetGraph(ID2D1TransformGraph* transformGraph) override;

	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
};

