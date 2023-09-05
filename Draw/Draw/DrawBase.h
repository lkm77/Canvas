#pragma once
#include "../HiEasyX.h"
#include <fstream>
//#include <vector>
namespace Draw {
	enum DrawType
	{
		Base,Line
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
		void SetLineColor(COLORREF lineColor = RGB(0, 0, 0));
		void SetFillColor(COLORREF fillColor = RGB(0, 0, 0));
		void SetBackgroundColor(COLORREF backgroundColor = RGB(0, 0, 0));

		virtual bool IsDraw() = 0;
		virtual void ReDraw(hiex::Canvas& canvas) = 0;
		void Save(std::wofstream& out);
		void Load(std::wifstream& in);
	protected:
		void SetDrawType(DrawType drawType);
		virtual void SaveData(std::wofstream& out) = 0;
		virtual void LoadData(std::wifstream& in) = 0;
	//protected:
	private:
		DrawType drawType;
		COLORREF lineColor;
		COLORREF fillColor;
		COLORREF backgroundColor;
	};

	typedef std::unique_ptr <DrawBase> DrawBaseUPtr;
}

