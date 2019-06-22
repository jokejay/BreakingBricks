#include <allegro5/color.h>
#include <memory>

#include "Collider.hpp"
#include "Point.hpp"

namespace Engine {
	bool Collider::IsPointInBitmap(Point pnt, const std::shared_ptr<ALLEGRO_BITMAP>& bmp) {
		return al_get_pixel(bmp.get(), static_cast<int>(pnt.x), static_cast<int>(pnt.y)).a != 0;
	}
	bool Collider::IsPointInRect(Point pnt, Point rectPos, Point rectSize) {
		return (rectPos.x <= pnt.x && pnt.x < rectPos.x + rectSize.x && rectPos.y <= pnt.y && pnt.y < rectPos.y + rectSize.y);
	}
	bool Collider::IsRectOverlap(Point r1Min, Point r1Max, Point r2Min, Point r2Max) {
		return (r1Max.x > r2Min.x && r2Max.x > r1Min.x && r1Max.y > r2Min.y && r2Max.y > r1Min.y);
	}
	bool Collider::IsCircleOverlap(Point c1, float r1, Point c2, float r2) {
		return (c1 - c2).Magnitude() < r1 + r2;
	}
	bool Collider::IsCircleOverlapRect(Point c1, float r1, Point rect, float rw, float rh) {
		//TBD: determine whether a circle overlap rectangle
		float testx = c1.x;
		float testy = c1.y;
		float ry = rect.y;
		float rx = rect.x;

		if (c1.x < rx)				testx = rx;
		else if (c1.x > rx + rw)	testx = rx + rw;
		if (c1.y < ry)				testy = ry;
		else if (c1.y > ry + rh)	testy = ry + rh;

		float distX = c1.x - testx;
		float distY = c1.y - testy;
		float distance = sqrt( (distX * distX) + (distY * distY) );

		if (distance <= r1) {
			return true;
		}

		return false;
	}
}
