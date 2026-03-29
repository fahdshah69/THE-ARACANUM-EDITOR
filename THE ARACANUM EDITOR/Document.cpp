#include "../../Document.h"
#include "../../Page.h"

Document::Document(): capacity(1024), length(0), spaces(0), pageCapacity(10), pageCount(0), currentMaxColumns(2), currentMaxLines(20), currentMaxChars(40), searchIndex(0), isSearching(false), searchBufferLength(0), alignment(0) {
	textBuffer = new wchar_t[capacity];
	pages = new Page*[pageCapacity];
}

Document::~Document() {
	
	for(int i = 0; i < pageCount; i++) 
		delete pages[i];
	delete[] pages;
	
	delete[] textBuffer;
}

void Document::expandBuffer(int requiredExtraSpace) {

	int newCapacity = capacity;
	while (newCapacity <= length + requiredExtraSpace)
		newCapacity *= 2;

	wchar_t* newTextBuffer = new wchar_t[newCapacity];
	for (int i = 0; i < length; i++) 
		newTextBuffer[i] = textBuffer[i];

	delete[] textBuffer;
	textBuffer = newTextBuffer;
	capacity = newCapacity;
}

void Document::insertCharacter(wchar_t c) {
	if (length + 1 >= capacity)
		expandBuffer(1);

	if (c == L'\b') { backspace(); return; }
	else if (c == L' ') {
		spaces++;
		//wordCount++; // test purp
		//if (isWord) {
		//	wordCount++;
		//	isWord = false;
		//}
	}
	//else if (c == L'\n') wordCount++;
	//else if (c == L'.' || c == L'?' || c == L'!') {
	//	if (isSentence) {
	//		sentenceCount++;
	//		isSentence = false;
	//	}
	//}
	//else {
	//	isWord = true;
	//	isSentence = true;
	//}

	textBuffer[length++] = c;
}

void Document::insertText(const wchar_t* pastedText, int pasteLength) {
	if(length + pasteLength >= capacity)
		expandBuffer(pasteLength);

	for (int i = 0; i < pasteLength; i++) {
		textBuffer[length + i] = pastedText[i];
		if (pastedText[i] == L' ')
			spaces++;
	}
	length += pasteLength;
}

void Document::reflow() {
	for (int i = 0; i < pageCount; i++)
		delete pages[i];
	
	pageCount = 0;

	Page* activePage = new Page(pageCount, currentMaxColumns, currentMaxLines, currentMaxChars);
	pages[pageCount++] = activePage;
	
	Column* activeColumn = activePage->addColumn();
	Line* activeLine = activeColumn->addLine();

	int bufferIndex = 0;
	while (bufferIndex < length) {

		int wordStartIndex = bufferIndex;
		while (bufferIndex - wordStartIndex < currentMaxChars && bufferIndex < length && textBuffer[bufferIndex] != L' ' && textBuffer[bufferIndex] != L'\n')
			bufferIndex++;
		int wordLength = bufferIndex - wordStartIndex;

		if (bufferIndex < length && textBuffer[bufferIndex] == L' ')
			bufferIndex++;

		bool isNewLine = false;
		if (wordLength == 0 && bufferIndex < length && textBuffer[bufferIndex] == L'\n') {
			isNewLine = true;
			bufferIndex++;
		}

		if (!activeLine->canFitWord(wordLength) || isNewLine) {
			if (activeColumn->hasRoom())
				activeLine = activeColumn->addLine();
			else
				if (activePage->hasRoom()) {
					activeColumn = activePage->addColumn();
					activeLine = activeColumn->addLine();
				}
				else {
					if (pageCount >= pageCapacity) {
						pageCapacity *= 2;

						Page** newPages = new Page * [pageCapacity];
						for (int i = 0; i < pageCount; i++)
							newPages[i] = pages[i];
						
						delete[] pages;
						pages = newPages;
					}
					activePage = new Page(pageCount, currentMaxColumns, currentMaxLines, currentMaxChars);
					pages[pageCount++] = activePage;
					activeColumn = activePage->addColumn();
					activeLine = activeColumn->addLine();
				}
		}
		activeLine->addWord(&textBuffer[wordStartIndex], wordLength);
	}

}


void convertToWchar(const wchar_t* prefix, wchar_t* string, const wchar_t* postfix, int number, int& len) {

	int preCount = 0;
	while (prefix[preCount] != L'\0')
		preCount++;

	for (int i = 0; i < preCount; i++)
		string[i] = prefix[i];

	if (number == 0) {
		string[preCount++] = L'0';
		string[preCount] = L'\0';
		len = preCount;
	}
	int right = preCount;
	while (number > 0) {
		string[right++] = number % 10 + L'0';
		number /= 10;
	}
	int left = preCount;
	int tempCharCount = right;
	right--;
	while (left < right) {
		wchar_t temp = string[left];
		string[left++] = string[right];
		string[right--] = temp;
	}

	int i = 0;
	while (postfix[i] != L'\0')
		string[tempCharCount++] = postfix[i++];

	string[tempCharCount] = L'\0';
	len = tempCharCount;
}

void Document::draw(HDC hdc, RECT rect) {
	int yPos = rect.bottom - 40;

	// Chars no spaces
	wchar_t spacesCountStr[100];
	int spacesCountLength = 0;
	convertToWchar(L"Chars (no spaces): ", spacesCountStr, L"", length - spaces, spacesCountLength);
	TextOutW(hdc, 50, yPos, spacesCountStr, spacesCountLength);

	TextOutW(hdc, 300, yPos, L"|", 1);

	// Chars
	wchar_t charCountStr[100];
	int charsCountLength = 0;
	convertToWchar(L"Chars: ", charCountStr, L"", length, charsCountLength);
	TextOutW(hdc, 330, yPos, charCountStr, charsCountLength);

	TextOutW(hdc, 480, yPos, L"|", 1);

	int wordCount, sentenceCount;
	wordSentenceCount(wordCount, sentenceCount);

	// Words
	wchar_t wordCountStr[100];
	int wordCountLength = 0;
	convertToWchar(L"Words: ", wordCountStr, L"", wordCount, wordCountLength);
	TextOutW(hdc, 510, yPos, wordCountStr, wordCountLength);

	TextOutW(hdc, 660, yPos, L"|", 1);

	// Sentences
	wchar_t sentenceCountStr[100];
	int sentenceCountLength = 0;
	convertToWchar(L"Sentences: ", sentenceCountStr, L"", sentenceCount, sentenceCountLength);
	TextOutW(hdc, 690, yPos, sentenceCountStr, sentenceCountLength);

	TextOutW(hdc, 870, yPos, L"|", 1);

	// Estim read time
	wchar_t estMinStr[100];
	int estMinLength = 0;
	convertToWchar(L"Est read time: ", estMinStr, L" minutes", wordCount / 200, estMinLength);
	TextOutW(hdc, 900, yPos, estMinStr, estMinLength);

	if (isSearching) {
		TextOutW(hdc, 50, rect.bottom - 80, L"Search: ", 8);
		TextOutW(hdc, 150, rect.bottom - 80, searchBuffer, searchBufferLength);

		TextOutW(hdc, 50, rect.bottom - 120, L"--- Recent Searches ---", 23);

		int historyY = rect.bottom - 150;

		for (int i = 0; i < 5; i++) {
			int termLen = 0;
			while (searchTerms[i][termLen] != L'\0') {
				termLen++;
			}

			if (termLen > 0) {
				TextOutW(hdc, 50, historyY, searchTerms[i], termLen);
				historyY -= 30;
			}
		}
	}
}

void Document::backspace() {
	if (length <= 0) return;
	if (length > 0 && textBuffer[length - 1] == L' ')
		spaces--;

	length--;
}

bool isWordCharacter(wchar_t c) {
	if (c >= L'A' && c <= L'Z') return true;
	if (c >= L'a' && c <= L'z') return true;
	if (c >= L'0' && c <= L'9') return true;
	return false;
}

void Document::wordSentenceCount(int& wordCount, int& sentenceCount) const {
	wordCount = 0;
	sentenceCount = 0;

	bool isWord = false;
	bool isSentence = false;

	for (int i = 0; i < length; i++) {
		wchar_t c = textBuffer[i];

		if (isWordCharacter(c)) {
			isWord = true;
			isSentence = true;
		}
		else if (c == L'.' || c == L'?' || c == L'!') {
			if (isSentence) {
				sentenceCount++;
				isSentence = false;
			}
			if (isWord) {
				wordCount++;
				isWord = false;
			}
		}
		else if (c == L' ' || c == L'\n' || c == L'\t') {
			if (isWord) {
				wordCount++;
				isWord = false;
			}
		}
	}

	// user hasn't pressed space or enter. still typed a word though
	if (isWord) {
		wordCount++;
	}
	if (isSentence) {
		sentenceCount++;
	}
}

void Document::search(wchar_t c) {
	if (c == 27) {
		isSearching = false;
		return;
	}

	if (c == L'\r') {
		isSearching = false;

		if (searchBufferLength > 0) {
			int i = 0;
			while (i <= searchBufferLength) { // <= for the \0
				searchTerms[searchIndex][i] = searchBuffer[i];
				i++;
			}

			searchIndex++;
			if (searchIndex > 4) searchIndex = 0;
		}
		return;
	}

	if (c == L'\b') {
		if (searchBufferLength > 0) {
			searchBufferLength--;
			searchBuffer[searchBufferLength] = L'\0';
		}
		return;
	}

	if (searchBufferLength < 99) {
		searchBuffer[searchBufferLength++] = c;
		searchBuffer[searchBufferLength] = L'\0';
	}
}