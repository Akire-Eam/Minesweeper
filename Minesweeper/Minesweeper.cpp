
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

/**
Description: Class that contains the codes that create the map
Arguments:

**/
class Map
{
public:

	int width, height;

    /**
    Description: Sets the height and width of the map
    Arguments:

    **/
	void setSize(int h, int w) {

		height = h;
		width = w;
	}

	/**
    Description: Function that counts the number of mines in the gameplay
    Arguments:
        ele      Mines present in the map
    Return:
        int      Returns the counted number of mines

    **/

	int countThisElement(char ele) {

		int c = 0;
		for (int i = 0; i < height; i++) {

			for (int j = 0; j < width; j++) {

				char ch = mapArray[i][j];
				if (ch == 'm') ch = 'M';
				if (ch == ele) {
					c++;
				}
			}
		}
		return c;
	}

    /**
    Description:        Generates mines to random positions
    Arguments:
        mineNumber      Number of mines in the map

    **/
	void layMineField(int mineNumber) {

		fillMapWith(' ');

		int totalCells = width * height;
		if (mineNumber > totalCells) mineNumber = totalCells;

		mapArray.resize(height);

		int mineCount = 0;
		boolean re_gen = false;

	generateMine:
		for (int i = 0; i < height; i++) {

			if (!re_gen) mapArray[i].resize(width);
			for (int j = 0; j < width; j++) {

				if (mapArray[i][j] != 'M') {

					if (mineCount < mineNumber) {
						int hasMine = rand() % 100 + 1;
						if (hasMine <= 8) {
							mapArray[i][j] = 'M';
							mineCount++;
						}
						else {
							mapArray[i][j] = ' ';
						}
					}
				}
			}
		}
		if (mineCount < mineNumber) {
			re_gen = true;
			goto generateMine;
		}
	}

    /**
    Description: Generates random maps
    Arguments:
        mineNum  Number of mines in the map

    **/
	void setRandomMap(int mineNum) {

		layMineField(mineNum);
		char digit[] = { ' ','1','2','3','4','5','6' };

		for (int y = 0; y < height; y++) {

			for (int x = 0; x < width; x++) {

				if (mapArray[y][x] != 'M') {

					int mineCount = 0;

					for (int i = y - 1; i <= y + 1; i++) {

						for (int j = x - 1; j <= x + 1; j++) {

							if (i >= 0 && i < height && j >= 0 && j < width) {

								if (mapArray[i][j] == 'M') {

									mineCount++;
								}
							}
						}
					}
					mapArray[y][x] = digit[mineCount];
				}
			}
		}
	}

    /**
    Description: Function that fills given map with given char
    Arguments:
        c        Char that will be placed on the map

    **/

	void fillMapWith(char c) {

		mapArray.resize(height);
		for (int i = 0; i < height; i++) {

			mapArray[i].resize(width);
			for (int j = 0; j < width; j++) {

				mapArray[i][j] = c;
			}
		}
	}

	/**
    Description: Function that returns char at a specific position in the map
    Arguments:
        y        y position
        x        x position
    Return:
        char     Returns char at the given position in the map

    **/

	char getCharAt(int y, int x) {

		if (y < mapArray.size() && x < mapArray[y].size())
			return mapArray[y][x];
		return mapArray[y][x];
	}

	/**
    Description: Function that replace position in map with the given char
    Arguments:
        y        y position
        x        x position
        c        Char that will be placed in the map

    **/

	void setChar(int y, int x, char c) {

		mapArray[y][x] = c;
	}

private:
	vector < vector<char> > mapArray;
};

/**
Description: Function that settles a new vertical and horizontal position
Arguments:

**/

void gotoxy(SHORT y, SHORT x) {

	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCursor(bool hide) {

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 25;
	cci.bVisible = !hide;
	SetConsoleCursorInfo(hStdout, &cci);
}

void prepareWindow(int y, int x) {

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, x, y, TRUE); // 800 width, 100 height

	HWND scroll = GetConsoleWindow();
	ShowScrollBar(scroll, SB_BOTH, FALSE);

	hideCursor(true);
}

/**
Description: Class that contains the codes that colorizes text and displays map
Arguments:

**/

class Renderer {

public:

    /**
    Description: Function that changes the color of certain text in the display
    Arguments:

    **/

	void colorize(char c = ' ') {

		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (c) {

		case 'M': {
			SetConsoleTextAttribute(h, FOREGROUND_RED | BACKGROUND_INTENSITY);
			break;
		}

		case 'm': {
			SetConsoleTextAttribute(h, 12 | BACKGROUND_GREEN);
			break;
		}

		case 'F': {
			SetConsoleTextAttribute(h, 14);
			break;
		}

		case 'f': {
			SetConsoleTextAttribute(h, 14 | BACKGROUND_RED);
			break;
		}

		case ' ': {
			SetConsoleTextAttribute(h, 8);
			break;
		}

		case  '1': {
			SetConsoleTextAttribute(h, 9);
			break;
		}

		case  '2': {
			SetConsoleTextAttribute(h, 10);
			break;
		}

		case  '3': {
			SetConsoleTextAttribute(h, 12);
			break;
		}

		case  '4': {
			SetConsoleTextAttribute(h, 15);
			break;
		}
		case  '5': {
			SetConsoleTextAttribute(h, 13);
			break;
		}
		case  '6': {
			SetConsoleTextAttribute(h, 11);
			break;
		}
		default:
			SetConsoleTextAttribute(h, int(c) - 48);
			break;
		}
	}

    /**
    Description: Function that prints the display of the minesweeper map
    Arguments:
        map

    **/

	void printMap(Map map) {

		gotoxy(2, 0);
		for (int i = 0; i < map.width + 2; i++) {
			colorize('F');
			cout << "=" << " "; //space
		}
		cout << endl;
		for (int i = 0; i < map.height; i++) {

			for (int j = 0; j < map.width; j++) {

				char c = map.getCharAt(i, j);

				if (j == 0) {
					colorize('F');
					cout << "|" << " "; //space
					colorize(c);

					cout << c;
					colorize(' ');
					cout << " "; //space
				}
				else if (j == map.width - 1) {

					colorize(c);
					if (c == 'm') c = 'M';
					cout << c;
					colorize('F');
					cout << " " << "|"; //space
				}
				else {

					colorize(c);
					if (c == 'm') c = 'M';
					if (c == 'f') c = 'F';
					cout << c;

					colorize(' ');
					cout << " "; //space
				}
				colorize();
			}
			cout << endl;
		}
		gotoxy(map.height + 3, 0);
		for (int i = 0; i < map.width + 2; i++) {
			colorize('F');
			cout << "=" << " "; //space
		}
		cout << endl;
	}
};

/**
Description: Structure to be used for the data of highscores
Arguments:

**/

struct HighScore {

	int beginner_score;
	int beginner_score2;
	int beginner_score3;
	int inter_score;
	int inter_score2;
	int inter_score3;
	int expert_score;
	int expert_score2;
	int expert_score3;
	string begname;
	string begname2;
	string begname3;
	string intername;
	string intername2;
	string intername3;
	string exname;
	string exname2;
	string exname3;
};

struct Cell {

	char value;
	int y;
	int x;
};

struct Action {

	string type;
	vector<Cell> cells;
};

/**
Description: Reads the list of high scores from a text file and inputs them in the Highscore structure
Arguments:

**/

HighScore readHighScore() {

	HighScore hc;
	ifstream input;
	input.open("highscore.txt");
	if (!input.eof()) {
		input >> hc.begname >> hc.beginner_score;
		input >> hc.begname2 >> hc.beginner_score2;
		input >> hc.begname3 >> hc.beginner_score3;
		input >> hc.intername >> hc.inter_score;
		input >> hc.intername2 >> hc.inter_score2;
		input >> hc.intername3 >> hc.inter_score3;
		input >> hc.exname >> hc.expert_score;
		input >> hc.exname2 >> hc.expert_score2;
		input >> hc.exname3 >> hc.expert_score3;

		if (hc.beginner_score < 0) hc.beginner_score = 0;
		if (hc.beginner_score2 < 0) hc.beginner_score2 = 0;
		if (hc.beginner_score3 < 0) hc.beginner_score3 = 0;
		if (hc.inter_score < 0) hc.inter_score = 0;
		if (hc.inter_score2 < 0) hc.inter_score2 = 0;
		if (hc.inter_score3 < 0) hc.inter_score3 = 0;
		if (hc.expert_score < 0) hc.expert_score = 0;
		if (hc.expert_score2 < 0) hc.expert_score2 = 0;
		if (hc.expert_score3 < 0) hc.expert_score3 = 0;
	}
	else {
		hc.beginner_score = 0;
		hc.beginner_score2 = 0;
		hc.beginner_score3 = 0;
		hc.inter_score = 0;
		hc.inter_score2 = 0;
		hc.inter_score3 = 0;
		hc.expert_score = 0;
		hc.expert_score2 = 0;
		hc.expert_score3 = 0;
	}
	input.close();
	return hc;
}

/**
Description: Class that contains the codes that make the game function
Arguments:

**/

class GameManager
{
public:
	GameManager();
	~GameManager();

	bool isGameOver = false;
	Map trueMap, hiddenMap, checkedMap;
	Renderer renderer;

	int fieldWidth;
	int fieldHeight;
	int mineNum;
	int flagLeft;

	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf;
	int counter = 0;

	bool clicked = false;
	bool exit = false;
	bool restart = false;
	bool beginGame = false;

	string level;

	int startTime;
	int elapsedTime;
	string name;

	vector<Cell> allOpenedCells;

	/**
    Description: Function that resets the data required for a new game
    Arguments:

    **/

	void initGame() {

		srand(time(NULL));

		startTime = clock();

		flagLeft = mineNum;

		restart = false;
		isGameOver = false;
		beginGame = false;

		allOpenedCells.clear();

		trueMap.setSize(fieldHeight, fieldWidth);
		hiddenMap.setSize(fieldHeight, fieldWidth);
		checkedMap.setSize(fieldHeight, fieldWidth);

		trueMap.setRandomMap(mineNum);
		hiddenMap.fillMapWith('?');
		checkedMap.fillMapWith(' ');

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hStdin, &fdwSaveOldMode);
		fdwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
		SetConsoleMode(hStdin, fdwMode);

		elapsedTime = (clock() - startTime) / (CLOCKS_PER_SEC);
	}

	/**
    Description: Function that prepares the dimensions and mines of a custom game
    Arguments:
        h        Height of the custom game
        w        Width of the custom game
        mn       Number of mines of the custom game

    **/

	void setCustomGame(int h, int w, int mn) {

		fieldHeight = h;
		fieldWidth = w;
		mineNum = mn;

		if (w<13 || h<13)
		{
			prepareWindow(450, 655);
		}
		else{
		prepareWindow(h * 20 + 150, w * 20 + 350);
		}
	}

	/**
    Description: Function that prepares the dimensions and mines of chosen level
    Arguments:
        lv       String that indicates game level

    **/

	void setGameByLevel(string lv) {

		level = lv;

		if (lv == "Beginner") {

			fieldHeight = 9;
			fieldWidth = 9;
			mineNum = 10;
			prepareWindow(350, 555);
		}

		else if (lv == "Intermediate") {

			fieldHeight = 16;
			fieldWidth = 16;
			mineNum = 40;
			prepareWindow(432, 670);
		}


		else if (lv == "Expert") {

			fieldHeight = 16;
			fieldWidth = 30;
			mineNum = 99;
			prepareWindow(525, 899);
		}
	}


	/**
    Description: Function that takes you name and welcomes you at the start of a gameplay
    Arguments:

    **/

	void Name()
	{
	    system("CLS");
	    hideCursor(false);
        renderer.colorize('2');
        gotoxy(6, 24 + 3);
        cout<<"What's your name? ";
        gotoxy(7, 24 );
        renderer.colorize('3');
        cout<<"Enter your name: ";
        cin >> name;
        hideCursor(true);
        system("CLS");
        renderer.colorize('2');
        gotoxy(7, 24 - 3);
        cout<<"Welcome to Minesweeper, "<< name <<"!";
        Sleep(1500);
        system("CLS");
	}

	/**
    Description: Function that prepares all requirements at the beginning of a gameplay until end of gameplay
    Arguments:

    **/

	void HoomanPlay() {

		while (true) {

            Name();
			initGame();
			showUI();
			renderer.printMap(hiddenMap);

			while (!isGameOver) {

				if (beginGame) {
					elapsedTime = (clock() - startTime) / (CLOCKS_PER_SEC) ;
				}

				showUI();
				checkInput();
				if (clicked) {
					renderer.printMap(hiddenMap);
					clicked = false;
					if (!beginGame) {
						startTime = clock();
						beginGame = true;
					}
				}
				if (restart) goto next;

				if (exit) goto out;

				if (checkWin()) {

					setNewHighScore(level, elapsedTime, name);

					renderer.colorize('2');
					gotoxy(trueMap.height + 4, trueMap.width - 6);
					cout << "OK YOU WIN :((" << endl;
					renderer.colorize('F');
					gotoxy(trueMap.height + 5, trueMap.width - 17);
					cout << "Please press Enter to restart game\n";
					gotoxy(trueMap.height + 6, trueMap.width - 12);
					cout << "or press ESC to exit...";
				}
			}

			gameOver();

		check_restart:
			while (true) {

				checkInput();
				if (restart) {
					if (isGameOver) system("cls");
					goto next;
				}
				if (exit) goto out;
			}
		out: { if (exit) break; }
		next: {}
		}
		system("cls");
	}

	/**
    Description: Function that puts a flag on the map and counts how many flags are left
    Arguments:
        y        y position
        x        x position

    **/

	void flagCell(int y, int x) {

		if (hiddenMap.getCharAt(y, x) != 'F') {
			hiddenMap.setChar(y, x, 'F');
			flagLeft--;
		}
		else {
			hiddenMap.setChar(y, x, '?');
			flagLeft++;
		}
	}

	/**
    Description: Function that opens cell in the map
    Arguments:

    **/

	void openCell(int y, int x) {

		if (hiddenMap.getCharAt(y, x) != 'F' && hiddenMap.getCharAt(y, x) != ' ') {
			clicked = true;
			if (trueMap.getCharAt(y, x) == 'M') {

				isGameOver = true;
				trueMap.setChar(y, x, 'm');
			}
			else {
				oilSpills(y, x);
			}
		}
	}

private:

    /**
    Description: Function that appends new high scorer to the list
    Arguments:
        level    Level of difficulty
        score    Given score
        name     Name of the new high scorer

    **/

	void setNewHighScore(string level, int score, string name) {

		HighScore hc = readHighScore();
		ofstream output("highscore.txt", ios::out | ios::trunc);

		if (level == "Beginner") {

			if (hc.beginner_score != 0) {
                if (score < hc.beginner_score){
                        hc.begname2 = hc.begname;
                        hc.begname = name;
                }
				if (score < hc.beginner_score){
                        hc.beginner_score2 = hc.beginner_score;
                        hc.beginner_score = score;
				}

				if (hc.beginner_score2 != 0) {
                    if (score < hc.beginner_score2 && score > hc.beginner_score) {
                            hc.begname3 = hc.begname2;
                            hc.begname2 = name;
                    }
                    if (score < hc.beginner_score2 && score > hc.beginner_score) {
                            hc.beginner_score3 = hc.beginner_score2;
                            hc.beginner_score2 = score;
				}

                }
                if (hc.beginner_score3 != 0) {
                    if (score < hc.beginner_score3 && score > hc.beginner_score2 && score > hc.beginner_score) hc.begname3 = name;
                    if (score < hc.beginner_score3 && score > hc.beginner_score2 && score > hc.beginner_score) hc.beginner_score3 = score;
                }
			}
			else {
			    hc.begname = name;
				hc.beginner_score = score;

			}




		}
		else if (level == "Intermediate") {

			if (hc.inter_score != 0) {
                if (score < hc.inter_score){
                        hc.intername2 = hc.intername;
                        hc.intername = name;
                }
				if (score < hc.inter_score){
                        hc.inter_score2 = hc.inter_score;
                        hc.inter_score = score;
				}

				if (hc.inter_score2 != 0) {
                    if (score < hc.inter_score2 && score > hc.inter_score) {
                            hc.intername3 = hc.intername2;
                            hc.intername2 = name;
                    }
                    if (score < hc.inter_score2 && score > hc.inter_score) {
                            hc.inter_score3 = hc.inter_score2;
                            hc.inter_score2 = score;
				}
                }


                if (hc.inter_score3 != 0) {
                    if (score < hc.inter_score3 && score > hc.inter_score2 && score > hc.inter_score) hc.intername3 = name;
                    if (score < hc.inter_score3 && score > hc.inter_score2 && score > hc.inter_score) hc.inter_score3 = score;
                }

			}
			else {
                hc.intername = name;
				hc.inter_score = score;

			}



		}
		else if (level == "Expert") {

			if (hc.expert_score != 0) {
                if (score < hc.expert_score){
                        hc.exname2 = hc.exname;
                        hc.exname = name;
                }
				if (score < hc.expert_score){
                        hc.expert_score2 = hc.expert_score;
                        hc.expert_score = score;
				}

				if (hc.expert_score2 != 0) {
                    if (score < hc.expert_score2 && score > hc.expert_score) {
                            hc.exname3 = hc.exname2;
                            hc.exname2 = name;
                    }
                    if (score < hc.expert_score2 && score > hc.expert_score) {
                            hc.expert_score3 = hc.expert_score2;
                            hc.expert_score2 = score;
                    }
                }


                if (hc.expert_score3 != 0) {
                    if (score < hc.expert_score3 && score > hc.expert_score2 && score > hc.expert_score) hc.exname3 = name;
                    if (score < hc.expert_score3 && score > hc.expert_score2 && score > hc.expert_score) hc.expert_score3 = score;
                }
			}
			else {
			    hc.exname = name;
				hc.expert_score = score;
			}



		}
		output << hc.begname << " " << hc.beginner_score << endl;
		output << hc.begname2 << " " << hc.beginner_score2 << endl;
		output << hc.begname3 << " " << hc.beginner_score3 << endl;
		output << hc.intername << " " << hc.inter_score << endl;
		output << hc.intername2 << " " << hc.inter_score2 << endl;
		output << hc.intername3 << " " << hc.inter_score3 << endl;
		output << hc.exname << " " << hc.expert_score << endl;
		output << hc.exname2 << " " << hc.expert_score2 << endl;
		output << hc.exname3 << " " << hc.expert_score3 << endl;
	}

	/**
    Description: Function that checks if all the mines have been flagged
    Arguments:
    Return:
        bool     Returns status of the gameplay

    **/

	bool checkWin() {

		bool isWin = true;
		for (int i = 0; i < fieldHeight; i++) {

			for (int j = 0; j < fieldWidth; j++) {

				if (trueMap.getCharAt(i, j) == 'M') {

					if (hiddenMap.getCharAt(i, j) != 'F') {

						isWin = false;
						break;
					}
				}
			}
		}
		return isWin;
	}

	void checkInput() {

		int x; int y;

		ReadConsoleInput(hStdin, &irInBuf, 1, &cNumRead);
		switch (irInBuf.EventType) {
		case KEY_EVENT:

			if (irInBuf.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
				exit = true;
			}
			else if (irInBuf.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
				Sleep(100);
				if (restart == false) restart = true;
			}
			break;

		case MOUSE_EVENT:

			if (!isGameOver) {
				MOUSE_EVENT_RECORD mer = irInBuf.Event.MouseEvent;

				switch (mer.dwEventFlags) {
				case 0:

					if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						x = mer.dwMousePosition.X;
						y = mer.dwMousePosition.Y;

						if (x % 2 == 0) {
							if (x >= 2 && x <= fieldWidth * 2 && y >= 2 && y <= fieldHeight + 2) {
								x = (x - 2) / 2;
								y = y - 3;

								openCell(y, x);
							}
						}
					}
					else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
						x = mer.dwMousePosition.X;
						y = mer.dwMousePosition.Y;
						if (x % 2 == 0) {
							if (x >= 2 && x <= fieldWidth * 2 && y >= 2 && y <= fieldHeight + 2) {
								x = (x - 2) / 2;
								y = y - 3;
								clicked = true;
								if (hiddenMap.getCharAt(y, x) == '?' || hiddenMap.getCharAt(y, x) == 'F') {
									flagCell(y, x);
								}
							}
						}
					}
					break;
				default:
					break;
				}
			}
			break;

		case WINDOW_BUFFER_SIZE_EVENT:
			break;

		case FOCUS_EVENT:
			break;

		case MENU_EVENT:
			break;

		default:
			break;
		}
		FlushConsoleInputBuffer(hStdin);
	}

	/**
    Description: Function that prepares user interface at the beginning of a gameplay
    Arguments:

    **/

	void showUI() {

		gotoxy(0, trueMap.width + 18);
		renderer.colorize('3');
		cout << "MINESWEEPERS" << endl;
		renderer.colorize('?');
		gotoxy(1, trueMap.width - 1);
		cout << "Machine Problem: Erica Mae V. Antonino & Stefani Ann Gutierrez" << endl;
		renderer.colorize();

		gotoxy(2, trueMap.width * 2 + 15);
		renderer.colorize('5');
		if (elapsedTime > 60) {

			int minutes = elapsedTime / 60;
			int seconds = elapsedTime % 60;
			if (minutes < 10) {
				cout << "Elapsed time: 0" << minutes << ":";
			}
			else {
				cout << "Elapsed time: " << minutes << ":";
			}
			if (seconds < 10) {
				cout << "0" << seconds << "    ";
			}
			else {
				cout << seconds << "    ";
			}
		}
		else {
			if (elapsedTime < 10) {
				cout << "Elapsed time: 00:0" << elapsedTime;
			}
			else {
				cout << "Elapsed time: 00:" << elapsedTime;
			}
		}

		gotoxy(3, trueMap.width * 2 + 15);
		renderer.colorize('2'); cout << "Field size: " << fieldWidth << "x" << fieldHeight;
		gotoxy(4, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << "Mines: " << trueMap.countThisElement('M');
		gotoxy(5, trueMap.width * 2 + 15);
		renderer.colorize('F'); cout << "Flag(s): " << flagLeft << "    ";

		gotoxy(7, trueMap.width * 2 + 15);
		renderer.colorize('6'); cout << "How to play?";
		gotoxy(8, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - Left click to open";
		gotoxy(9, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - Right click to set flag";
		gotoxy(10, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - Press Enter to restart";
		gotoxy(11, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - Press ESC to exit";

		gotoxy(13, trueMap.width * 2 + 15);
		renderer.colorize('2'); cout << "Note: ";
		gotoxy(14, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - ";
		renderer.colorize('M'); cout << "M";
		renderer.colorize('4'); cout << ": Mine";

		gotoxy(15, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - ";
		renderer.colorize('F'); cout << "F";
		renderer.colorize('4'); cout << ": Flag";

		gotoxy(16, trueMap.width * 2 + 15);
		renderer.colorize('4'); cout << " - ";
		renderer.colorize('f'); cout << "F";
		renderer.colorize('4'); cout << ": Wrong flag";
	}

	/**
    Description: Function that prepares display when the player opens a mine
    Arguments:

    **/

	void gameOver() {

		for (int i = 0; i < trueMap.height; i++) {

			for (int j = 0; j < trueMap.width; j++) {

				if (hiddenMap.getCharAt(i, j) != 'F') {
					hiddenMap.setChar(i, j, trueMap.getCharAt(i, j));
				}
				else {
					if (trueMap.getCharAt(i, j) != 'M') {
						hiddenMap.setChar(i, j, 'f');
					}
				}
			}
		}
		showUI();
		renderer.printMap(hiddenMap);
		renderer.colorize('3');
		gotoxy(trueMap.height + 4, trueMap.width - 14);
		cout << "   BOOM!! GAME OVER!! :))" << endl;
		renderer.colorize('F');
		gotoxy(trueMap.height + 5, trueMap.width - 17);
		cout << "Please press Enter to restart game\n";
		gotoxy(trueMap.height + 6, trueMap.width - 12);
		cout << "or press ESC to exit...";
	}

	void oilSpills(int y, int x) {

		checkedMap.setChar(y, x, 'x');
		char c = trueMap.getCharAt(y, x);


		if (c == ' ') {

			hiddenMap.setChar(y, x, ' ');
			string s = to_string(y) + "-" + to_string(x) + "-v=" + c;

			if (x - 1 >= 0) {

				char tc = trueMap.getCharAt(y, x - 1);
				char xc = checkedMap.getCharAt(y, x - 1);
				char hc = hiddenMap.getCharAt(y, x - 1);

				if (xc != 'x' && hc != 'F' && tc != 'M') {
					oilSpills(y, x - 1);
				}
			}
			if (x + 1 < hiddenMap.width) {

				char tc = trueMap.getCharAt(y, x + 1);
				char xc = checkedMap.getCharAt(y, x + 1);
				char hc = hiddenMap.getCharAt(y, x + 1);

				if (xc != 'x' && hc != 'F' && tc != 'M') {
					oilSpills(y, x + 1);
				}
			}
			if (y - 1 >= 0) {

				char tc = trueMap.getCharAt(y - 1, x);
				char xc = checkedMap.getCharAt(y - 1, x);
				char hc = hiddenMap.getCharAt(y - 1, x);

				if (xc != 'x' && hc != 'F' && tc != 'M') {
					oilSpills(y - 1, x);
				}
			}
			if (y + 1 < hiddenMap.height) {

				char tc = trueMap.getCharAt(y + 1, x);
				char xc = checkedMap.getCharAt(y + 1, x);
				char hc = hiddenMap.getCharAt(y + 1, x);
				if (xc != 'x' && hc != 'F' && tc != 'M') {
					oilSpills(y + 1, x);
				}
			}
		}
		else {
			hiddenMap.setChar(y, x, c);
		}
	}
};
GameManager::GameManager()
{
}
GameManager::~GameManager()
{
}

/**
Description: Class that contains the codes to display the starting screen and menu
Arguments:

**/

class Menu {

public:

	Renderer renderer;
	int choice;

	vector<string> mainText = { "Play", "LeaderBoard","Instructions", "Exit" };
	vector<string> levelText = { "Beginner", "Intermediate", "Expert", "Custom" };

	Menu() {

		choice = 0;
	}

    /**
    Description: Function that shows the first display
    Arguments:

    **/

	void start(){

		system("cls");
		prepareWindow(510, 400);
		gotoxy(8, 24 - 4);
		renderer.colorize('3');
		cout << "MINESWEEPERS" << endl;
		renderer.colorize('?');
		gotoxy(10, 24 - 6);
		cout << "Machine Problem:" << endl;
		renderer.colorize('F');
		gotoxy(11, 24 - 8);
		cout << "Erica Mae V. Antonino" << endl;
		gotoxy(12, 24 - 8);
		cout << "Stefani Ann Gutierrez" << endl;
		Sleep(1500);
		system("cls");

	}

    /**
    Description: Function that displays the banner
    Arguments:

    **/

	void showBanner() {

		prepareWindow(510, 400);
		gotoxy(0, 24 - 4);
		renderer.colorize('3');
		cout << "MINESWEEPERS" << endl;
		renderer.colorize('?');
		gotoxy(1, 24 - 6);
		cout << "Machine Problem: " << endl;
		gotoxy(2, 24 - 20);
		cout << "Erica Mae V. Antonino & Stefani Ann Gutierrez" << endl;
		renderer.colorize('F');
		gotoxy(3, 24 - 19);
		cout << "Use Up and Down button to select then Enter\n" << endl;

	}

    /**
    Description: Function that displays the instructions of the game
    Arguments:

    **/

	void printInstructions()
    {
        showBanner();

		renderer.colorize('F');
		gotoxy(4, 0);
        cout<<"THERE ARE 2 TYPES OF TILES:\n\na)COVERED\nb)UNCOVERED\n\n";
        cout<<"COVERED:\t\t\t"<<'?'<<"\n-------\n";
        cout<<"UNCOVERED: \n---------\n\ti)EMPTY:\t\t"<<' '<<"\n\tii)NUMBERED:\t\t1,2,3,4,5,6,7,8\n\tiii)MINES:\t\t"<<'M'<<"\n\tiiii)FLAGS:\t\t"<<'F'<<"\n\n";
        cout<<"A ROWxCOL BOARD WILL BE PRESENTED, WHICH, INITIALLY HAS ALL TILES COVERED. YOUR TASK IS TO UNCOVER ALL TILES THAT ARE NOT HAVING MINES.\n";
        cout<<"IF YOU SUCCEED TO DO SO, YOU WIN. IF YOU STEP ON A MINE, YOU LOSE.\n\n";
        cout<<"YOU HAVE TO ENTER ROW AND COL OF A PARTICULAR TILE TO UNCOVER THAT TILE.\n";
        cout<<"EACH NUMBERED TILE REPRESENTS THE NUMBER OF MINE ADJACENT TO THAT NUMBERED TILE.\n";
        renderer.colorize();
    }

    /**
    Description: Function that displays the high scores
    Arguments:

    **/

	void showHighScore() {

		showBanner();

		HighScore hc;
		hc = readHighScore();

		renderer.colorize('F');
		gotoxy(4, 0);
		cout << "==================================================" << endl;
		cout << "											" << endl;
		cout << "                    HIGHSCORE:		" << endl;

		if (hc.beginner_score != 0) {
			cout << "	       Beginner level: " << endl;
			renderer.colorize('3');
			cout << "	   " << "1st: " << hc.begname << " - " << hc.beginner_score << " seconds" << endl;
			if (hc.beginner_score2 != 0) {
                renderer.colorize('3');
                cout << "	   " << "2nd: " << hc.begname2 << " - " << hc.beginner_score2 << " seconds" << endl;
			}
			else{
                cout << "	   " << "2nd: None" << endl;
			}

			if (hc.beginner_score3 != 0) {
                renderer.colorize('3');
                cout << "	   " << "3rd: " << hc.begname3 << " - " << hc.beginner_score3 << " seconds" << endl;
			}
			else{
                cout << "	   " << "3rd: None" << endl;
			}

		}
		else {
			cout << "	       Beginner level: None" << endl;
		}

        renderer.colorize('F');
		if (hc.inter_score != 0) {
			cout << "	       Intermediate level: " << endl;
			renderer.colorize('3');
			cout << "	   " << "1st: " << hc.intername << " - " << hc.inter_score << " seconds" << endl;
			if (hc.inter_score2 != 0) {
                renderer.colorize('3');
                cout << "	   " << "2nd: " << hc.intername2 << " - " << hc.inter_score2 << " seconds" << endl;
			}
			else{
                cout << "	   " << "2nd: None" << endl;
			}

			if (hc.inter_score3 != 0) {
                renderer.colorize('3');
                cout << "	   " << "3rd: " << hc.intername3 << " - " << hc.inter_score3 << " seconds" << endl;
			}
			else{
                cout << "	   " << "3rd: None" << endl;
			}
		}
		else {
			cout << "	       Intermediate level: None" << endl;
		}

        renderer.colorize('F');
		if (hc.expert_score != 0) {
			cout << "	       Expert level: " << endl;
			renderer.colorize('3');
			cout << "	   " << "1st: " << hc.exname << " - " << hc.expert_score << " seconds" << endl;
			if (hc.expert_score2 != 0) {
                renderer.colorize('3');
                cout << "	   " << "2nd: " << hc.exname2 << " - " << hc.expert_score2 << " seconds" << endl;
			}
			else{
                cout << "	   " << "2nd: None" << endl;
			}

			if (hc.expert_score3 != 0) {
                renderer.colorize('3');
                cout << "	   " << "3rd: " << hc.exname3 << " - " << hc.expert_score3 << " seconds" << endl;
			}
			else{
                cout << "	   " << "3rd: None" << endl;
			}
		}
		else {
			cout << "	       Expert level: None" << endl;
		}

        renderer.colorize('F');
		cout << "											" << endl;
		cout << "==================================================";
		cout << "                   Press Esc to Exit";
		renderer.colorize();
	}

	/**
    Description: Function lets you choose from the menu and redirects to your choice
    Arguments:

    **/

	void showMain() {
		showBanner();
		vector<string> text = mainText;
		bool gone_up = false;
		bool gone_down = false;
		bool entered = false;
		bool esc_ed = false;

		bool choiceLevel = false;

		while (true) {

			if ((GetKeyState(VK_UP) & 1) != gone_up) {
				choice--;
				gone_up = !gone_up;
			}
			else if ((GetKeyState(VK_DOWN) & 1) != gone_down) {
				choice++;
				gone_down = !gone_down;
			}
			else if ((GetKeyState(VK_RETURN) & 1) != entered) {
				entered = !entered;

				if (choiceLevel) {

					GameManager gameManager;
					if (choice < 3) {
						gameManager.setGameByLevel(levelText[choice]);
					}
					else {
                        system("cls");
						int h, w, mn;
						hideCursor(false);
						renderer.colorize('6');
						gotoxy(9, 24 - 17);
						cout << "Enter width of the field: "; cin >> w;
						gotoxy(10, 24 - 17);
						cout << "Enter height of the field: "; cin >> h;
						gotoxy(11, 24 - 17);
						cout << "Enter number of mines (should be ~" << (int)(w*h*0.16) << "): "; cin >> mn;
						renderer.colorize();
						hideCursor(true);
						gameManager.setCustomGame(h, w, mn);
					}
					system("cls");
					gameManager.HoomanPlay();
					choice = 0;
					text = mainText;
					system("cls");
					choiceLevel = false;
				}
				else {

					if (choice == 0) {
						system("cls");
						choiceLevel = true;
						text = levelText;
					}
					else if (choice == 1) {
						system("cls");
						while (true) {
							showHighScore();
							if (GetKeyState(VK_ESCAPE) & 0x8000) break;
						}
						system("cls");
					}
					else if (choice == 2) {
                        system("cls");
                        while (true) {
							printInstructions();
							if (GetKeyState(VK_ESCAPE) & 0x8000) break;
						}
						system("cls");
					}
					else if (choice == 3) {
						goto end;
					}
				}
			}
			else if ((GetKeyState(VK_ESCAPE) & 1) != esc_ed) {
				esc_ed = !esc_ed;
				text = mainText;
				if (choiceLevel) {
					system("cls");
					choiceLevel = false;
				}
			}

			if (choice < 0) {
				choice = text.size() - 1;
			}
			else if (choice > text.size() - 1) {
				choice = 0;
			}

			showBanner();
			for (int i = 0; i < text.size(); i++) {
				renderer.colorize('2');
				gotoxy(i * 2 + 5, 24 - text[i].length() / 2);
				if (choice == i) renderer.colorize('f');
				cout << text[i];
				renderer.colorize(' ');
			}
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		}
	end: {}
	}
};

/**
Description: Function that starts the program
Arguments:

**/

int main() {

	Menu menu;
	menu.start();
	system("cls");
	menu.showMain();
	return 0;
}
