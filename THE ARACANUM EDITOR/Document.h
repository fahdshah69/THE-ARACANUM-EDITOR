#pragma once
#include "Page.h"

class Page;

class Document {
private:
    wchar_t* textBuffer;
    int capacity;
    int length;
    int spaces;

    wchar_t searchTerms[5][100];
    int searchIndex;

    Page** pages;          // Dynamic array of Page pointers
    int pageCapacity;        // Total allocated size of the pages array
    int pageCount;           // How many pages currently exist
    
    int currentMaxColumns;   // e.g., 2
    int currentMaxLines;     // e.g., 20
    int currentMaxChars;     // e.g., 40

    void expandBuffer(int requiredExtraSpace);

public:
    // Constructor initializes the default layout state and allocates initial memory
    Document();

    // Destructor cleanly cascades deletion to prevent memory leaks
    ~Document();

    // textBuffer modifiers mostly. search ftns have different functionality
    void insertCharacter(wchar_t c);
    void insertText(const wchar_t* pastedText, int pasteLength);
    void backspace();
    void search(wchar_t c);
    bool isSearching;
    wchar_t searchBuffer[100];
    int searchBufferLength;
    int alignment;

    // Destroys the View (Pages/Cols/Lines) and rebuilds them from textBuffer using curr layout
    void reflow();

    int getPageCount() { return pageCount; }

    int getLength() { return length; }

    void wordSentenceCount(int& outWords, int& outSentences)const;

    Page* getPage(int index) {
        return pages[index];
    }

    void draw(HDC hdc, RECT rect);
};