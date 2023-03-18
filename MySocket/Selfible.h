#pragma once
class Selfible
{
public:
	void virtual WINAPI UpdateSelf() = 0;
	void virtual WINAPI PaintSelf(CDC *dc) = 0;
	DataPack virtual WINAPI PackSelf() = 0;
};

