#include <iostream>

using namespace std;

class data_read {
private:
	int CW;//대문자
	int NC;//숫자
	int WC;//글자
public:
	data_read() {
		CW = 0; NC = 0; WC = 0;
		FILE* fp = NULL;
		if (0==fopen_s(&fp, "data.txt", "r")) {
			int check_word{ 0 }, check_capital{ 0 }, check_number{ 0 };
			char text = ' ';
			while (feof(fp) == 0) {//32/127
				fscanf_s(fp, "%c", &text, sizeof(text));
				if (32 < text && text < 127) {
					if ('0' <= text && text <= '9') {
						check_number = 1;
					}
					else {
						if ('A' <= text && text <= 'Z')
							check_capital = 1;
						check_word = 1;
					}
				}
				else if (check_word == 1) {
					WC++;
					if (check_capital == 1) {
						CW++;
						check_capital = 0;
					}
					check_word = 0;
					if (check_number != 0)check_number = 0;
				}
				else if (check_number == 1) {
					NC++;
					check_number = 0;
				}
			}
			if (check_word == 1) {
				WC++;
				if (check_capital == 1) {
					CW++;
					check_capital = 0;
				}
				check_word = 0;
				if (check_number != 0)check_number = 0;
			}
			else if (check_number == 1) {
				NC++;
				check_number = 0;
			}
			fclose(fp);
		}
	}
	void print_data_read() {
		cout << "word count : " << WC << endl;
		cout << "number count : " << NC << endl;
		cout << "capital count : " << CW << endl;
	}
};
int main(void) {
	data_read TXTFILE;
	TXTFILE.print_data_read();
}