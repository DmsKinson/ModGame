
// stdafx.cpp : source file that includes just the standard includes
// MySocket.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

INT DotMulti(const FPoint & Pt1, const FPoint & Pt2)
{
	return Pt1.x*Pt2.x + Pt1.y*Pt2.y;
}

FPoint ScalarMulti(const FPoint & Pt1, DOUBLE Num)
{
	return FPoint(Pt1.x*Num,Pt1.y*Num);
}

BOOL SegmentIntersect(FPoint p1, FPoint p2, FPoint p3, FPoint p4)
{
	DOUBLE d1 = Direction(p3, p4, p1);
	DOUBLE d2 = Direction(p3, p4, p2);
	DOUBLE d3 = Direction(p1, p2, p3);
	DOUBLE d4 = Direction(p1, p2, p4);
	if (d1*d2 < 0 && d3*d4 < 0)
		return TRUE;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return TRUE;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return TRUE;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return TRUE;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return TRUE;
	else
		return FALSE;
}

DOUBLE Direction(FPoint p1, FPoint p2, FPoint p3)
{
	return (p3 - p1)*(p2 - p1);
}

BOOL OnSegment(FPoint p1, FPoint p2, FPoint p3)
{
	return (min(p1.x, p2.x) <= p3.x && max(p1.x, p2.x) >= p3.x && min(p1.y, p2.y) <= p3.y && max(p1.y, p2.y) >= p3.y);
}

FPoint PointSub(const FPoint & pt1, const FPoint & pt2)
{
	return FPoint(pt1.x - pt2.x, pt1.y - pt2.y);
}

FPoint PointAdd(const FPoint & pt1, const FPoint & pt2)
{
	return FPoint(pt1.x+pt2.x,pt1.y+pt2.y);
}



Pointf::Pointf()
{
	x = y = 0.0;
}

Pointf::Pointf(const Pointf & point)
{
	x = point.x;
	y = point.y;
}

Pointf::Pointf(const FPoint & point)
{
	x = point.x;
	y = point.y;
}

Pointf::Pointf(DOUBLE _x, DOUBLE _y)
{
	x = _x;
	y = _y;
}

Pointf Pointf::Mul(DOUBLE num)
{
	x *= num;
	y *= num;
	return Pointf(x, y);
}

Pointf Pointf::Add(const Pointf & point)
{
	x += point.x;
	y += point.y;
	return Pointf(x, y);
}

Pointf Pointf::Sub(const Pointf & point)
{
	x -= point.x;
	y -= point.y;
	return Pointf(x, y);
}

DOUBLE Pointf::Dot(const Pointf & point)
{
	return x*point.x + y*point.y;
}

Pointf Pointf::Normalize()
{
	DOUBLE n = Magnitude();
	x /= n;
	y /= n;
	return Pointf(x, y);
}

DOUBLE Pointf::Magnitude()
{
	return sqrt(x*x + y*y);
}


