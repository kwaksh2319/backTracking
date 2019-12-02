#include<stdio.h>
#include<string.h>
#include<Windows.h>
//backtracking ->tree�� ǥ�� ������ ������ �ذ��ϴ� �˰���

#define MAX 1024
#define INIT -1
#define START 'S'
#define GOAL 'G'
#define WAY ' '
#define WALL '#'
#define MARKED '+'
enum class Direction {

	Up=0,Down,Right,Left
};
struct Position {
	int X, Y;


};
struct MazeDesc {

	int Rows;
	int Columns;
	char**Data;
};
void ReadFile(const char*path, MazeDesc*maze) {
	FILE*fp;
	fopen_s(&fp, path, "r");
	int rows = 0, columns = INIT;
	char buffer[MAX];
	while (fgets(buffer, MAX, fp) != NULL) {
		//�� ī��Ʈ
		rows++;
		//�� �ٿ� �� ���ڰ� �ִ���
		if (columns == INIT)
			columns = strlen(buffer) - 1;
	}

	maze->Rows = rows;
	maze->Columns = columns;
	maze->Data = new char*[rows];
	for (int i = 0; i < rows; i++)
		maze->Data[i] = new char[columns];

	rewind(fp);

	for (int i = 0; i < rows; i++) {
		fgets(buffer, MAX, fp);
		for (int j = 0; j < columns; j++) {
			maze->Data[i][j] = buffer[j];
		}
	}
	fclose(fp);

}

bool GetNextStep(MazeDesc*maze, Position*curr, Direction direction, Position*next) {
	switch (direction)
	{
	case Direction::Up: {
		next->X = curr->X;
		next->Y = curr->Y - 1;
		if (next->Y == -1)
			return false;
		}break;

	case Direction::Down:{
		next->X = curr->X;
		next->Y = curr->Y + 1;
		if (next->Y == maze->Rows)
			return false;
			}break;
	case Direction::Left:{
		next->X = curr->X - 1;
		next->Y = curr->Y;
		if (next->X ==-1)
			return false;
				}break;
	case Direction::Right:{
		next->X = curr->X+1;
		next->Y = curr->Y;
		if (next->X == maze->Columns)
			return false;
			}break;

		return true;
	
	}
	if (maze->Data[next->Y][next->X] == WALL) return false;
	if (maze->Data[next->Y][next->X] == MARKED) return false;
	return true;
}
bool MoveTo(MazeDesc*maze, Position*curr, Direction direction) {

	Position next;
	//��� ����� Ż�� ����
	if (maze->Data[curr->Y][curr->X] == GOAL)
		return true;
	//������
	maze->Data[curr->Y][curr->X] = MARKED;
	for (int i = 0; i < 4; i++) {
		//4���� ���� �ִ��� �˻�
		if (GetNextStep(maze, curr, (Direction)i, &next) == false)
			continue;
		//���� �ִ� ��Ȳ�̸� ��͸� �¿�
		if (MoveTo(maze, &next, Direction::Up) == true)
			return true; //��� �Ѵܰ辿 Ż��
	}
	//��� ��Ͱ� ������ marked�� way�� ����
	maze->Data[curr->Y][curr->X] = WAY;
	return true;
}

bool Solve(MazeDesc*maze) {
	bool startFound = false;
	bool result = false;

	//������ ã��
	Position start;
	for (int y = 0; y < maze->Rows; y++) {
		for (int x = 0; x < maze->Columns; x++)
		{
			start.X = x;
			start.Y = y;
			startFound = true;
			break;

		}
	}
	if (startFound == false) {

		printf("����� ����\n");
		return false;

	}
	result |= MoveTo(maze, &start, Direction::Up);
	result |= MoveTo(maze, &start, Direction::Down);
	result |= MoveTo(maze, &start, Direction::Right);
	result |= MoveTo(maze, &start, Direction::Left);

	maze->Data[start.Y][start.Y] = START;

	return result;

}
void main() {

	const char*path = "Maze2.txt";
	MazeDesc maze;
	ReadFile(path, &maze);

	if (Solve(&maze) == false)
		return;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int y = 0; y < maze.Rows; y++)
	{
		for (int x = 0; x < maze.Columns; x++) {
			if (maze.Data[y][x] == MARKED)
				SetConsoleTextAttribute(handle, FOREGROUND_BLUE| FOREGROUND_GREEN);
			else
				SetConsoleTextAttribute(handle, FOREGROUND_RED|FOREGROUND_BLUE | FOREGROUND_GREEN);

			printf("%c", maze.Data[y][x]);
		}
		printf("\n");
	}



	system("pause");
}