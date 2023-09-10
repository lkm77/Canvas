#include "DrawBase.h"
namespace Draw {
	bool operator==(const POINT& po1, const POINT& po2)
	{
		if (po1.x != po2.x)return false;
		if (po1.y != po2.y)return false;
		return true;
	}
	
	DrawBase::DrawBase()
	{
		this->SetDrawType(Base);
		this->SetBackgroundColor();
		this->SetFillColor();
		this->SetLineColor();
	}
	DrawBase::~DrawBase()
	{
	}
	DrawType DrawBase::GetDrawType()
	{
		return this->drawType;
	}
	COLORREF DrawBase::GetLineColor()
	{
		return this->lineColor;
	}
	COLORREF DrawBase::GetFillColor()
	{
		return this->fillColor;
	}
	COLORREF DrawBase::GetBackgroundColor()
	{
		return this->backgroundColor;
	}
	void DrawBase::SetLineColor(COLORREF lineColor)
	{
		this->lineColor = lineColor;
	}
	void DrawBase::SetFillColor(COLORREF fillColor)
	{
		this->fillColor = fillColor;
	}
	void DrawBase::SetBackgroundColor(COLORREF backgroundColor)
	{
		this->backgroundColor = backgroundColor;
	}
	void DrawBase::SetDrawType(DrawType drawType)
	{
		this->drawType = drawType;
	}
}