#pragma once
#include "FPoint.h"
class LineSegment2
{
public:
	FPoint m_cpStart, m_cpEnd;
	LineSegment2(FPoint Start,FPoint End);
	LineSegment2();
	~LineSegment2();

};

