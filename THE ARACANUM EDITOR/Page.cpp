#include "../../Page.h"
#include "../../Column.h"

Page::Page(int pageNumber, int maxColumns, int maxLinesPerCol, int maxCharsPerLine) {
	this->pageNumber = pageNumber;
	this->maxColumns = maxColumns;
	this->maxLinesPerCol = maxLinesPerCol;
	this->maxCharsPerLine = maxCharsPerLine;
	columnCount = 0;

	columns = new Column * [maxColumns];
}

Page::~Page() {
	for (int i = 0; i < columnCount; i++)
		delete columns[i];
	delete[] columns;
}

bool Page::hasRoom() {
	return columnCount < maxColumns;
}

Column* Page::addColumn() {
	if (hasRoom()) {
		Column* newColumn = new Column(maxLinesPerCol, maxCharsPerLine);
		columns[columnCount++] = newColumn;
		return newColumn;
	}

	return nullptr;
}

void convertTochar(wchar_t* pageNumberStr, int pageNumber, int &length) {
	pageNumberStr[0] = L'-';
	pageNumberStr[1] = L' ';

	int charCount = 2;
	while (pageNumber > 0) {
		pageNumberStr[charCount++] = pageNumber % 10 + L'0';
		pageNumber /= 10;
	}
	int i = 2;
	length = charCount + 2;
	int tempCharCount = charCount;
	charCount--;
	while (i < charCount) {
		wchar_t temp = pageNumberStr[i];
		pageNumberStr[i++] = pageNumberStr[charCount];
		pageNumberStr[charCount--] = temp;
	}
	
	pageNumberStr[tempCharCount++] = L' ';
	pageNumberStr[tempCharCount++] = L'-';
	pageNumberStr[tempCharCount] = L'\0';
}

void Page::draw(HDC hdc, RECT rect, int startX, int startY, int fontHeight, int colWidth, int alignment,const wchar_t* searchWord, int searchLen) {
	for (int i = 0; i < columnCount; i++) {
		columns[i]->draw(hdc, startX, startY, colWidth, alignment,fontHeight,searchWord, searchLen);
		startX += colWidth;
	}

	wchar_t pageNumberStr[10];
	int pageStrLength = 0;
	convertTochar(pageNumberStr, pageNumber + 1, pageStrLength); // +1 b/c pageCount starts at 0
	
	TextOutW(hdc, rect.right / 2 - 20, rect.bottom - 80, pageNumberStr, pageStrLength);

}

