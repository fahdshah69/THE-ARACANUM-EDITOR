#include "../../Line.h"
#include "../../Document.h";

Line::Line(int maxChars): length(0), maxChars(maxChars) {
	text = new wchar_t[maxChars];
}

Line::~Line() {
	delete[] text;
}

bool Line::canFitWord(int wordLength) {
	if (length + wordLength <= maxChars)
		return true;
	return false;
}

void Line::addWord(const wchar_t* word, int wordLength) {
	if (canFitWord(wordLength)) {
		for (int i = length; i < length + wordLength; i++) {
			text[i] = word[i - length];
		}
		//Document::incrementWord();
		length += wordLength;
		if(canFitWord(1))
			text[length++] = L' ';
	}
}

int findSubstring(const wchar_t* str, const wchar_t* sub) {
    int i, j, k;

    for (i = 0; str[i] != L'\0'; i++) {
		wchar_t a = str[i], b = sub[0];
        if (a == b || a + 32 == b || a - 32 == b) {
            for (j = 0, k = i; sub[j] != L'\0' && str[k] != L'\0'; j++, k++) {
                if (str[k] + 32 != sub[j] && str[k] - 32 != sub[j] && str[k] != sub[j]) {
                    break;
                }
            }
            if (sub[j] == L'\0') {
                return i;
            }
        }
    }

    return -1;
}

void Line::draw(HDC hdc, int x, int y, int colWidth, int alignment, const wchar_t* searchWord, int searchLen) {
    int drawX = x;
    SIZE fullLineSize;
    GetTextExtentPoint32W(hdc, text, length, &fullLineSize);

    int extraPixels = colWidth - fullLineSize.cx;

    int totalSpaces = 0;
    for (int i = 0; i < length; i++) {
        if (text[i] == L' ') totalSpaces++;
    }

    if (alignment == 1) { // center
        drawX = x + (extraPixels / 2);
        TextOutW(hdc, drawX, y, text, length);
    }
    else if (alignment == 2) { // right
        drawX = x + extraPixels;
        TextOutW(hdc, drawX, y, text, length);
    }
    else if (alignment == 3 && totalSpaces > 0 && extraPixels > 0) {
        int basePadding = extraPixels / totalSpaces; // pixels to add on each space
        int remainder = extraPixels % totalSpaces; // leftover pixels. not divisible on spaceCount

        int currentX = x;
        int wordStart = 0;
        int spacesUsed = 0;

        SIZE spaceSize;
        GetTextExtentPoint32W(hdc, L" ", 1, &spaceSize);

        for (int i = 0; i <= length; i++) {
            if (i == length || text[i] == L' ') {
                int wordLen = i - wordStart;

                if (wordLen > 0) {
                    TextOutW(hdc, currentX, y, &text[wordStart], wordLen); // print current word

                    SIZE wordSize;
                    GetTextExtentPoint32W(hdc, &text[wordStart], wordLen, &wordSize);
                    currentX += wordSize.cx;
                }
                
                if (i < length) {
                    currentX += spaceSize.cx + basePadding;
                    if (spacesUsed < remainder) { // after basePadding, remainder pixels also need to be inserted one by one
                        currentX += 1;
                    }
                    spacesUsed++;
                }
                wordStart = i + 1; 
            }
        }
    }
    else { // default left
        TextOutW(hdc, drawX, y, text, length);
    }

    if (searchWord == nullptr || searchLen == 0) return;

    SetBkMode(hdc, OPAQUE);

    wchar_t originalChar = text[length];
    text[length] = L'\0';

    int currIdx = 0;

    while (currIdx < length) {
        int subIdx = findSubstring(&text[currIdx], searchWord);
        if (subIdx == -1) break;

        int absoluteIdx = currIdx + subIdx;

        SIZE prefixSize;
        GetTextExtentPoint32W(hdc, text, absoluteIdx, &prefixSize); // part before subIdx

        int xOffset = prefixSize.cx;

        if (alignment == 3 && totalSpaces > 0 && extraPixels > 0) {
            int prefixSpaces = 0;
            for (int i = 0; i < absoluteIdx; i++) {
                if (text[i] == L' ') prefixSpaces++;
            }

            int basePadding = extraPixels / totalSpaces;
            int remainder = extraPixels % totalSpaces;

            // Add the exact amount of padding pixels that occurred before this word
            int paddingPixels = (prefixSpaces * basePadding) + (prefixSpaces < remainder ? prefixSpaces : remainder);
            xOffset += paddingPixels;
        }

        SetBkColor(hdc, RGB(255, 255, 0));
        SetTextColor(hdc, RGB(0, 0, 0));

        TextOutW(hdc, (alignment == 3 ? x : drawX) + xOffset, y, &text[absoluteIdx], searchLen);

        currIdx = absoluteIdx + searchLen;
    }

    text[length] = originalChar;
    SetBkColor(hdc, RGB(20, 20, 40));
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
}