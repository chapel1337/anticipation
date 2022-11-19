#include <iostream>
#include <vector>
#include <conio.h>
#include <random>
#include <windows.h>
#include <string>

using std::cout, std::cin, std::string, std::vector, std::to_string, std::stoi;

// written by chapel1337
// made on 11/18/2022
// was originally supposed to be minesweeper, became a degraded knockoff version
// not much to say about this

void clear()
{
	system("cls");
}

std::mt19937 mt{ std::random_device{}() };
int getRandom(int minimum, int maximum)
{
	std::uniform_int_distribution random{ minimum, maximum };

	return random(mt);
}

void title()
{
	SetConsoleTitleA("anticipation");
}

void title(string title)
{
	SetConsoleTitleA(("anticipation - " + title).c_str());
}

void title(string title, int i)
{
	SetConsoleTitleA(("anticipation - " + title + to_string(i)).c_str());
}

void setTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetTextColor()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

vector<vector<char>> map{};
vector<vector<bool>> revealedTiles{};

int mapY{ 5 };
int mapX{ 5 };

int selectedTileY{};
int selectedTileX{};

int tilesRevealed{};
int lives{ 1 };
bool directions{ true };

void start(int position);
void menu(int position);
void refresh();
void generateMap();
void getInput();

void displayStats()
{
	// cout << "map y: " << mapY << '\n';
	// cout << "map x: " << mapX << "\n\n";

	// cout << "selected tile y: " << selectedTileY << '\n';
	// cout << "selected tile x: " << selectedTileX << "\n\n";

	cout << "tiles revealed: " << tilesRevealed << "\n\n";
	cout << "lives: " << lives << "\n\n";
}

void youWin()
{
	clear();
	title("you win!");

	setTextColor(2);
	cout << "you win!\n\n";
	resetTextColor();

	_getch();

	generateMap();
	refresh();
	getInput();
}

void youLose()
{
	clear();
	title("you lose!");
	displayStats();

	cout << "| ";
	for (int i{}; i < mapX; ++i)
	{
		cout << '-';
	}
	cout << " |\n";

	for (unsigned int i{}; i < map.size(); ++i)
	{
		cout << "| ";
		for (unsigned int o{}; o < map[i].size(); ++o)
		{
			if (i == selectedTileY && o == selectedTileX)
			{
				setTextColor(3);
				cout << map[i][o];
				resetTextColor();
			}
			else if (map[i][o] == '!')
			{
				setTextColor(12);
				cout << map[i][o];
				resetTextColor();
			}
			else if (map[i][o] == '*')
			{
				setTextColor(2);
				cout << map[i][o];
				resetTextColor();
			}
			else
			{
				cout << '#';
			}
		}

		cout << " |\n";
	}

	cout << "| ";
	for (int i{}; i < mapX; ++i)
	{
		cout << '-';
	}
	cout << " |\n";

	_getch();

	generateMap();
	refresh();
	getInput();
}

void generateMap()
{
	map.clear();
	revealedTiles.clear();
	tilesRevealed = 0;

	vector<char> line{};
	vector<bool> revealedLine{};

	for (int i{}; i < mapY; ++i)
	{
		for (int o{}; o < mapX; ++o)
		{
			/*
			if (getRandom(1, 5) == 2)
			{
				line.push_back('!');
			}
			else if (getRandom(1, 8) == 2)
			{
				line.push_back('*');
			}
			else
			{
				line.push_back(' ');
			}
			*/

			line.push_back(' ');

			revealedLine.push_back(false);
		}

		revealedTiles.push_back(revealedLine);
		map.push_back(line);

		line.clear();
		revealedLine.clear();
	}

	selectedTileX = static_cast<int>(floor(mapX / 2));
	selectedTileY = static_cast<int>(floor(mapY / 2));

	if (map[selectedTileY][selectedTileX] == '!' || map[selectedTileY][selectedTileX] == '*')
	{
		map[selectedTileY][selectedTileX] = ' ';
	}
}

void refresh()
{
	clear();
	displayStats();

	if (tilesRevealed == mapY * mapX)
	{
		youWin();
	}

	cout << "| ";
	for (int i{}; i < mapX; ++i)
	{
		cout << '-';
	}
	cout << " |\n";

	for (unsigned int i{}; i < map.size(); ++i)
	{
		cout << "| ";

		for (unsigned int o{}; o < map[i].size(); ++o)
		{
			if (i == selectedTileY && o == selectedTileX)
			{
				if (map[selectedTileY][selectedTileX] == ' ' && revealedTiles[selectedTileY][selectedTileX])
				{
					setTextColor(8);
					cout << '_';
					resetTextColor();
				}
				else
				{
					setTextColor(8);
					cout << '#';
					resetTextColor();
				}
			}
			else if (map[i][o] == '*' && revealedTiles[i][o])
			{
				setTextColor(2);
				cout << map[i][o];
				resetTextColor();
			}
			else if (map[i][o] == '!' && revealedTiles[i][o])
			{
				setTextColor(12);
				cout << map[i][o];
				resetTextColor();
			}
			else if (revealedTiles[i][o])
			{
				cout << map[i][o];
			}
			else
			{
				cout << '#';
			}
		}

		cout << " |\n";
	}

	cout << "| ";
	for (int i{}; i < mapX; ++i)
	{
		cout << '-';
	}
	cout << " |\n";
}

void revealTile()
{
	if (map[selectedTileY][selectedTileX] == '!')
	{
		if (lives == 1)
		{
			youLose();
		}
		else
		{
			--lives;
			revealedTiles[selectedTileY][selectedTileX] = true;
		}
	}
	else if (map[selectedTileY][selectedTileX] == '*')
	{
		++lives;
		revealedTiles[selectedTileY][selectedTileX] = true;
	}
	else if (!revealedTiles[selectedTileY][selectedTileX])
	{
		revealedTiles[selectedTileY][selectedTileX] = true;
		
		/*
		while (selectedTileX != mapX && map[selectedTileY][selectedTileX + 1] != ' ' && selectedTileX != 0 && map[selectedTileY][selectedTileX - 1] != ' '  && selectedTileX != mapY && map[selectedTileY + 1][selectedTileX] != ' ' && selectedTileY != 0 && map[selectedTileY - 1][selectedTileX] != ' ')
		{
			selectedTileY = getRandom(1, mapY - 1);
			selectedTileX = getRandom(1, mapX - 1);
		}
		*/
	}

	++tilesRevealed;
}

void positionSelectedTile(const char direction)
{
	if (direction == 'w' && selectedTileY != 0)
	{
		--selectedTileY;
	}
	else if (direction == 's' && selectedTileY != mapY - 1)
	{
		++selectedTileY;
	}
	else if (direction == 'a' && selectedTileX != 0)
	{
		--selectedTileX;
	}
	else if (direction == 'd' && selectedTileX != mapX - 1)
	{
		++selectedTileX;
	}
}

string mapYString{ "5" };
string mapXString{ "5" };

void customMap(int position)
{
	clear();

	mapY = stoi(mapYString);
	mapX = stoi(mapXString);

	cout << "  | ";
	for (int i{}; i < mapX; ++i)
	{
		cout << '-';
	}
	cout << " |\n";

	for (int i{}; i < mapY; ++i)
	{
		cout << "  | ";

		for (int o{}; o < mapX; ++o)
		{
			cout << '#';
		}

		cout << " |\n";
	}

	cout << "  | ";
	for (int i{}; i < mapX; ++i)
	{
		cout << '-';
	}
	cout << " |\n";

	if (position == 1)
	{
		cout << "  > map y: " << mapY << '\n';
	}
	else
	{
		cout << "  map y: " << mapY << '\n';
	}

	if (position == 2)
	{
		cout << "  > map x: " << mapX << '\n';
	}
	else
	{
		cout << "  map x: " << mapX << '\n';
	}

	int keyInput{ _getch() };

	if (keyInput == 'W' && position != 0 || keyInput == 'w' && position != 0)
	{
		customMap(position - 1);
	}
	else if (keyInput == 'S' && position != 5 || keyInput == 's' && position != 5)
	{
		customMap(position + 1);
	}
	else if (keyInput == 13)
	{
		generateMap();
		refresh();
		getInput();
	}

	// hell
	if (position == 1 && keyInput == '1' || position == 1 && keyInput == '2' || position == 1 && keyInput == '3' || position == 1 && keyInput == '4' || position == 1 && keyInput == '5' || position == 1 && keyInput == '6' || position == 1 && keyInput == '7' || position == 1 && keyInput == '8' || position == 1 && keyInput == '9' || position == 1 && keyInput == '0')
	{
		mapYString.push_back(keyInput);
		customMap(position);
	}
	else if (position == 1 && keyInput == 8)
	{
		if (mapYString.length() == 1)
		{
			mapYString[0] = '0';
		}
		else
		{
			mapYString.pop_back();
		}
		customMap(position);
	}

	// hell part 2
	if (position == 2 && keyInput == '1' || position == 2 && keyInput == '2' || position == 2 && keyInput == '3' || position == 2 && keyInput == '4' || position == 2 && keyInput == '5' || position == 2 && keyInput == '6' || position == 2 && keyInput == '7' || position == 2 && keyInput == '8' || position == 2 && keyInput == '9' || position == 2 && keyInput == '0')
	{
		mapXString.push_back(keyInput);
		customMap(position);
	}
	else if (position == 2 && keyInput == 8)
	{
		if (mapXString.length() == 1)
		{
			mapXString[0] = '0';
		}
		else
		{
			mapXString.pop_back();
		}
		customMap(position);
	}
}

void start(int position)
{
	clear();
	title();

	cout << "  select difficulty\n\n";

	if (position == 1)
	{
		cout << "  > easy <\n";
	}
	else
	{
		cout << "  easy\n";
	}

	if (position == 2)
	{
		cout << "  > medium <\n";
	}
	else
	{
		cout << "  medium\n";
	}

	if (position == 3)
	{
		cout << "  > hard <\n";
	}
	else
	{
		cout << "  hard\n";
	}

	if (position == 4)
	{
		cout << "  > impossible <\n";
	}
	else
	{
		cout << "  impossible\n";
	}

	if (position == 5)
	{
		cout << "  > custom <\n";
	}
	else
	{
		cout << "  custom\n";
	}

	const int keyInput{ _getch() };

	if (keyInput == 'W' && position != 0 || keyInput == 'w' && position != 0)
	{
		start(position - 1);
	}
	else if (keyInput == 'S' && position != 5 || keyInput == 's' && position != 5)
	{
		start(position + 1);
	}
	else if (keyInput == 13)
	{
		switch (position)
		{
		case 1:
			mapY = 5;
			mapX = 5;
			break;

		case 2:
			mapY = 10;
			mapX = 10;
			break;

		case 3:
			mapY = 15;
			mapX = 20;
			break;

		case 4:
			mapY = 18;
			mapX = 45;
			break;

		case 5:
			customMap(1);
			break;
		}

		selectedTileY = static_cast<int>(floor(mapY / 2));
		selectedTileX = static_cast<int>(floor(mapX / 2));
	}

	if (directions)
	{
		clear();

		cout << "  press esc to stop playing\n";
		cout << "  press h for help\n\n";

		Sleep(2000);
		directions = false;
	}

	generateMap();
	refresh();
	getInput();
}

void help()
{
	clear();

	cout << "  help\n\n";

	cout << "  this is a game of luck\n";
	cout << "  to win, reveal all of the hidden tiles\n";
	cout << "  some hidden tiles have mines, which deplete your life by 1\n";
	cout << "  some hidden tiles also have health packs, which give you +1 life\n";
	cout << "  if you die, all of the hidden tiles will be revealed\n";
	cout << "  have fun!\n\n";

	cin.get();
	refresh();
}

void credits()
{
	clear();
	title("credits");

	cout << "  anticipation\n\n";

	cout << "  written by chapel1337\n";
	cout << "  made on 11/18/2022\n\n";

	cout << "  inspired by minesweeper (made by microsoft)\n\n";

	cin.get();
	menu(1);
}

void quit()
{
	clear();

	for (int i{ 3 }; i > 0; --i)
	{
		title("okay, quitting in ", i);
		cout << "  okay, exiting in " << i;

		Sleep(1000);
		clear();
	}

	Beep(200, 325);
	exit(1);
}

void menu(int position)
{
	clear();
	title("menu");

	cout << "  anticipation\n\n";

	if (position == 1)
	{
		cout << "  > start <\n";
	}
	else
	{
		cout << "  start\n";
	}

	if (position == 2)
	{
		cout << "  > credits <\n";
	}
	else
	{
		cout << "  credits\n";
	}

	if (position == 3)
	{
		cout << "  > quit <\n";
	}
	else
	{
		cout << "  quit\n";
	}

	const int keyInput{ _getch() };

	if (keyInput == 'W' && position != 1 || keyInput == 'w' && position != 1)
	{
		menu(position - 1);
	}
	else if (keyInput == 'S' && position != 3 || keyInput == 's' && position != 3)
	{
		menu(position + 1);
	}
	else if (keyInput == 13)
	{
		switch (position)
		{
		case 1:
			start(1);
			break;

		case 2:
			credits();
			break;

		case 3:
			quit();
			break;
		}
	}
	else
	{
		menu(position);
	}
}

void getInput()
{
	const int keyInput{ _getch() };

	switch (keyInput)
	{
	case 'W':
	case 'w':
		positionSelectedTile('w');
		break;

	case 'S':
	case 's':
		positionSelectedTile('s');
		break;

	case 'D':
	case 'd':
		positionSelectedTile('d');
		break;

	case 'A':
	case 'a':
		positionSelectedTile('a');
		break;

	case 'H':
	case 'h':
		help();
		break;

	case ' ':
	case 13:
		revealTile();
		break;

	case 27:
		menu(1);
		break;
	}

	refresh();
	getInput();
}

int main()
{
	menu(1);
}