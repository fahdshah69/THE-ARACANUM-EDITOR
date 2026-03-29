#pragma once
#include "Line.h"

class Column {
public:
	int maxLines;
	Line** lines;
	int lineCount;
	int maxCharsPerLine;

	Column(int maxLines, int maxCharsPerLine);
	~Column();

	bool hasRoom();
	Line* addLine();

	void draw(HDC hdc, int startX, int startY, int colWidth, int alignment, int lineHeight, const wchar_t* searchWord, int searchLen);
};