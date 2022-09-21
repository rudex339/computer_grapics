#include <iostream>

using namespace std;

//struct list {
//	int x, y, z;
//	list* next,*prev;
//};
//int main(void) {
//	list* st=NULL,*NEW;
//	int count{ 0 };
//
//	char command = ' ';
//
//	while (command != 'q') {
//		cout << "command: ";
//		cin >> command;
//
//		switch (command) {
//		case '+':
//			if (count == 0) {
//				st = new list;
//				st->next = st;
//				st->prev = st;
//				cout << "x= ";
//				cin >> st->x;
//				cout << "y= ";
//				cin >> st->y;
//				cout << "z= ";
//				cin >> st->z;
//
//				count++;
//			}
//			else {
//				NEW = new list;
//				NEW->next = st->next;
//				NEW->prev = st;
//				st->next->prev = NEW;
//				st->next = NEW;
//
//				cout << "x= ";
//				cin >> NEW->x;
//				cout << "y= ";
//				cin >> NEW->y;
//				cout << "z= ";
//				cin >> NEW->z;
//
//				st = st->next;
//				count++;
//			}
//			break;
//		case '-':
//			if (count != 0) {
//				NEW = st;
//				st = st->prev;
//				st->next = NEW->next;
//				NEW->next->prev = st;
//				count--;
//
//			}
//			else
//				cout << "리스트가 비어있습니다" << endl;
//			break;
//		}
//		NEW = st->next;
//		for (int i = 0; i < 10; i++) {
//			cout << i;
//			if (i < count) {
//				cout << "(" << NEW->x << " " << NEW->y << " " << NEW->z << ")";
//				NEW = NEW->next;
//			}
//			cout<< endl;
//		}
//	}
//}

struct list {
	int x, y, z;
	int on;
};
void sort_list(list List[10]) {
	list sort[10];
	int count = 0,sort_count=0, check;
	for (int i = 0; i < 10; i++) {
		if (List[i].on) count++;
		sort[i].on = 0;
	}
	for (int i = 0; i < 10; i++) {
		if (List[i].on) {
			sort[sort_count].x = List[i].x;
			sort[sort_count].y = List[i].y;
			sort[sort_count].z = List[i].z;
			sort[sort_count].on = 1;
			sort_count++;
		}
	}

	for (int i = 0; i < count; i++) {
		for (int j = i+1; j < count; j++) {
			if (sort[i].x * sort[i].x + sort[i].y * sort[i].y + sort[i].z * sort[i].z > sort[j].x * sort[j].x + sort[j].y * sort[j].y + sort[j].z * sort[j].z) {
				check = sort[i].x;
				sort[i].x = sort[j].x;
				sort[j].x = check;
				check = sort[i].y;
				sort[i].y = sort[j].y;
				sort[j].y = check;
				check = sort[i].z;
				sort[i].z = sort[j].z;
				sort[j].z = check;
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		cout << i;
		if (sort[i].on) {
			cout << "(" << sort[i].x << " " << sort[i].y << " " << sort[i].z << ")";
		}
		cout << endl;
	}

}
int main(void) {
	list List[10];
	int count{ 0 };
	int check, leng{0};
	for (int i = 0; i < 10; i++) {
		List[i].on = 0;
	}
	char command = ' ';

	while (command != 'q') {
		cout << "command: ";
		cin >> command;

		switch (command) {
		case '+':
			if (count < 10) {
				cout << "x= ";
				cin >> List[count].x;
				cout << "y= ";
				cin >> List[count].y;
				cout << "z= ";
				cin >> List[count].z;
				List[count].on = 1;
				count++;
			}
			else {
				if (List[0].on == 0) {
					cout << "x= ";
					cin >> List[0].x;
					cout << "y= ";
					cin >> List[0].y;
					cout << "z= ";
					cin >> List[0].z;
					List[0].on = 1;
				}
			}
				
			break;
		case '-':
			if (count > 0) {
				count--;
				List[count].on = 0;
			}
			else
				cout << "리스트가 비었습니다" << endl;
			break;
		case 'e':
			if (count < 10) {
				for (int i = count; i > 0; i--) {
					List[i].x = List[i - 1].x;
					List[i].y = List[i - 1].y;
					List[i].z = List[i - 1].z;
				}
				List[count].on = 1;
				cout << "x= ";
				cin >> List[0].x;
				cout << "y= ";
				cin >> List[0].y;
				cout << "z= ";
				cin >> List[0].z;
				List[0].on = 1;
				count++;
			}
			break;
		case 'd':
			if (count > 0) {
				for (int i = 0; i < 10; i++) {
					if (List[i].on) { 
						List[i].on = 0; 
						break;
					}
				}
				count--;
			}
			break;
		case 'l':
			check = 0;
			for (int i = 0; i <10; i++) {
				if (List[i].on) {
					check++;
				}
			}
			cout << check << endl<<endl;
			break;
		case 'c':
			for (int i = 10; i >= 0; i--) {
				List[i].on = 0;
			}
			count = 0;
			break;
		case 'm':
			check = count-1;
			for (int i = 0; i <10; i++) {
				if (List[i].on) {
					if (List[check].x* List[check].x + List[check].y * List[check].y+ List[check].z* List[check].z < List[i].x* List[i].x + List[i].y* List[i].y + List[i].z* List[i].z) {
						check = i;
					}
				}
			}
			cout << "(" << List[check].x << " " << List[check].y << " " << List[check].z << ")"<<endl;
			break;
		case 'n':
			check = count - 1;
			for (int i = 0; i < 10; i++) {
				if (List[i].on) {
					if (List[check].x * List[check].x + List[check].y * List[check].y + List[check].z * List[check].z > List[i].x * List[i].x + List[i].y * List[i].y + List[i].z * List[i].z) {
						check = i;
					}
				}
			}
			cout << "(" << List[check].x << " " << List[check].y << " " << List[check].z << ")" << endl<<endl;
			break;
		case 's':
			sort_list(List);
			break;
		}
		
		for (int i = 0; i < 10; i++) {
			cout << i;
			if (List[i].on) {
				cout << "(" << List[i].x << " " << List[i].y << " " << List[i].z << ")";
				}
			cout << endl;
		}
	}
}