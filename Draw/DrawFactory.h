#pragma once
#include "Draw/DrawLine.h"
#include "Draw/DrawImage.h"

namespace Draw {
	DrawBaseUPtr DrawFactory(DrawType DrTy);
}