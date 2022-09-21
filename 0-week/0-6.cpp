#include <iostream>
#include <random>

using namespace std;
random_device rd;
class make_road {
private:
	int board[30][30];
	int position_x, position_y;

public:
	make_road() {
		uniform_int_distribution<int> uid(1, 7);
		int number{ 2 }, direct{ 0 }, move_direct{0};
		int count = 0;
		position_x = 0; position_y = 0;
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				board[i][j] = 0;
			}
		}
		board[0][0] = 1;
		direct = uid(rd) % 2;//0=x 1=y
		while (position_x != 29 || position_y != 29) {
			count++;
			if (count > 2) {
				move_point(direct, 8, &number);
			}
			else
			{
				if (uid(rd) % 2)move_point(direct, uid(rd), &number);
				else move_point(direct, uid(rd) * (-1), &number);
			}
			if (direct) {
				direct = 0;
			}
			else {
				direct = 1;
			}
		}
		board[29][29] = number;
	}
	void re_make() {
		uniform_int_distribution<int> uid(1, 7);
		int number{ 2 }, direct{ 0 }, move_direct{ 0 };
		int count = 0;
		position_x = 0; position_y = 0;
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				board[i][j] = 0;
			}
		}
		board[0][0] = 1;
		direct = uid(rd) % 2;//0=x 1=y
		while (position_x != 29 || position_y != 29) {
			count++;
			if (count > 2) {
				move_point(direct, 7, &number);
				count = 0;
			}
			else
			{
				if (uid(rd) % 2)move_point(direct, uid(rd), &number);
				else move_point(direct, uid(rd) * (-1), &number);
			}
			if (direct) {
				direct = 0;
			}
			else {
				direct = 1;
			}
		}
		board[29][29] = number;
	}
	void move_point(int direct, int distance,int* number) {
		if (direct) {
			if (distance > 0) {
				for (int i = 1; i <= distance; i++) {
					if (position_x + i >= 29) { position_x = 29; return; }
					board[position_y][position_x + i] = *number;
					
					*number = *number+1;
				}
			}
			else {
				for (int i = -1; i >= distance; i--) {
					if (position_x + i <= 0) { position_x = 0; return; }
					board[position_y][position_x + i] = *number;
					
					*number = *number + 1;
				}
			}
			position_x += distance;
		}
		else {
			if (distance > 0) {
				for (int i = 1; i <= distance; i++) {
					if (position_y + i >= 29) { position_y = 29; return; }
					board[position_y+i][position_x] = *number;
					
					*number = *number + 1;
				}
			}
			else {
				for (int i = -1; i >= distance; i--) {
					if (position_y + i <= 0) { position_y = 0; return; }
					board[position_y+i][position_x] = *number;
					
					*number = *number + 1;
				}
			}
			position_y += distance;
		}
	}
	void draw_map() {
		system("cls");
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				cout << board[i][j] << "   ";
			}
			cout << endl;
		}
	}
	void move_direct(int R_L) {
		int a;
		if (R_L) {			
			for (int i = 0; i < 30; i++) {
				a = board[i][29];
				for (int j = 29; j > 0; j--) {
					board[i][j] = board[i][j - 1];
				}
				board[i][0]=a;
			}
		}
		else {
			for (int i = 0; i < 30; i++) {
				a = board[i][0];
				for (int j = 0; j < 29; j++) {
					board[i][j] = board[i][j + 1];
				}
				board[i][29] = a;
			}
		}
	}
};

int main(void) {
	make_road road;
	char option = ' ';
	while (option != 'q') {
		road.draw_map();
		cin >> option;
		switch (option) {
		case 'R':
			road.re_make();
			break;
		case 'r':
			road.move_direct(1);
			break;
		case 'l':
			road.move_direct(0);
			break;
		}
		
	}
}