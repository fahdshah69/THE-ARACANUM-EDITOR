#pragma once
#include "../../Column.h"

class Page {
public:
	int pageNumber;
	Column** columns;
	int columnCount;
	int maxColumns;
	int maxLinesPerCol;
	int maxCharsPerLine;

	Page(int pageNumber, int maxColumns, int maxLinesPerCol, int maxCharsPerLine);
	~Page();

	bool hasRoom();
	Column* addColumn();

	void draw(HDC hdc, RECT rect, int startX, int startY, int fontHeight, int colWidth, int alignment, const wchar_t* searchWord, int searchLen);
};