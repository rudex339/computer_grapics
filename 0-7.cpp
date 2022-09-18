#include <iostream>
#include <random>
#include <Windows.h>

using namespace std;
random_device rd;

class card_game {
private:
	char board[4][4];
	int check[4][4];

	int turn;
	int A_score, B_score;
	int count;
public:
	card_game() {
		count = 20;
		turn = 1;
		A_score = 0; B_score = 0;
		uniform_int_distribution<int> uid(0, 3);
		int x, y;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				check[i][j] = 0;
				board[i][j] = ' ';
			}
		}
		for (int i = 0; i < 8; i++) {//65
			while (1) {
				x = uid(rd);
				y= uid(rd);
				if (board[x][y] == ' ') {
					board[x][y] = 65 + i;
					break;
				}
			}
			while (1) {
				x = uid(rd);
				y = uid(rd);
				if (board[x][y] == ' ') {
					board[x][y] = 65 + i;
					break;
				}
			}
		}

	}
	void print_board() {
		system("cls");
		
		cout << "  A B C D" << endl;
		for (int i = 0; i < 4; i++) {
			cout << i << " ";
			for (int j = 0; j < 4; j++) {
				if (check[i][j]) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (short)board[i][j]);
					cout << board[i][j];					
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else {
					cout << "*";
				}
				cout << " ";
			}
			cout << endl;
		}
		if (turn) {
			cout<<"turn= A"<<endl;
		}
		else {
			cout << "turn= B" << endl;
		}
		cout << "A= " << A_score << ", " << "B= " << B_score << endl;
	}

	void play_game() {
		if (count <= 0) {
			cout << "A= " << A_score << ", " << "B= " << B_score << endl;
			if (A_score > B_score) {
				cout << "A WIN" << endl;
			}
			else if (A_score < B_score) {
				cout << "B WIN" << endl;
			}
			else {
				cout << "DRAW" << endl;
			}
		}
		char coice1[2];
		char coice2[2];
		int x, y;
		while (1) {
			cout << "choice1 ex)(a,1): ";
			cin >> coice1[0];
			if (coice1[0] == 'r') {
				card_game();
				print_board();
				return;
			}
			cin >> coice1[1];
			
			if (check[(int)coice1[1] - 48][(int)coice1[0] - 97])
				cout << "잘못된 선택입니다" << endl;
			else {
				check[(int)coice1[1] - 48][(int)coice1[0] - 97] = 1;
				break;
			}
		}
		print_board();
		while (1) {
			cout << "choice2 ex)(a,1): ";
			cin >> coice2[0];
			if (coice2[0] == 'r') {
				card_game();
				print_board();
				return;
			} 
			cin>> coice2[1];
			if (check[(int)coice2[1] - 48][(int)coice2[0] - 97])
				cout << "잘못된 선택입니다" << endl;
			else {
				check[(int)(coice2[1] - 48)][(int)(coice2[0] - 97)] = 1;
				break;
			}
		}
		print_board();
		if (board[(int)coice1[1] - 48][(int)coice1[0] - 97] == board[(int)coice2[1] - 48][(int)coice2[0] - 97]) {
			if (turn) {
				A_score++;
			}
			else {
				B_score++;
			}
		}
		else {
			check[(int)coice1[1] - 48][(int)coice1[0] - 97] = 0;
			check[(int)coice2[1] - 48][(int)coice2[0] - 97] = 0;
		}

		if (turn) {
			turn = 0;
		}
		else {
			turn = 1;
		}
		count--;
	}

};
int main(void) {
	card_game game;
	game.print_board();
	while (1) {
		game.play_game();
	}

}