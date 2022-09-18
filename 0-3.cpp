#include <iostream>

using namespace std;
struct sentence {
	char letter;
	int mode_E;
	sentence* left,*right;
};
struct paragraph {
	sentence* st;
	paragraph* next;
};
class data_read {
private:
	paragraph* Para;
	int direction;
	int mode_E;
public:
	data_read() {
		direction = 1;	mode_E = 0;
		Para = new paragraph;
		paragraph* Ppoint=Para;
		sentence* NEW;
		Para->next = NULL;
		Para->st = NULL;
		FILE* fp = NULL;
		if (0 == fopen_s(&fp, "data.txt", "r")) {
			char text = ' ';
			while (feof(fp) == 0) {//32/127
				fscanf_s(fp, "%c", &text,sizeof(text));
				if (Ppoint->st == NULL) {
					Ppoint->st = new sentence;
					Ppoint->st->letter = text;
					Ppoint->st->mode_E = 0;
					Ppoint->st->right = Ppoint->st;
					Ppoint->st->left = Ppoint->st;
				}
				else {
					NEW = new sentence;
					NEW->letter = text;
					NEW->mode_E = 0;
					NEW->right = Ppoint->st->right;
					NEW->left = Ppoint->st;
					Ppoint->st->right->left = NEW;
					Ppoint->st->right = NEW;

					Ppoint->st = Ppoint->st->right;
				}
				
				if (text == '\n') {
					Ppoint->next = new paragraph;
					Ppoint = Ppoint->next;
					Ppoint->st = NULL;
					Ppoint->next = NULL;
				}

			}			
			fclose(fp);
		}
	}
	void print_data_read() {
		paragraph* Ppoint = Para;
		sentence* Ps,*NEW;

		while (Ppoint != NULL) {
			if (direction == 1) Ps = Ppoint->st->right;
			else Ps = Ppoint->st->left;

			while (Ps != Ppoint->st) {
				cout << Ps->letter;

				if (direction == 1) Ps = Ps->right;
				else Ps = Ps->left;

			}
			cout << "\n";
			Ppoint = Ppoint->next;
		}
	}
	void turn_mode(char option) {
		paragraph* Ppoint = Para;
		sentence* Ps;
		switch (option) {
		case 'd':
			direction *= -1;
			this->print_data_read();
			break;
		case 'e':
			if (mode_E) {
				sentence* Die;

				while (Ppoint != NULL) {
					Ps = Ppoint->st->right;
					while (Ps != Ppoint->st) {
						{
							if (Ps->mode_E) {
								Ps->left->right = Ps->right;
								Ps->right->left = Ps->left;
								Die = Ps;
								Ps = Ps->right;
								free(Die);
							}
						}
						Ps = Ps->right;
					}
					Ppoint = Ppoint->next;
				}
				mode_E = 0;
			}
			else {
				sentence* NEW;
				int count=0;

				while (Ppoint != NULL) {
					Ps = Ppoint->st->right;
					while (Ps != Ppoint->st) {
						{
						count++;
						if (count == 3) {
							NEW = new sentence;
							NEW->letter = ' ';
							NEW->mode_E = 1;
							NEW->right = Ps->right;
							NEW->left = Ps;
							Ps->right->left = NEW;
							Ps->right = NEW;
							
							count = 0;
						}
						}
						Ps = Ps->right;
					}
					Ppoint = Ppoint->next;
				}

				mode_E = 1; 
			}

			this->print_data_read();
			break;
		case 'g':
			char a, b;
			cout << endl << "公均阑: ";
			cin >> a;
			cout << endl << "公均栏肺: ";
			cin >> b;
			
			

			while (Ppoint != NULL) {
				Ps = Ppoint->st->right;

				while (Ps != Ppoint->st) {					
					if (Ps->letter == a)
						Ps->letter = b;

					Ps = Ps->right;
				}
				Ppoint = Ppoint->next;
			}

			this->print_data_read();
			break;
		case 'f':
			sentence* rev;

			while (Ppoint != NULL) {
				if (direction == 1) Ps = Ppoint->st->right;
				else Ps = Ppoint->st->left;

				while (Ps != Ppoint->st) {
					if (Ps->letter == ' ' || Ps->letter == '\n') {
						if (direction == 1) rev = Ps->left;
						else rev = Ps->right;
						while (rev->letter != ' ' && rev->letter != '\n') {
							cout << rev->letter;
							if (direction == 1) rev = rev->left;
							else rev = rev->right;
						}
						cout << Ps->letter;
					}					
					if (direction == 1) Ps = Ps->right;
					else Ps = Ps->left;

				}
				cout << "\n";
				Ppoint = Ppoint->next;
			}
			break;
		case 'h':
			sentence * Ps2;
			while (Ppoint != NULL) {
				Ps = Ppoint->st->right;
				Ps2= Ppoint->st->left;

				while (Ps != Ps2&&Ps->letter== Ps2->letter) {
					cout << Ps->letter;

					Ps = Ps->right;
					Ps2 = Ps2->left;

				}
				cout << "\n";
				Ppoint = Ppoint->next;
			}
			break;
		}

	}
	
};
int main(void) {
	char option = ' ';
	data_read TXTFILE;
	TXTFILE.print_data_read();
	while (option != 'q') {
		cout << endl << "option: ";
		cin >> option;
		TXTFILE.turn_mode(option);
	}
}