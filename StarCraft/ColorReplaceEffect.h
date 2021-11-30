#pragma once

DEFINE_GUID(GUID_ColorReplacePixelShader, 0x628404f9, 0xea5e, 0x4795, 0x8f, 0xf5, 0xdb, 0x71, 0x91, 0x38, 0x51, 0x8e);
DEFINE_GUID(CLSID_CustomColorReplaceEffect, 0x6f91336b, 0xa6f0, 0x464d, 0x9b, 0x8f, 0x77, 0xcd, 0xef, 0x45, 0xec, 0x3);

const D2D_VECTOR_3F EFFECT_COLOR_RED = { 1.0f, 0.0f, 0.0f };
const D2D_VECTOR_3F EFFECT_COLOR_GREEN = { 0.0f, 1.0f, 0.0f };
const D2D_VECTOR_3F EFFECT_COLOR_BLUE = { 0.0f, 0.0f, 1.0f };
const D2D_VECTOR_3F EFFECT_COLOR_YELLOW = { 1.0f, 1.0f, 0.0f };
const D2D_VECTOR_3F EFFECT_COLOR_EMERALD = { 1.0f, 0.0f, 1.0f };
const D2D_VECTOR_3F EFFECT_COLOR_PURPLE = { 0.5f, 0.0f, 1.0f };
const D2D_VECTOR_3F EFFECT_COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
const D2D_VECTOR_3F EFFECT_COLOR_BLACK = { 0.1f, 0.1f, 0.1f };


enum class eColorReplaceProperty
{
	ROW = 0,
};

class ColorReplaceEffect : public ID2D1EffectImpl, public ID2D1DrawTransform
{
private:
	ColorReplaceEffect();
	ID2D1DrawInfo* mpDrawInfo = nullptr;
	ID2D1EffectContext* mpEffectContext = nullptr;
	LONG mRefCount = 0;
	D2D1_RECT_L mInputRect;
	D2D1_RECT_L mInputLUTRect;
	float mDpi;
	ID2D1TransformGraph* mpTransformGraph = nullptr;

	struct
	{
		D2D_VECTOR_3F row;
	} mConstants;
public:
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);

	static HRESULT __stdcall CreateColorReplaceImpl(_Outptr_ IUnknown** ppEffectImpl);

	virtual HRESULT __stdcall Initialize(ID2D1EffectContext* effectContext, ID2D1TransformGraph* transformGraph) override;
	virtual HRESULT __stdcall PrepareForRender(D2D1_CHANGE_TYPE changeType) override;
	virtual HRESULT __stdcall SetGraph(ID2D1TransformGraph* transformGraph) override;

	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;

	virtual UINT32 __stdcall GetInputCount() const override;
	virtual HRESULT __stdcall MapOutputRectToInputRects(const D2D1_RECT_L* outputRect, D2D1_RECT_L* inputRects, UINT32 inputRectsCount) const override;
	virtual HRESULT __stdcall MapInputRectsToOutputRect(const D2D1_RECT_L* inputRects, const D2D1_RECT_L* inputOpaqueSubRects, UINT32 inputRectCount, D2D1_RECT_L* outputRect, D2D1_RECT_L* outputOpaqueSubRect) override;
	virtual HRESULT __stdcall MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, D2D1_RECT_L* invalidOutputRect) const override;
	virtual HRESULT __stdcall SetDrawInfo(ID2D1DrawInfo* drawInfo) override;

	HRESULT SetRow(D2D_VECTOR_3F row);
	D2D_VECTOR_3F GetRow() const;
};

