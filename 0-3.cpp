#include <iostream>

using namespace std;
struct sentence {
	char letter;
	sentence* left,*right;
};
struct paragraph {
	sentence* st;
	paragraph* next;
};
class data_read {
private:
	paragraph* Para;
public:
	data_read() {
		Para = new paragraph;
		Para->next = NULL;
		Para->st = NULL;
		FILE* fp = NULL;
		if (0 == fopen_s(&fp, "data.txt", "r")) {
			char text = ' ';
			while (feof(fp) == 0) {//32/127
				fscanf_s(fp, "%c", &text);
				if (Para->st == NULL) {
					Para->st = new sentence;
					Para->st->letter = text;
					Para->st->right = Para->st;
					Para->st->left = Para->st;
				}
				else {

				}
			}
			
			fclose(fp);
		}
	}
	void print_data_read() {
		
	}
};
int main(void) {
	data_read TXTFILE;
	TXTFILE.print_data_read();
}