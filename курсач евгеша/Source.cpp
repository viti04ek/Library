#include<iostream>
#include<Windows.h>
#include<fstream>
#include<vector>
#include<stdlib.h>
#include<conio.h>
#include<string>


using namespace std;


enum Color { Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue = 9, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };

enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Esc = 27, Space = 32 };


HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);


class functions
{
public:

	void set_color(Color text, Color background)
	{
		SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
	}

	void set_cursor(int x, int y)
	{
		COORD myCoords = { x,y };
		SetConsoleCursorPosition(hStdOut, myCoords);
	}


	void start_menu_buttons(int ch)
	{
		string buttons[] = { "ÐÎÇÏÎ×ÀÒÈ ÐÎÁÎÒÓ", "ÏÐÎ ÐÎÇÐÎÁÍÈÊÀ", "ÂÈÉÒÈ" };

		for (int i = 0; i < 3; i++)
		{
			set_cursor(45, 10 + 2 * i);

			if (i == ch)
				set_color(Black, Green);
			else
				set_color(White, Black);

			cout << buttons[i];
		}
	}

	int start_menu()
	{
		system("cls");

		int key = 1, change = 0, A = 0;
		int choice;

		start_menu_buttons(change);

		do
		{
			key = _getch();

			switch (key)
			{
			case Up:
				if (change > 0)
				{
					change--;
					start_menu_buttons(change);
				}
				break;

			case Down:
				if (change < 2)
				{
					change++;
					start_menu_buttons(change);
				}
				break;

			case Enter:
				do
				{
					switch (change)
					{
					case 0:
						choice = 0;
						A++;
						break;

					case 1:
						choice = 1;
						A++;
						break;

					case 2:
						choice = 2;
						A++;
						break;
					}
				} while (key != Enter);
			}

		} while (A < 1);
		
		
		return choice;
	}


	void about()
	{
		system("cls");

		set_cursor(30, 20);
		set_color(Black, Green);
		cout << "Created by Evgeniya Lubanevich, KA-11 IASA";

		int key = 1;

		while (true)
		{
			key = _getch();
			if (key == Esc)
			{
				system("color 07");
				return;
			}
		}

	}

}f;


class Info
{
private:

	int id;
	string name;
	string surname;
	vector<string> books;
	int debts;

public:

	Info()
	{
		this->id = 0;
		this->name = "-";
		this->surname = "-";
		this->debts = 0;
	}

	Info(int id, string name, string surname, vector<string> books, int debts)
	{
		this->id = id;
		this->name = name;
		this->surname = surname;
		this->books = books;
		this->debts = debts;
	}


	int get_id() { return id; }
	string get_name() { return name; }
	string get_surname() { return surname; }
	vector<string> get_books() { return books; }
	int get_debts() { return debts; }


	Info& mod_id(int id)
	{
		this->id = id;
		return *this;
	}
	Info& mod_name(string name)
	{
		this->name = name;
		return *this;
	}
	Info& mod_surname(string surname)
	{
		this->surname = surname;
		return *this;
	}
	Info& mod_books(string books)
	{
		this->books.push_back(books);
		return *this;
	}
	Info& mod_all_books(vector<string> vec)
	{
		this->books = vec;
		return *this;
	}
	Info& mod_debts()
	{
		int debts = 0;

		for (int i = 0; i < this->books.size(); i++)
			if (books[i][0] == '-')
				debts++;

		this->debts = debts;

		return *this;
	}


	vector<string> delete_element(vector<string> vec, int index)
	{
		vector<string>::iterator it_erase = vec.begin();
		it_erase += index;
		vec.erase(it_erase);
		return vec;
	}


	Info& operator=(Info& inf)
	{
		this->id = inf.get_id();
		this->name = inf.get_name();
		this->surname = inf.get_surname();
		this->books = inf.get_books();
		this->debts = inf.get_debts();

		return *this;
	}

};


class Library
{
private:

	Info* info;
	int size;

public:

	Library()
	{
		info = new Info[100];
		size = 0;
		
		get_from_file();
	}

	~Library()
	{
		delete[] info;
	}


	Info get_info() { return *info; }
	int get_size() { return size; }


	Library& mod_info(Info* info)
	{
		this->info = info;
		return *this;
	}
	Library& mod_size(int size)
	{
		this->size = size;
		return *this;
	}


	Info& operator[](int index)
	{
		return info[index];
	}


	void get_from_file()
	{
		ifstream F;
		F.open("info.txt", ifstream::app);

		int n = 0;
		string temp;

		getline(F, temp);

		while (!F.eof())
		{
			Info inf;

			string num = to_string(n);

			inf.mod_id(n);


			getline(F, temp);
			inf.mod_name(temp);

			getline(F, temp);
			inf.mod_surname(temp);

			num = to_string(n + 1);
			while (getline(F, temp))
			{
				if (temp == num || temp == "")
					break;

				inf.mod_books(temp);
			}

			inf.mod_debts();

			size++;
			info[size - 1] = inf;
			
			n++;

			if (temp == "")
				break;
		}

		F.close();
	}

	void set_in_file()
	{
		ofstream F;
		F.open("info.txt", ofstream::out | ofstream::trunc);

		for (int n = 0; n < size; n++)
		{
			F << info[n].get_id() << endl;
			F << info[n].get_name() << endl;
			F << info[n].get_surname() << endl;

			for (int i = 0; i < info[n].get_books().size(); i++)
				F << info[n].get_books()[i] << endl;
		}

		F.close();
	}


	void build_row(int ch, int index, int i)
	{
		f.set_cursor(18, 4 + 2 * (i + 1));
		f.set_color(White, Black);
		cout << "+===============================================+";

		f.set_cursor(18, 5 + 2 * (i + 1));
		cout << "#\t" << info[index].get_id() << " #\t";

		if (i == ch)
			f.set_color(Black, Green);
		else
			f.set_color(White, Black);
		cout << info[index].get_name() << "\t" << info[index].get_surname();
		f.set_color(White, Black);

		f.set_cursor(56, 5 + 2 * (i + 1));
		cout << "#";

		cout << "\t" << info[index].get_debts() << " #";
	}

	void menu_buttons(int ch)
	{
		system("cls");

		string buttons[] = { "ÏÎØÓÊ", "ÄÎÄÀÒÈ ÂIÄÂIÄÓÂÀ×À" };

		for (int i = 0; i < 2; i++)
		{
			f.set_cursor(30, 2 + 2 * i);

			if (i == ch)
				f.set_color(Black, Green);
			else
				f.set_color(White, Black);

			cout << buttons[i];
		}

		f.set_color(White, Black);
		f.set_cursor(18, 8);
		cout << "+===============================================+";
		f.set_cursor(18, 9);
		cout << "#  id  " << " #\t ²ì'ÿ òà ïð³çâèùå \t#" << "  Áîðã   #";


		int i;
		for (i = 0; i < size; i++)
			build_row(ch, i, i + 2);

		f.set_cursor(18, 4 + 2 * (i + 3));
		cout << "+===============================================+";

	}

	int menu()
	{
		system("cls");

		int key = 1, change = 0, A = 0;

		menu_buttons(change);
		do
		{
			key = _getch();

			switch (key)
			{
			case Up:
				if (change > 0)
				{
					change--;
					menu_buttons(change);
				}
				break;

			case Down:
				if (change < 1 + size)
				{
					change++;
					menu_buttons(change);
				}
				break;

			case Enter: A++; break;

			case Esc: return -1;
				
			}

		} while (A < 1);

		return change;
	}


	void profile_buttons(int ch, int m_id)
	{
		string button = "ÂÈÄÀËÈÒÈ ÊÎÐÈÑÒÓÂÀ×À";

		int k = 0;


		f.set_cursor(30, 2);

		if (k == ch)
			f.set_color(Black, Green);
		else
			f.set_color(White, Black);
		cout << button;

		k++;


		f.set_cursor(30, 4);
		f.set_color(White, Black);
		cout << "²Ì'ß: ";
		if (k == ch)
			f.set_color(Black, Green);
		else
			f.set_color(White, Black);
		cout << info[m_id].get_name();

		k++;


		f.set_cursor(30, 6);
		f.set_color(White, Black);
		cout << "ÏÐ²ÇÂÈÙÅ: ";
		if (k == ch)
			f.set_color(Black, Green);
		else
			f.set_color(White, Black);
		cout << info[m_id].get_surname();
		f.set_color(White, Black);

		k++;


		f.set_cursor(40, 10);
		f.set_color(White, Black);
		cout << "+===========================+";
		f.set_cursor(40, 11);
		cout << "#\tÍå ïîâåðíåí³ êíèæêè:";
		f.set_cursor(68, 11);
		cout << "#";
		f.set_cursor(40, 12);
		cout << "+===========================+";

		f.set_cursor(40, 13);
		cout << "#\t";
		if (k == ch)
			f.set_color(Black, Green);
		else
			f.set_color(White, Black);
		cout << "Äîäàòè êíèæêó";
		f.set_color(White, Black);
		f.set_cursor(68, 13);
		cout << "#";

		f.set_cursor(40, 14);
		cout << "+===========================+";

		k++;

		int counter = 15;
		for (int i = 0; i < info[m_id].get_books().size(); i++)
		{
			if (info[m_id].get_books()[i][0] == '-')
			{
				if (k == ch)
				{
					f.set_cursor(40, counter);
					cout << "#\t";
					f.set_color(Black, Green);

					for (int j = 1; j < info[m_id].get_books()[i].size(); j++)
						cout << info[m_id].get_books()[i][j];

					f.set_color(White, Black);
					f.set_cursor(68, counter);
					cout << "#";
					counter++;

					f.set_cursor(40, counter);
					cout << "+===========================+";
					counter++;
				}
				else
				{
					f.set_cursor(40, counter);
					f.set_color(White, Black);
					cout << "#\t";

					for (int j = 1; j < info[m_id].get_books()[i].size(); j++)
						cout << info[m_id].get_books()[i][j];

					f.set_cursor(68, counter);
					cout << "#";
					counter++;

					f.set_cursor(40, counter);
					cout << "+===========================+";
					counter++;
				}

				k++;
			}
		}
	}

	int profile(int m_id)
	{
		system("cls");

		f.set_cursor(4, 10);
		f.set_color(White, Black);
		cout << "+===========================+";
		f.set_cursor(4, 11);
		cout << "#\tÏîâåðíåí³ êíèæêè:";
		f.set_cursor(32, 11);
		cout << "#";
		f.set_cursor(4, 12);
		cout << "+===========================+";

		int counter = 13;
		for (int i = 0; i < info[m_id].get_books().size(); i++)
		{
			if (info[m_id].get_books()[i][0] == '+')
			{
				f.set_cursor(4, counter);
				cout << "#\t";

				for (int j = 1; j < info[m_id].get_books()[i].size(); j++)
					cout << info[m_id].get_books()[i][j];
				
				f.set_cursor(32, counter);
				cout << "#";
				counter++;

				f.set_cursor(4, counter);
				cout << "+===========================+";
				counter++;
			}
		}


		int key = 1, change = 0, A = 0;

		profile_buttons(change, m_id);
		do
		{
			key = _getch();

			switch (key)
			{
			case Up:
				if (change > 0)
				{
					change--;
					profile_buttons(change, m_id);
				}
				break;

			case Down:
				if (change < 3 + info[m_id].get_debts())
				{
					change++;
					profile_buttons(change, m_id);
				}
				break;

			case Enter: A++; break;

			case Esc: return -1;

			}

		} while (A < 1);

		return change;
	}

	
	void search_buttons(int ch, vector<int> suitable)
	{
		f.set_color(White, Black);
		f.set_cursor(18, 8);
		cout << "+===============================================+";
		f.set_cursor(18, 9);
		cout << "#  id  " << " #\t ²ì'ÿ òà ïð³çâèùå \t#" << "  Áîðã   #";


		int i;
		for (i = 0; i < suitable.size(); i++)
			build_row(ch + 2, suitable[i], i + 2);

		f.set_cursor(18, 4 + 2 * (i + 3));
		cout << "+===============================================+";
	}

	int search()
	{
		system("cls");

		f.set_cursor(30, 2);
		cout << "Ââåä³òü ïîøóêîâèé çàïèò:";

		string request;
		f.set_cursor(30, 3);
		cin >> request;

		vector<int> suitable;
		
		for (int i = 0; i < size; i++)
			if (request == info[i].get_name() || request == info[i].get_surname())
				suitable.push_back(info[i].get_id());


		system("cls");

		f.set_cursor(30, 2);
		cout << "Ââåä³òü ïîøóêîâèé çàïèò:";

		f.set_cursor(30, 3);
		cout<< request;

		f.set_cursor(30, 5);
		cout << "Ðåçóëüòàòè ïîøóêó:";

		int key = 1, change = 0, A = 0;

		search_buttons(change, suitable);
		do
		{
			key = _getch();

			switch (key)
			{
			case Up:
				if (change > 0)
				{
					change--;
					search_buttons(change, suitable);
				}
				break;

			case Down:
				if (change < suitable.size() - 1)
				{
					change++;
					search_buttons(change, suitable);
				}
				break;

			case Enter: A++; break;

			}

		} while (A < 1);

		return suitable[change];
	}


	void add_user()
	{
		f.set_cursor(30, 4);
		cout << "                                         ";

		string n_name, n_surname;

		f.set_cursor(30, 4);
		cout << "Ââåä³òü äàí³ íîâîãî êîðèñòóâà÷à:";

		f.set_cursor(30, 5);
		cout << "²ì'ÿ: ";
		cin >> n_name;

		f.set_cursor(30, 6);
		cout << "Ïð³çâèùå: ";
		cin >> n_surname;


		info[size].mod_id(info[size - 1].get_id() + 1);
		info[size].mod_name(n_name);
		info[size].mod_surname(n_surname);
		info[size].mod_debts();
		size++;
	}

	void delete_user(int m_id)
	{
		for (int i = m_id + 1; i < size; i++)
		{
			info[i].mod_id(info[i].get_id() - 1);
			info[i - 1] = info[i];
		}

		size--;
	}


	void change_name(int m_id)
	{
		string n_name;

		f.set_cursor(30, 4);
		cout << "                               ";

		f.set_cursor(30, 4);
		cout << "Ââåä³òü íîâå ³ì'ÿ: ";
		cin >> n_name;

		info[m_id].mod_name(n_name);
	}

	void change_surname(int m_id)
	{
		string n_surname;

		f.set_cursor(30, 6);
		cout << "                               ";

		f.set_cursor(30, 6);
		cout << "Ââåä³òü íîâå ïð³çâèùå: ";
		cin >> n_surname;

		info[m_id].mod_surname(n_surname);
	}


	void add_book(int m_id)
	{
		string book;


		f.set_cursor(41, 13);
		cout << "                    ";

		f.set_cursor(42, 13);
		getline(cin, book);

		book = '-' + book;

		info[m_id].mod_books(book);
		info[m_id].mod_debts();
	}


	void turn_book(int m_id, int num)
	{
		int counter = 0;
		int index = 0;
		int element = 0;

		for (int i = 0; i < info[m_id].get_books().size(); i++)
		{
			if (info[m_id].get_books()[i][0] == '-')
				counter++;

			if (counter == num)
			{
				element = i;
				break;
			}

			index++;
		}

		string temp = info[m_id].get_books()[element];
		temp[0] = '+';
		info[m_id].mod_books(temp);
		info[m_id].mod_all_books(info[m_id].delete_element(info[m_id].get_books(), index));
	}

};


int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	while (true)
	{
		int choice = f.start_menu();

		while (choice != 0)
		{
			if (choice == 1)
				f.about();

			if (choice == 2)
				return 0;

			choice = f.start_menu();
		}

		Library lab;

		bool menu = false;

		while (menu != true)
		{
			choice = lab.menu();

			switch (choice)
			{
			case 1: lab.add_user(); break;

			case -1: menu = true; break;

			case 0: choice = lab.search() + 2;

			default:
				int choice1;
				bool menu1 = false;

				while (menu1 != true)
				{
					choice1 = lab.profile(choice - 2);

					switch (choice1)
					{
					case 0: lab.delete_user(choice - 2); menu1 = true;  break;

					case 1: lab.change_name(choice - 2); break;

					case 2: lab.change_surname(choice - 2); break;

					case 3: lab.add_book(choice - 2); break;

					case -1: menu1 = true; break;

					default: lab.turn_book(choice - 2, choice1 - 3); break;

					}
				}
			}

			lab.set_in_file();
		}

	}
}