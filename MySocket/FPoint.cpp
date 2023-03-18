#include "stdafx.h"
#include "FPoint.h"


FPoint::FPoint()
{
}

FPoint::FPoint(INT x, INT y)
	:CPoint(x,y)
{
}

FPoint::FPoint(const FPoint &point)
	:CPoint(point.x,point.y)
{
}

FPoint::FPoint(const Pointf & point)
{
	x = point.x;
	y = point.y;
}

FPoint::FPoint(const CPoint & point)
{
	x = point.x;
	y = point.y;
}

DOUBLE FPoint::Dot(const FPoint &point) const
{
	return x*point.x + y*point.y;
}

FPoint FPoint::Add(const FPoint &point)
{
	x += point.x;
	y += point.y;
	return FPoint(x, y);
}

FPoint FPoint::Sub(const FPoint &point)
{
	x -= point.x;
	y -= point.y;
	return FPoint(x, y);
}

FPoint FPoint::Mul(DOUBLE num)
{
	x *= num;
	y *= num;
	return FPoint(x, y);
}

void FPoint::SetPoint(DOUBLE _x, DOUBLE _y)
{
	x = _x;
	y = _y;
}

void FPoint::SetPoint(const FPoint & point)
{
	x = point.x;
	y = point.y;
}

void FPoint::SetPoint(const CPoint & point)
{
	x = point.x;
	y = point.y;
}

void FPoint::Offset(DOUBLE _x, DOUBLE _y)
{
	x += _x;
	y += _y;
}

void FPoint::Offset(FPoint & point)
{
	x += point.x;
	y += point.y;
}

FPoint FPoint::NorV()
{
	double temp = x;
	x = -y;
	y = temp;
	return FPoint(x,y);
}

FPoint FPoint::operator-(const FPoint & point) const
{
	return FPoint(x - point.x, y - point.y);
}

FPoint FPoint::operator+(const FPoint & point) const
{
	return FPoint(x + point.x, y + point.y);
}

DOUBLE FPoint::operator*(const FPoint & point) const
{
	return x*point.y - y*point.x;
}

DOUBLE FPoint::Proj(const FPoint & line) const
{
	return DotMulti(*this, line) / line.Magnitude();
}

DOUBLE FPoint::Magnitude() const
{
	return sqrt(x*x + y*y);
}



Pointf FPoint::Normalize() const
{
	DOUBLE n = Magnitude();
	return Pointf(x/n,y/n);
}


FPoint::~FPoint()
{
}
