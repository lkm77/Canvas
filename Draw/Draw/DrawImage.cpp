#include "DrawImage.h"

namespace Draw {
	bool DrawImage::IsDraw()
	{
		return this->image.getheight() || this->image.getwidth();
	}
	void DrawImage::ReDraw(hiex::Canvas& canvas)
	{
		canvas.PutImageIn_Alpha(
			0, 0,
			&this->image
		);
	}
	void DrawImage::SetImage(IMAGE& image)
	{
		this->image = image;
	}
	void DrawImage::SaveData(std::wofstream& out)
	{
	}
	void DrawImage::LoadData(std::wifstream& in)
	{
	}
}