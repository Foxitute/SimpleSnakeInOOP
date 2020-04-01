/*
	Project Name: Snake Game (Basic)
	Date: 31th Mar, 2020
	Developer: Grigorchuk Max
*/
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>

using namespace std;

const int MAXFRAMEX = 69;
const int MAXFRAMEY = 29;
const int MAXSnakeSize = 100;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{
	if (size == 0)
	{
		size = 20; // default cursor size Changing to numbers from 1 to 20, decreases
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

class Point
{
private:
	int x;
	int y;
public:
	Point() { x = y = 0; }
	Point(int _x, int _y) : x(_x), y(_y) { }

	void SetPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int GetX() { return x; }
	int GetY() { return y; }


	void MoveUp() { 
		y--; 
		if (y < 0)
			y = MAXFRAMEY;
	}
	void MoveDown() {
		y++; 
		if (y > MAXFRAMEY)
			y = 0;
	}
	void MoveLeft() { 
		x--; 
		if (x < 0)
			x = MAXFRAMEX;
	}
	void MoveRight() { 
		x++;
		if (x > MAXFRAMEX)
			x = 0;
	}


	void Draw(char ch = '0') {
		gotoxy(x, y);
		cout << ch;
	}

	void Erase() {
		gotoxy(x, y);
		cout << " ";
	}

	void CopyPos(Point* p) {
		p->x = x;
		p->y = y;
	}

	bool isEqual(Point* p) {
		if (p->x == x && p->y == y)
			return true;
		return 0;
	}

	void Debug() {
		cout << "(" << x << ", " << y << ") ";
	}
};

class Snake
{
private:
	Point* cell[MAXSnakeSize]; // array of points to represent snake
	int size; // current size of snake
	char dir; // current direction of snake
	Point fruit;
	int score;
	bool state; // bool representing of state of snake i.e. living, dead
	bool started;
	bool blink; // fruit blink

public:
	Snake() 
	{
		score = 0;
		size = 1; // default size
		cell[0] = new Point(20, 20); // default position
		
		for (int i = 1; i < MAXSnakeSize; i++)
			cell[i] = NULL;
		fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
		state = false;
		started = true;
	}

	void AddCell(int x, int y) {
		cell[size++] = new Point(x, y);
	}

	void TurnUp() {
		if (dir != 's')
			dir = 'w'; // w is control key for turning upward
	}
	void TurnDown() {
		if (dir != 'w')
			dir = 's'; // s is control key for turning downward
	}
	void TurnLeft() {
		if (dir != 'd')
			dir = 'a'; // a is control key for turning left
	}
	void TurnRight() {
		if (dir != 'a')
			dir = 'd'; // d is control key for turning right
	}

	void WellcomeScreen() {
		SetConsoleTextAttribute(console, 15);
		printf("\n");
		printf("\t\t    _________         _________ 			 \n");
		printf("\t\t   /         \\       /         \\ 			 \n");
		printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\ 		 \n");
		printf("\t\t  |  |     |  |     |  |     |  | 			 \n");
		printf("\t\t  |  |     |  |     |  |     |  | 			 \n");
		printf("\t\t  |  |     |  |     |  |     |  |         /	 \n");
		printf("\t\t  |  |     |  |     |  |     |  |       //	 \n");
		printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ /\n");
		printf("\t\t  \\__/      \\         /       \\        /  \n");
		printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~   	 \n");
		printf("\t\t    ^										 \n");
		printf("\t		Welcome To The Snake Game!			\n");
		printf("\t		 PRESS ANY KEY TO START...	\n");
		printf("\n");
	}

	void Move()
	{
		// Clear screen
		system("cls");
		printf("Score %d", score);

		if (state == false) {
			if ( started ) {
				WellcomeScreen();
				_getch();
				state = true;
				size = 1;
			}
			else if ( !started ) {
				cout << "\n\t\tGAME OVER";
				cout << "\n\t\tPRESS ANY KEY TO START...\n";
				_getch();
				state = true;
				size = 1;
			}
		}


		// making snake body follow its head
		for (int i = size-1; i > 0; i--)
		{
			cell[i-1]->CopyPos(cell[i]);
		}
		// turning snake's head
		switch (dir)
		{
		case 'w':
			cell[0]->MoveUp();
			break;
		case 's':
			cell[0]->MoveDown();
			break;
		case 'a':
			cell[0]->MoveLeft();
			break;
		case 'd':
			cell[0]->MoveRight();
			break;
		}

		if ( SelfCollision() ) {
			state = false;
			started = false;
		}

		// Collision with fruit
		if (fruit.GetX() == cell[0]->GetX() && 
				fruit.GetY() == cell[0]->GetY())
		{
			AddCell(0, 0);
			fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
			score++;
		}

		// drawing snake
		for (int i = 0; i < size; i++)
			cell[i]->Draw();

		SetConsoleTextAttribute(console, 02);
		if (!blink)
			fruit.Draw('@');
		blink = !blink;
		SetConsoleTextAttribute(console, 04);

		//Debug();

		Sleep(50);
	}

	bool SelfCollision() {
		for (int i = 1; i < size; i++)
			if (cell[0]->isEqual(cell[i]))
				return true;
		return false;
	}

	void Debug() {
		for (int i = 0; i < size; i++)
			cell[i]->Debug();
	}
};

int main()
{
	setcursor(0, 0);
	// random no generation
	srand((unsigned) time(NULL));


	// testing snake
	Snake snake;
	char op = 'l';

	do
	{
		if (_kbhit()) {
			op = _getch();
		}
		switch (op)
		{
			case 'w':
			case 'W':
				snake.TurnUp();
				break;
			case 's':
			case 'S':
				snake.TurnDown();
				break;
			case 'a':
			case 'A':
				snake.TurnLeft();
				break;
			case 'd':
			case 'D':
				snake.TurnRight();
				break;
			default:
				break;
		}
		snake.Move();

	} while (op!='e');

	SetConsoleTextAttribute(console, 07);

	return 0;
}

// THE END