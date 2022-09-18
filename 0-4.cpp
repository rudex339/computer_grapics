#include <iostream>
#include <random>
using namespace std;
random_device rd;

int main(void) {
	uniform_int_distribution<int> uid(600, 800);

	int rect1_x[2], rect1_y[2];
	int rect2_x[2], rect2_y[2];

	rect1_x[0] = uid(rd);
	rect1_y[0] = uid(rd);
	rect1_x[1] = rect1_x[0]+100;
	rect1_y[1] = rect1_x[0]+100;

	rect2_x[0] = uid(rd);
	rect2_y[0] = uid(rd);
	rect2_x[1] = rect1_x[0] + 100;
	rect2_y[1] = rect1_x[0] + 100;

	int turn = 0;
	char command = ' ';
	while (command != 'q') {
		if ((rect1_x[0] > rect2_x[1]) && (rect1_y[1] > rect2_y[0]) && (rect1_x[1] < rect2_x[0]) && (rect1_y[0] < rect2_y[1])) {
			cout << "Rectangle 1 & Rectangle 2 collide!!" << endl;
			break;
		}
		cout << "RECT1= ";
		cout << "(" << rect1_x[0] << ", " << rect1_x[1] << ") ";
		cout << "(" << rect1_y[0] << ", " << rect1_y[1] << ") " << endl;
		cout << "RECT2= ";
		cout << "(" << rect2_x[0] << ", " << rect2_x[1] << ") ";
		cout << "(" << rect2_y[0] << ", " << rect2_y[1] << ") " << endl;

		cout << "input : ";
		cin >> command;

		switch (command) {
		case 'w':
			if (turn) {
				rect1_y[0] += 20;
				rect1_y[1] += 20;
				turn = 0;
			}
			else {
				rect2_y[0] += 20;
				rect2_y[1] += 20;
				turn = 1;
			}
			break;
		case 's':
			if (turn) {
				rect1_y[0] -= 20;
				rect1_y[1] -= 20;
				turn = 0;
			}
			else {
				rect2_y[0] -=20;
				rect2_y[1] -= 10;
				turn = 1;
			}
			break;
		case 'a':
			if (turn) {
				rect1_x[0] -= 20;
				rect1_x[1] -= 20;
				turn = 0;
			}
			else {
				rect2_x[0] -= 20;
				rect2_x[1] -= 20;
				turn = 1;
			}
			break;
		case 'd':
			if (turn) {
				rect1_x[0] += 20;
				rect1_x[1] += 20;
				turn = 0;
			}
			else {
				rect2_x[0] += 20;
				rect2_x[1] += 20;
				turn = 1;
			}
			break;
		}
	}
	
}