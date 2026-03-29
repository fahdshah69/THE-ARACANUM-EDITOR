#include "../../Column.h"
#include "../../Line.h"

Column::Column(int maxLines, int maxCharsPerLine): maxLines(maxLines), lineCount(0), maxCharsPerLine(maxCharsPerLine) {
	lines = new Line * [maxLines];
}

Column::~Column() {
	for (int i = 0; i < lineCount; i++) {
		delete lines[i];
	}
	delete[] lines;
}

bool Column::hasRoom() {
	return lineCount < maxLines;
}

Line* Column::addLine() {
	if (hasRoom()) {
		Line* newLine = new Line(maxCharsPerLine);
		lines[lineCount++] = newLine;
		return newLine;
	}
	return nullptr;
}

void Column::draw(HDC hdc, int startX, int startY, int colWidth, int alignment,int lineHeight, const wchar_t* searchWord, int searchLen) {
	for (int i = 0; i < lineCount; i++) {
		lines[i]->draw(hdc, startX, startY, colWidth, alignment,searchWord, searchLen);
		startY += lineHeight;
	}
}