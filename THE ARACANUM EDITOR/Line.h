#pragma once
#include <Windows.h>

class Line {
public:
    wchar_t* text; // The actual words to draw on this line
    int length; // Number of characters on this line
    int maxChars; // The strict limit (e.g., 40)

    Line(int maxChars);
    ~Line();

    // Returns true if the word fits, false if it overflows
    bool canFitWord(int wordLength);

    // Adds a word (and a space) to this line's text array
    void addWord(const wchar_t* word, int wordLength);

    // The GDI drawing function (uses TextOutW/ExtTextOutW)
    void draw(HDC hdc, int x, int y, int colWidth, int alignment, const wchar_t* searchWord, int searchLen);
};