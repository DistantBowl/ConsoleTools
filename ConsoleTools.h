#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

enum Color
{
	Black = 30,
	Red = 31,
	Green = 32,
	Yellow = 33,
	Blue = 34,
	Magenta = 35,
	Cyan = 36,
	White = 37
};
enum Format
{
	Bold = 1,
	Dim = 2,
	Italic = 3,
	Underline = 4,
	Blink = 5,
	Inverse = 7
};
enum Alignment
{
	Left,
	Centre,
	Right
};

struct GridSize
{
	int Columns;
	int Rows;
};

void Print(std::string aText, enum Color aForegroundColor, enum Color aBackgroundColor, std::vector<enum Format> aFormat, enum Alignment aAlignment, int aLeftPadding = 0);

std::string Format(std::string aText, enum Color aForegroundColor, enum Color aBackgroundColor, std::vector<enum Format> aFormat, int aLeftPadding = 0);

std::string CalculatePadding(std::string aText, enum Alignment aAlignment, int aLeftPadding);

class Menu
{
private:
	GridSize myButtonGridSize;
	std::vector<std::string> myOptions;
	enum Color myButtonColor;
	std::vector<enum Format> mySelectedFormatting;
	enum Alignment myAlignment;

public:
	Menu(GridSize aButtonGridSize, std::vector<std::string> aOptions, enum Color aButtonColor, std::vector<enum Format> aSelectedFormatting, enum Alignment aAlignment)
	{
		myButtonGridSize = aButtonGridSize;
		myOptions = aOptions;
		myButtonColor = aButtonColor;
		mySelectedFormatting = aSelectedFormatting;
		myAlignment = aAlignment;
	}
	
	virtual ~Menu() = default;

	virtual void RenderBody() = 0;
	
	void RenderButtons(const int &aPositionIndex)
	{
		for (int y = 0; y < myButtonGridSize.Rows; y++)
		{
			// Unformatted line is used to calculate the padding for the formatted line
			std::string newFormattedLine;
			std::string newUnformattedLine;
			for (int x = 0; x < myButtonGridSize.Columns; x++)
			{
				int index = x + (y * myButtonGridSize.Columns);

				// Skip if the index is out of bounds of the options vector
				if (myOptions.size() <= index) continue;

				if (index == aPositionIndex)
				{
					newFormattedLine += Format(myOptions[index], myButtonColor, Color::Black, mySelectedFormatting, x > 0 ? 5 : 0);
				}
				else
				{
					newFormattedLine += Format(myOptions[index], myButtonColor, Color::Black, {}, x > 0 ? 5 : 0);
				}
				
				// Makes sure padding is only added to the unformatted line if it's not the first button in the row
				if (x > 0)
				{
					newUnformattedLine += CalculatePadding(myOptions[index], Alignment::Left, 5);
				}

				newUnformattedLine +=  myOptions[index];
			}
			std::cout << CalculatePadding(newUnformattedLine, myAlignment, 0) + newFormattedLine << std::endl;
		}
	}
	
	void RenderFrame(int &aSelectedIndex)
	{
		std::cout << "\x1b[H";
		RenderBody();
		RenderButtons(aSelectedIndex);
		std::cout.flush();
	}

	virtual void CallButtonMethod(int aOption) = 0;

	bool  MoveCursorLeft(int& positionIndex)
	{
		if (positionIndex % myButtonGridSize.Columns != 0)
		{
			positionIndex--;
			return true;
		}
		return false;
	}

	bool MoveCursorUp(int & positionIndex)
	{
		if (positionIndex > (myButtonGridSize.Columns - 1))
		{
			positionIndex -= myButtonGridSize.Columns;
			return true;
		}
		return false;
	}

	bool MoveCursorRight(int& positionIndex)
	{
		if ((positionIndex + 1) % myButtonGridSize.Columns != 0 && (positionIndex + 1) < myOptions.size())
		{
			positionIndex++;
			return true;
		}
		return false;
	}

	bool MoveCursorDown(int& positionIndex)
	{
		if (positionIndex < (myOptions.size() - myButtonGridSize.Columns))
		{
			positionIndex += myButtonGridSize.Columns;
			return true;
		}
		return false;
	}

	void InputLoop()
	{
		bool selecting = true;
		int position = 0;
		RenderFrame(position);
		while (selecting)
		{
			INPUT_RECORD inputRecord;
			DWORD numRead;
			ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &inputRecord, 1, &numRead);

			bool moved = false;
			if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
			{
				switch (inputRecord.Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_LEFT:
					moved = MoveCursorLeft(position);
					break;
				case VK_UP:
					moved = MoveCursorUp(position);
					break;
				case VK_RIGHT:
					moved = MoveCursorRight(position);
					break;
				case VK_DOWN:
					moved = MoveCursorDown(position);
					break;
				case VK_RETURN:
					selecting = false;
					break;
				}

				if (moved)
				{
					RenderFrame(position);
				}
			}
		}

		CallButtonMethod(position);
	}
};