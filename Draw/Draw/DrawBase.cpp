#include "DrawBase.h"
namespace Draw {
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
	void DrawBase::Save(std::wofstream& out)
	{
		out << this->GetDrawType() << " ";
		out << this->GetLineColor() << " ";
		out << this->GetFillColor() << " ";
		out << this->GetBackgroundColor() << " ";
		this->SaveData(out);
	}
	void DrawBase::Load(std::wifstream& in)
	{
		//不处理类型,外部需要根据类型构造类
		COLORREF liCo;
		in >> liCo;
		this->SetLineColor(liCo);
		COLORREF fiCo;
		in >> fiCo;
		this->SetFillColor(fiCo);
		COLORREF baCo;
		in >> baCo;
		this->SetBackgroundColor(baCo);
		this->LoadData(in);
	}
	void DrawBase::SetDrawType(DrawType drawType)
	{
		this->drawType = drawType;
	}
}