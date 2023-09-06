#include "DrawFactory.h"

namespace Draw {
	DrawBaseUPtr Draw::DrawFactory(DrawType DrTy)
	{
		DrawBaseUPtr drBaUPtr;
		switch (DrTy)
		{
		case Draw::Line:
			drBaUPtr.reset(new DrawLine);
			break;
		case Draw::Image:
			drBaUPtr.reset(new DrawImage);
			break;
		default:
			break;
		}
		return std::move(drBaUPtr);
	}
}
