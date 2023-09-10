#pragma once
#include "../HiEasyX.h"
#include <fstream>
//#include <vector>
namespace Draw {
	bool operator==(const POINT& po1, const POINT& po2);
	enum DrawType
	{
		Base,Line, Image
	};
	
	//typedef unsigned int DrawType;
	class DrawBase
	{
	public:
		DrawBase();
		virtual ~DrawBase();
		DrawType GetDrawType();
		COLORREF GetLineColor();
		COLORREF GetFillColor();
		COLORREF GetBackgroundColor();
		void SetLineColor(COLORREF lineColor = SET_ALPHA(RGB(0, 0, 0), 255));
		void SetFillColor(COLORREF fillColor = SET_ALPHA(RGB(0, 0, 0), 255));
		void SetBackgroundColor(COLORREF backgroundColor = SET_ALPHA(RGB(0, 0, 0), 255));

		virtual bool IsDraw() = 0;
		virtual void ReDraw(hiex::Canvas& canvas) = 0;
	protected:
		void SetDrawType(DrawType drawType);
	private:
		DrawType drawType;
		COLORREF lineColor;
		COLORREF fillColor;
		COLORREF backgroundColor;
	};

	typedef std::unique_ptr <DrawBase> DrawBaseUPtr;
}

