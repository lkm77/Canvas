#include "DrawImage.h"

namespace Draw {
	bool DrawImage::IsDraw()
	{
		return this->image->getwidth() && this->image->getheight();
	}
	void DrawImage::ReDraw(hiex::Canvas& canvas)
	{
		canvas.PutImageIn_Alpha(
			0, 0,
			this->image.get(),
			{ (0) },
			(BYTE)255U,
			true,
			true
		);
	}
	void DrawImage::SetImage(ImageUPtr& image)
	{
		this->image.reset(image.release());
	}
	void DrawImage::RemoveImage()
	{
		this->image.release();
	}
}