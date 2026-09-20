#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

const std::string globalEscapePrefix = "\x1b[";
const char globalEscapeSuffix = 'm';
const int globalBackgroundCodeOffset = 10;

enum Color
{
	Black = 30,
	Red = 31,
	green = 32,
	yellow = 33,
	blue = 34,
	magenta = 35,
	cyan = 36,
	white = 37
};

enum Format
{
	Bold = 1,
	Dim = 2,
	Italic = 3,
	Underline = 4,
	Blink = 5,
	inverse = 7
};

enum Alignment
{
	Left,
	Centre,
	Right
};

std::string CalculatePadding(std::string aText, enum Alignment aAlignment, int aLeftPadding)
{
	std::string padding;

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	int columns = info.srWindow.Right - info.srWindow.Left + 1;
	int neededPadding = 0;

	if (aAlignment == Alignment::Centre)
	{
		int columnsLeftOfCentre = (aText.length() / 2) + 1;
		neededPadding = (columns / 2) - columnsLeftOfCentre;
	}
	else if (aAlignment == Alignment::Right)
	{
		int columnsForText = aText.length();
		neededPadding = columns - columnsForText;
	}

	for (int i = 0; i < neededPadding + aLeftPadding; i++)
	{
		padding += ' ';
	}
	return padding;
}

std::string Format(std::string aText, enum Color aForegroundColor = Color::white, enum Color aBackgroundColor = Color::Black, std::vector<enum Format> aFormat = {}, int aLeftPadding = 0)
{
	std::string formattedText;

	for (int i = 0; i < aLeftPadding; i++)
	{
		formattedText += ' ';
	}

	// Text formatting
	formattedText += globalEscapePrefix;
	formattedText += std::to_string(aForegroundColor);
	formattedText += ';' + std::to_string(aBackgroundColor + globalBackgroundCodeOffset);
	if (aFormat.size() > 0)
	{
		for (enum Format format : aFormat)
		{
			formattedText += ';' + std::to_string(format);
		}
	}
	formattedText += globalEscapeSuffix + aText;

	// Remove formatting for following text
	formattedText += globalEscapePrefix + std::to_string(0) + globalEscapeSuffix;
	return formattedText;
}

void Print(std::string aText, enum Color aForegroundColor, enum Color aBackgroundColor, std::vector<enum Format> aFormat, enum Alignment aAlignment, int aLeftPadding = 0)
{
	std::string formattedText = Format(aText, aForegroundColor, aBackgroundColor, aFormat, aLeftPadding);
	std::cout << CalculatePadding(aText, aAlignment, 0) + formattedText << std::endl;
}

