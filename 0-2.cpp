#include <iostream>

using namespace std;

class data_read {
private:
	int CW;
	int NC;
	int WC;
public:
	data_read() {
		CW = 0; NC = 0; WC = 0;
		FILE* fp = fopen("data.txt", "r");
		int check_word{0}, check_capital{0}, check_number{0};
		char text =' ';
		while (feof(fp) == 0) {//32/127
			fscanf(fp, "%s", &text);
			if (32 < text && text < 127) {
				if (check_word == 0) {
					check_word = 1;
				}
				if (64 < text&& text < 91) {

				}
			}
			else if(check_word !=0) {
				CW++;
				check_word = 0;
			}

		}
		fclose(fp); 
	}
};
int main(void) {

}