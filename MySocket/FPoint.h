#pragma once
#include "atltypes.h"
struct Pointf;
class FPoint :
	public CPoint
{
public:
	FPoint();
	FPoint(INT x, INT y);
	FPoint(const FPoint &point);
	FPoint(const Pointf &point);
	FPoint(const CPoint &point);
	DOUBLE Dot(const FPoint &point) const;
	FPoint Add(const FPoint &point);
	FPoint Sub(const FPoint &point);
	FPoint Mul(DOUBLE num);
	void SetPoint(DOUBLE x, DOUBLE y);
	void SetPoint(const FPoint &point);
	void SetPoint(const CPoint &point);
	void Offset(DOUBLE x, DOUBLE y);
	void Offset(FPoint &point);
	FPoint NorV();
	FPoint operator- (const FPoint& point) const;
	FPoint operator+ (const FPoint& point) const;
	DOUBLE operator* (const FPoint& point) const;
	DOUBLE Proj(const FPoint& line) const;
	DOUBLE Magnitude() const;
	Pointf Normalize() const;
	~FPoint();
};

