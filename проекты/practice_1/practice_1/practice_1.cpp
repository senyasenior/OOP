//Практическая работа 1, задания 1.1 и 1.2, вариант 16

#include <iostream>
#include <list>
using namespace std;


class Patient
{
public:
	string surname, name, birthday, phone, address;
	int card_id, bloody_type;

	Patient(string sr, string nm, string bd, string ph, string ad, int card, int blood)
	{
		surname, name, birthday, phone, address = sr, nm, bd, ph, ad;
		card_id, bloody_type = card, blood;
	}

	bool operator > (Patient element)
	{
		if (card_id < element.card_id) return true;
		if (bloody_type < element.bloody_type) return true;
		if (surname < element.surname) return true;
		if (name < element.name) return true;
		return false;
	}

	bool operator != (Patient element)
	{
		return surname != element.surname && name != element.name && birthday != element.birthday && phone != element.phone && address != element.address && card_id && element.card_id && bloody_type != element.bloody_type;
	}
	
	friend ostream& operator << (ostream& s, Patient& el);
};


ostream& operator <<(ostream& s, Patient& el)
{
	s << el.name;
	return s;
}


Patient pop(list<Patient>& lst)
{
	typename list<Patient>::iterator p = lst.begin();
	Patient res = *p;
	list<Patient> lst_res;
	while (p != lst.end())
	{
		p++;
		if (*p > res)res = *p;
	};

	p = lst.begin();
	while (p != lst.end())
	{
		if (*p != res) lst_res.push_back(*p);
	}
	lst = lst_res;
	return res;
}

template <class T>
void push(list<T>& lst, T element)
{
	typename list<T>::iterator<T> p = lst.begin();
	while (p != lst.end())
	{
		if (*p > element)
			break;
		p++;
	}
	lst.insert(p, element);
}

template <class T>
void pop(list<T>& lst, int index = -1)
{
	if (index == -1) return lst.pop_back();

	typename list<T>::iterator p = lst.begin();
	for (int i = 0; i < index; i++, p++);
	/*cout << "\n" << *p << "\n";*/
	lst.remove(*p);
	
}

template <class T>
list<T> filter(list<T> lst, bool (*f)(T))
{
	typename list<T>::iterator p;
	int index = 0;
	while (index < lst.size())
	{
		p = lst.begin();
		for (int i = 0; i < index; i++)p++;
		if (f(*p) == false) pop(lst, index);
		index++;
		
	}
	return lst;
}


bool consonant(char c)
{
	int con = 'a' - 'A';
	if (c >= 'A' && c <= 'Z') c += con;
	return c != 'a' && c != 'o' && c != 'u' && c != 'e' && c != 'i';
}

template <class T>
void print(list<T> lst)
{
	typename list<T>::iterator p = lst.begin();
	while (p != lst.end())
	{
		cout << *p;
		p++;
	}
}

int main()
{
	


	Patient p1("Lenin", "Arseniy", "20.05.2004", "89603183896", "Vernadskogo pr-t 86", 123, 4);
	Patient p2("Ivanov", "Ivan", "21.05.2004", "89633183896", "Vernadskogo pr-t 78", 122, 3);
	Patient p3("Sidorov", "Petr", "19.05.2004", "89003183896", "Strominka 20", 128, 3);

	list<Patient> patient_list;
	push(patient_list, p1);
	push(patient_list, p2);
	push(patient_list, p3);
	print(patient_list);

	cout << endl;
	Patient patient = pop(patient_list);
	cout << patient;
	
	char c;
	cin >> c;
	return 0;
}