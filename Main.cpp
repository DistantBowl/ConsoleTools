#include "ConsoleTools.h"

class NewMenu : public Menu
{
public:
	using Menu::Menu;
	void RenderBody() override
	{
		Print("Hello World!", Color::White, Color::Black, { Format::Underline }, Alignment::Centre);
		std::cout << '\n';
	}
	void CallButtonMethod(int aOption) override
	{
		switch (aOption)
		{
		case 0:
			Print("YESSS", Color::Green, Color::Black, { Format::Bold }, Alignment::Centre);
			break;
		case 1:
			Print("NOOO", Color::Red, Color::Black, { Format::Bold }, Alignment::Centre);
			break;
		}
	}
};

int main()
{
	
	NewMenu menu(GridSize{3,2}, 
		{ "Yes", "No", "Maybe", "Never", "Quit" }, 
		Color::Blue, 
		{ Format::Underline, Format::Italic, Format::Bold }, 
		Alignment::Centre);
	menu.InputLoop();
}