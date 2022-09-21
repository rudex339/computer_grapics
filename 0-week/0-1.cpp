#include <iostream>
#include <random>
using namespace std;
random_device rd;
void print_row(int(*row)[3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << row[i][j] << " ";
		}
		cout << endl;
	}
	return;
}
int main(void) {
	uniform_int_distribution<int> rando(0, 2);
	int row[3][3], row2[3][3];
	char opton = ' ';
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			row[i][j] = rando(rd);
			row2[i][j] = rando(rd);
		}
	}
	print_row(row);
	cout << endl;
	print_row(row2);
	while (opton != 'q') {
		cin >> opton;
		switch (opton) {
		case 'm':
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << row[i][j] + row2[i][j] << " ";
				}
				cout << endl;
			}
			break;
		case 'a':
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << row[i][0] * row2[0][j] + row[i][1] * row2[1][j] + row[i][2] * row2[2][j] << " ";
				}
				cout << endl;
			}
			break;
		case 'd':
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << row[i][j] - row2[i][j] << " ";
				}
				cout << endl;
			}
			break;
		case 'r':
			cout << row[0][0] * row[1][1] * row[2][2]
				+ row[0][1] * row[1][2] * row[2][0]
				+ row[0][2] * row[1][0] * row[2][1]
				- row[0][0] * row[1][2] * row[2][2]
				- row[0][1] * row[1][0] * row[2][1]
				- row[0][2] * row[1][1] * row[2][0];

			cout << endl;
			cout << row2[0][0] * row2[1][1] * row2[2][2]
				+ row2[0][1] * row2[1][2] * row2[2][0]
				+ row2[0][2] * row2[1][0] * row2[2][1]
				- row2[0][0] * row2[1][2] * row2[2][2]
				- row2[0][1] * row2[1][0] * row2[2][1]
				- row2[0][2] * row2[1][1] * row2[2][0];

			cout << endl;
			break;
		case 't':
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << row[j][i] << " ";
				}
				cout << endl;
			}
			cout << row[0][0] * row[1][1] * row[2][2]
				+ row[1][0] * row[2][1] * row[0][2]
				+ row[2][0] * row[0][1] * row[1][2]
				- row[0][0] * row[2][1] * row[2][2]
				- row[1][0] * row[0][1] * row[1][2]
				- row[2][0] * row[1][1] * row[0][2];
			cout << endl << endl;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << row2[j][i] << " ";
				}
				cout << endl;
			}
			cout << row2[0][0] * row2[1][1] * row2[2][2]
				+ row2[1][0] * row2[2][1] * row2[0][2]
				+ row2[2][0] * row2[0][1] * row2[1][2]
				- row2[0][0] * row2[2][1] * row2[2][2]
				- row2[1][0] * row2[0][1] * row2[1][2]
				- row2[2][0] * row2[1][1] * row2[0][2];
			cout << endl;
			break;
		case 'h':
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == 3 && j == 3)
						cout << 1 << " ";
					else if (i == 3)
						cout << 0 << " ";
					else if (j == 3)
						cout << 0 << " ";
					else
						cout << row[i][j] << " ";
				}
				cout << endl;
			}
			cout << row[0][0] * row[1][1] * row[2][2]
				+ row[1][0] * row[2][1] * row[0][2]
				+ row[2][0] * row[0][1] * row[1][2]
				- row[0][0] * row[2][1] * row[2][2]
				- row[1][0] * row[0][1] * row[1][2]
				- row[2][0] * row[1][1] * row[0][2] << endl;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == 3 && j == 3)
						cout << 1 << " ";
					else if (i == 3)
						cout << 0 << " ";
					else if (j == 3)
						cout << 0 << " ";
					else
						cout << row2[i][j] << " ";
				}
				cout << endl;
			}
			cout << row2[0][0] * row2[1][1] * row2[2][2]
				+ row2[1][0] * row2[2][1] * row2[0][2]
				+ row2[2][0] * row2[0][1] * row2[1][2]
				- row2[0][0] * row2[2][1] * row2[2][2]
				- row2[1][0] * row2[0][1] * row2[1][2]
				- row2[2][0] * row2[1][1] * row2[0][2];
			cout << endl;
			break;
		case 's':
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					row[i][j] = rando(rd);
					row2[i][j] = rando(rd);
				}
			}
			print_row(row);
			cout << endl;
			print_row(row2);
			break;
		}
	}
	return 0;
}
