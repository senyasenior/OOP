//Практическая работа 2, вариант 16

using namespace std;
#include <map>
#include <iostream>
#include<set>
#include<list>



class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};


class InvalidKeyException : public Exception
{
protected:
	string key_value;
public:
	InvalidKeyException(const char* str, string kv) : Exception(str) { key_value = kv; };
	virtual void print()
	{
		cout << "InvalidKeyException: " << str << "; Key_value: " << key_value<< "; " << what();
	}

};


class Patient
{
public:
	string surname, birthday, phone, address;
	string name, card_id;
	int bloody_type;

	Patient(string sr, string nm, string bd, string ph, string ad, string card, int blood)
	{
		surname = sr;
		birthday = bd;
		phone = ph;
		address = ad;
		name = nm;
		card_id = card;
		bloody_type = blood;
	}

	/*Patient(Patient& element)
	{
		surname = element.surname;
		name = element.name;
		birthday = element.birthday;
		phone = element.phone;
		address = element.address;
		card_id = element.card_id;
		bloody_type = element.bloody_type;
	}*/


	bool operator > (Patient& element)
	{
		if (card_id > element.card_id) return true;
		else if (card_id < element.card_id) return false;

		if (bloody_type > element.bloody_type) return true;
		else if (bloody_type < element.bloody_type) return false;

		if (surname < element.surname) return true;
		else if (surname > element.surname) return false;

		if (name < element.name) return true;
		return false;
	};


	bool operator < (Patient& element)
	{
		if (card_id < element.card_id) return true;
		else if (card_id > element.card_id) return false;

		if (bloody_type < element.bloody_type) return true;
		else if (bloody_type > element.bloody_type) return false;

		if (surname > element.surname) return true;
		else if (surname < element.surname) return false;

		if (name > element.name) return true;
		return false;
	};


	bool operator != (Patient& element)
	{
		return surname != element.surname && name != element.name && birthday != element.birthday && phone != element.phone && address != element.address && card_id == element.card_id && bloody_type != element.bloody_type;
	}

	Patient operator = (Patient& element)
	{
		surname = element.surname;
		name = element.name;
		birthday = element.birthday;
		phone = element.phone;
		address = element.address;
		card_id = element.card_id;
		bloody_type = element.bloody_type;
		return *this;
	}

	friend ostream& operator << (ostream& s, Patient el);

};


ostream& operator <<(ostream& s, Patient el)
{
	s << el.name;
	return s;
}

template <class Tkey, class Tval>
Tval find_val(map<Tkey, Tval>& marks, int value)
{
	typename map<Tkey, Tval>::iterator it_m = marks.begin();
	while (it_m != marks.end())
	{
		Tval val = it_m->second;
		if (val == value) return it_m->second;
		it_m++;
	}
}

template <class Tkey, class Tval>
Tval find_key(map<Tkey, Tval>& marks, Tkey key)
{
	typename map<Tkey, Tval>::iterator it = marks.find(key);

	return it->second;
}

template <class Tkey, class Tval>
void print(map<Tkey, Tval>& marks)
{
	typename map<Tkey, Tval>::iterator it_m = marks.begin();
	while (it_m != marks.end())
	{
		cout << "key: " << it_m->first << " value: " << it_m->second << endl;
		it_m++;
	}
}

template <class Tkey, class Tval>
void push(map<Tkey, Tval>& marks, Tkey key, Tval& val)
{
	if(marks.find(key) != marks.end()) throw InvalidKeyException("This key is already in map", key);
	typename map<Tkey, Tval>::iterator it_m = marks.begin();
	/*while (it_m != marks.end())
	{
		if (it_m->first == key) throw InvalidKeyException("This key is already in map", key);
		it_m++;
	}*/
	marks[key] = val;
}


template <class Tkey, class Tval>
map<Tkey, Tval> filter(map<Tkey, Tval>& marks, int threshold)
{
	map<Tkey, Tval> res;
	typename map<Tkey, Tval>::iterator it_m = marks.begin();
	
	while (it_m != marks.end())
	{
		if (it_m->second > threshold)res.insert({ it_m->first, it_m->second });
		it_m++;
	}
	return res;
}

template <class Tkey, class Tval>
set<Tval>uniq_val(map<Tkey, Tval>& marks)
{
	typename map<Tkey, Tval>::iterator it_m = marks.begin();
	set<Tval> res;
	while (it_m != marks.end())
	{
		res.insert(it_m->second);
		it_m++;
	}
	return res;
}


template <class Tkey, class Tval>
Tval find_val(multimap<Tkey, Tval>& marks, int value)
{
	typename multimap<Tkey, Tval>::iterator it_m = marks.begin();
	while (it_m != marks.end())
	{
		Tval val = it_m->second;
		if (val == value) return it_m->second;
		it_m++;
	}
}

template <class Tkey, class Tval>
list<Tval> find_key(multimap<Tkey, Tval>& marks, Tkey key)
{
	typename multimap<Tkey, Tval>::iterator it = marks.find(key);
	list<Tval> res;
	while (it->first == key)
	{
		res.push_back(it->second);
		it++;
	}

	return res;
}

template <class Tkey, class Tval>
void print(multimap<Tkey, Tval>& marks)
{
	typename multimap<Tkey, Tval>::iterator it_m = marks.begin();
	while (it_m != marks.end())
	{
		cout << "key: " << it_m->first << " value: " << it_m->second << endl;
		it_m++;
	}
}

template <class Tkey, class Tval>
multimap<Tkey, Tval> filter(multimap<Tkey, Tval>& marks, int threshold)
{
	multimap<Tkey, Tval> res;
	typename multimap<Tkey, Tval>::iterator it_m = marks.begin();

	while (it_m != marks.end())
	{
		if (it_m->second > threshold)res.insert({ it_m->first, it_m->second });
		it_m++;
	}
	return res;
}

template <class Tkey, class Tval>
set<Tval>uniq_val(multimap<Tkey, Tval>& marks)
{
	typename multimap<Tkey, Tval>::iterator it_m = marks.begin();
	set<Tval> res;
	while (it_m != marks.end())
	{
		res.insert(it_m->second);
		it_m++;
	}
	return res;
}


int main()
{
	try
	{
		Patient p1("Lenin", "Arseniy", "20.05.2004", "89603183896", "Vernadskogo pr-t 86", "122", 4);
		Patient p2("Ivanov", "Ivan", "21.05.2004", "89633183896", "Vernadskogo pr-t 78", "120", 2);
		Patient p3("Sidorov", "Petr", "19.05.2004", "89003183896", "Strominka 20", "121", 3);
		Patient p4("Kovalev", "Artem", "1.06.2004", "1234567891", "Sumskaya 9", "113", 1);


		map<string, int> marks;
		//заполняем marks элементами
		push(marks, p1.card_id, p1.bloody_type);
		push(marks, p2.card_id, p2.bloody_type);
		push(marks, p3.card_id, p3.bloody_type);
		push(marks, p4.card_id, p4.bloody_type);

		//попробуем добавить элемень по сущесвтующему ключу
		//push(marks, p1.card_id, p2.bloody_type);
		
		//функция вывода
		print(marks); cout << '\n';

		//поиск по ключу
		int bloody_type = find_key(marks, string("113"));
		cout << "key: " <<"113"<< "\tVal: "<< bloody_type << endl;

		//поиск по значению
		int p = find_val(marks, 1);
		cout << "Val: "<< p << endl;

		//фильтр возвращает новое дерево
		map<string, int> filtered_marks = filter(marks, 2); cout << "filtered_marks: \t" << endl;
		print(filtered_marks); cout << '\n';

		//множество значений по ключам marks
		set<int> uniq_vals_marks = uniq_val(marks);

		set<int>::iterator it = uniq_vals_marks.begin();
		while (it != uniq_vals_marks.end())
		{
			cout << *it << '\n';
			it++;
		}


		//работа класса mulimap для нескольких элементов с одинаковым ключом
		multimap<int, int> m;
		m.insert({ 1, 10 });
		m.insert({ 1, 20 });
		m.insert({ 2, 30 });
		m.insert({ 3, 40 });
		multimap<int, int>::iterator i = m.begin(); cout << '\n';
		while (i != m.end())
		{
			cout << "Key: " << i->first << " Value: " << i->second << endl;
			i++;
		}
		
		multimap<string, int> multimarks;
		//заполняем multimarks элементами
		multimarks.insert({p1.card_id, p1.bloody_type });
		multimarks.insert({ p2.card_id, p2.bloody_type });
		multimarks.insert({ p3.card_id, p3.bloody_type });
		multimarks.insert({ p4.card_id, p4.bloody_type });
		multimarks.insert({ p3.card_id, p4.bloody_type });
		cout << '\n' << "Multimap:" << endl;
		
		//функция вывода
		print(multimarks); cout << '\n';

		//поиск по ключу в multimap возвращает список всех значений по заданном ключу
		list<int> patient_lst = find_key(multimarks, string("121"));
		list<int>::iterator ptr = patient_lst.begin();
		cout << "Values in multimarks by key: " << "\t" << "121"<<'\n';
		while (ptr != patient_lst.end())
		{
			cout << *ptr << endl;
			ptr++;
		}
		

		//поиск по значению
		cout << endl;
		int patient3 = find_val(multimarks, 1);
		cout << "Val: "<< patient3 << endl;

		//фильтр возвращает новое дерево
		multimap<string, int> filtered_multimarks = filter(multimarks, 3);
		print(filtered_multimarks); cout << '\n';

		//множество значений по ключам marks
		set<int> uniq_vals_multimarks = uniq_val(multimarks);

		set<int>::iterator it2 = uniq_vals_multimarks.begin();
		while (it2 != uniq_vals_multimarks.end())
		{
			cout << *it2 << '\n';
			it2++;
		}

		/*cout << *find_val(marks, 1) << endl;
		cout << *find_key(marks, "113") << endl;*/
		//marks["Petrov"] = 28;
		//marks["Ivanov"] = 4;
		//marks["Sidorov"] = 5;
		//marks["ABNikolaev"] = 3;
		//marks["Anramov"] = 4;
		//cout << "\nMap:\n";
		////итератор пробегает по map
		//map<string, int>::iterator it_m = marks.end();
		//it_m--;
		//while (it_m != marks.begin())
		//{
		//	//перемещение по списку с помощью итератора, нет операции [i]
		//	cout << "Key: " << it_m->first << ", value: " << it_m -> second << "\n";
		//	it_m--;
		//}
	}
	catch (InvalidKeyException e)
	{
		cout << "InvalidKeyException has been caught" << '\n'; e.print();
	}
	catch (exception e)
	{
		cout << "\nexception has been caught: "; e.what();
	};
	return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
