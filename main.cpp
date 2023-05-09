#include "tree.h"

/*
#include <fstream>

ifstream dat;
dat.open("datoteka.dat");
if( ! dat.is_open() ) greska();
char niz[20];
dat >> niz;
dat.close();
*/

int main() {
	Tree* t = nullptr;
	string s;
	while (true) {
		cout << "\t---MENI---\t\n"
			"Izaberite jednu od sledecih opcija\n"
			"1)Kreiranje objekta stabla\n"
			"2)Brisanje objekta stabla\n"
			"3)Pronalazenje kljuca u stablu\n"
			"4)Ispis stabla\n"
			"5)Umetanje kljuca u stablo\n"
			"6)Brisanje kljuca iz stabla\n"
			"7)Za prosledjeni kljuc X, ispisati koliko postoji kljuceva u stablu koji su leksikografski manji od prosledjenog X\n"
			"8)Kraj programa :(\n";
		int option;
		cout << endl << "Unesi zeljenu opciju: ";
		cin >> option;

		if (option > 8 || option < 1) {
			cout << "LOS UNOS-POKUSAJ OPET\n";
		}

		int indEOP = 0;

		switch (option) {
		case 1:
			cout << "Unesi dimenziju stabla m (3-10):  ";
			int m;
			cin >> m;
			if (m < 3 || m > 10) {
				cout << "DIMENZIJA NIJE DOBRO UNETA" << endl;
				continue;
			}
			t = new Tree(m);
			break;
		case 2:
			if (t)delete t;
			break;
		case 3:
			cout << "Unesite kljuc koji zelite da unesete u stablo:  ";
			cin >> s;
			t->isInTree(s);
			break;
		case 4:
			if(t)cout << *t << endl;
			break;
		case 5:
			cout << "Unesite vrednost kljuca koji zelite da unesete u stablo:  ";
			cin >> s;
			if (!t->insertBool(s)) {
				cout << "Ovaj kljuc vec postoji u stablu. " << endl;
			}
			else {
				cout << "Kljuc je dodat u stablo. " << endl;
			}
			break;
		case 6:
			cout << "Unesite vrednost kljuca koji zelite da obrisete iz stabla:  ";
			cin >> s;
			if (!t->removeBool(s)) {
				cout << "Ovaj kljuc vec postoji u stablu. " << endl;
			}
			else {
				cout << "Ovaj kljuc ne postoji u stablu. (Ovaj zapis je van fje) " << endl;
			}
			break;
		case 7:
			cout << "Unesite kljuc:  ";
			cin >> s;
			cout << "Broj manjih je: " << t->howManyLesserThen(s) << endl;
			break;
		case 8:
			indEOP = 1;
			break;
		default:
			continue;
		}
		if (indEOP)break;
	}
//
	/*t1.insert("a");
	t1.insert("b");
	t1.insert("c");
	t1.insert("d");
	t1.insert("e");
	t1.insert("f");
	t1.insert("g");
	t1.insert("h");
	t1.insert("i");
	t1.insert("j");
	t1.insert("k");
	t1.insert("l");
	t1.insert("m");
	t1.insert("n");
	cout << t1;
	t1.insert("o");
	cout << t1;
	t1.insert("p");
	cout << t1;
	t1.insert("q");
	cout << t1;
	t1.insert("r");
	cout << t1;
	t1.insert("s");
	cout << t1;
	t1.insert("t");
	cout << t1;
	t1.insert("u");
	cout << t1;
	t1.insert("v");
	cout << t1;
	t1.insert("w");
	cout << t1;
	t1.insert("x");
	cout << t1;
	t1.insert("y");
	cout << t1;
	t1.insert("z");
	cout << t1;*/
//
	/*string s = "C";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "d";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "e";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "f";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "g";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "h";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "i";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "j";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "k";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "l";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "m";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "n";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "o";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	 s = "p";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "q";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "r";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "s";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "t";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "u";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "v";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "x";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "y";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";

	s = "z";
	cout << "karakter: " << s << " ";
	cout << t1.isInTree(s) << endl;
	s = "";*/

	//cout << t1.howManyLesserThen("n");
//
	//Tree t1(4);
	//t1.insert("z");
	//t1.insert("y");
	//t1.insert("x");
	//t1.insert("w");
	//t1.insert("v");
	//t1.insert("u");
	////cout << t1;
	//t1.insert("t");
	//t1.insert("s");
	//t1.insert("r");
	//t1.insert("q");
	//t1.insert("p");
	//t1.insert("o");
	//t1.insert("n");
	//t1.insert("m");
	//t1.insert("l");
	//t1.insert("k");
	////cout << t1;
	//t1.insert("j");
	//t1.insert("i");
	//t1.insert("h");
	//t1.insert("g");
	//t1.insert("f");
	//t1.insert("e");
	//cout << t1;
	//t1.insert("d");
	//t1.insert("c");
	//t1.insert("b");
	////t1.insert("a");
	//cout << t1;

	////leva pozajmica
	//t1.remove("g");
	//cout << t1;
	//t1.remove("h");
	//cout << t1;
//
	//desna pozajmica
	/*t1.remove("d");
	cout << t1;
	t1.remove("c");
	cout << t1;*/

	return 0;
}
