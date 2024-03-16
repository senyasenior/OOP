//Практическая работа 1.3-1.5, вариант 16

#include <iostream>

#include <iostream>
#include <fstream>

using namespace std;



class Patient
{
public:
	string surname, birthday, phone, address;
	string name;
	int card_id;
	int bloody_type;

	Patient()
	{
		surname = " ";
		name = " ";
		birthday = " ";
		phone = " ";
		address = " ";
		card_id = NULL;
		bloody_type = NULL;
	}

	Patient(string sr, string nm, string bd, string ph, string ad, int card, int blood)
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


class InvalidPopException : Exception
{
public:
	InvalidPopException(const char* str) : Exception(str) {};
	virtual void print()
	{
		cout << "InvalidPopException: " << str <<"; "<< what();
	}
};


class ReadIteratorException : Exception
{
public:
	ReadIteratorException(const char* str) : Exception(str) {};
	virtual void print()
	{
		cout << "ReadIteratorException: " << str << "; " << what();
	}
};


template <class T>
class Element
{
	//элемент связного списка
private:
	//указатель на предыдущий и следующий элемент
	Element* next;
	Element* prev;

	//информация, хранимая в поле
	T field;
public:
	//доступ к полю *next
	virtual Element* getNext() { return next; }
	virtual void setNext(Element* value) { next = value; }

	//доступ к полю *prev
	virtual Element* getPrevious() { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }

	//доступ к полю с хранимой информацией field
	virtual T getValue() { return field; }
	virtual void setValue(T value) { field = value; }

	template<class T> friend ostream& operator<< (ostream& ustream, Element<T>& obj);

	Element(T value) { field = value; next = prev = NULL; }

	bool operator < (Element<T>& el) { return field < el.getValue(); }

	bool operator > (Element<T>& el) { return field < el.getValue(); }
};

template<class T>
ostream& operator << (ostream& ustream, Element<T>& obj)
{
	ustream << obj.field;
	return ustream;
}

template <class T>
class LinkedListParent
{
protected:
	//достаточно хранить начало и конец
	Element<T>* head;
	Element<T>* tail;
	//для удобства храним количество элементов
	int num;
public:
	virtual int Number() { return num; }

	virtual Element<T>* getBegin() { return head; }

	virtual Element<T>* getEnd() { return tail; }

	LinkedListParent()
	{
		//конструктор без параметров
		cout << "\nParent constructor";
		head = NULL; tail = NULL;
		num = 0;
	}

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
	virtual Element<T>* push(T value) = 0;

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
	virtual T pop() = 0;

	virtual ~LinkedListParent()
	{
		//деструктор - освобождение памяти
		cout << "\nParent destructor";
		Element<T>* previous;
		for (Element<T>* current = head; current != NULL;)
		{
			previous = current;
			current = current->getNext();
			delete previous;
		}
	}

	//получение элемента по индексу - какова асимптотическая оценка этого действия?
	virtual Element<T>* operator[](int i)
	{
		//индексация
		//if (i<0 || i>num) return NULL;
		int k = 0;

		//ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
		Element<T>* cur = head;
		for (k = 0; k < i && cur != NULL; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}

	

	template<class T1> friend ostream& operator<< (ostream& ustream, LinkedListParent<T1>& obj);
	template<class T1> friend istream& operator>> (istream& ustream, LinkedListParent<T1>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, LinkedListParent<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
			ustream << *current << " ";
		return ustream;
	}

	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
		ustream << "arr[" << i << "] = " << *current << "\n";

	return ustream;
}

template<class T>
istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
{
	//чтение из файла и консоли совпадают
	int len;
	ustream >> len;
	//здесь надо очистить память под obj, установить obj.num = 0
	double v = 0;
	for (int i = 0; i < len; i++)
	{
		ustream >> v;
		obj.push(v);
	}
	return ustream;
}

//дописать класс итератора по списку
template<typename ValueType>
class ListIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
public:
	//конструкторы
	ListIterator() { ptr = NULL; }
	ListIterator(Element<ValueType>* p) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	//методы работы с итераторами
	//присваивание
	ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
	ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }

	//проверка итераторов на равенство
	bool operator!=(ListIterator const& other) const { return other.ptr != ptr; }
	bool operator==(ListIterator const& other) const { return other.ptr == ptr; }
	//получить значение
	Element<ValueType>& operator*()
	{
		if (ptr == NULL)
		{
			throw ReadIteratorException("Iterator isn't linked with any element");
		}
		return *ptr;
	}
	//перемещение с помощью итераторов
	ListIterator& operator++() //префиксная ++it
	{
		if (ptr->getNext() == NULL)
			throw ReadIteratorException("Iterator isn't linked with any element");
		ptr = ptr->getNext();
		return *this;
	}
	ListIterator& operator++(int v)//постфиксная it++
	{
		if (ptr->getNext() == NULL)
			throw ReadIteratorException("Iterator isn't linked with any element");
		ptr = ptr->getNext();
		return *this;
	}
	ListIterator& operator--()//префиксная --it
	{
		if (ptr->getPrevious() == NULL)
			throw ReadIteratorException("Iterator isn't linked with any element");
		ptr = ptr->getPrevious();
		return *this;
	}
	ListIterator& operator--(int v) //постфиксная it--
	{
		if (ptr->getPrevious() == NULL)
			throw ReadIteratorException("Iterator isn't linked with any element");
		ptr = ptr->getPrevious();
		return *this;
	}

	//текущий элемент
	Element<ValueType>* ptr;
};

//класс итерируемый список - наследник связного списка, родитель для Очереди и Стека
template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() { cout << "\nIteratedLinkedList constructor"; }
	virtual ~IteratedLinkedList() { cout << "\nIteratedLinkedList destructor"; }

	//ListIterator<T> iterator;

	ListIterator<T> begin() { ListIterator<T> it = LinkedListParent<T>::head; return it; }
	ListIterator<T> end() { ListIterator<T> it = LinkedListParent<T>::tail; return it; }

	IteratedLinkedList<T>* univ_filter(LinkedListParent<T>& lst, bool (*f)(T))
	{
		ListIterator<T> it;
		it.ptr = lst.getBegin();

		while (it.ptr != NULL)
		{
			if (f(it.ptr->getValue())) {this->push(it.ptr->getValue()); }

			if (it.ptr->getNext() == NULL)
				break;
			it++;
		}
		return this;
	}
};



template <class T>
class Stack : public IteratedLinkedList<T>
{
	
public:
	using LinkedListParent<T>::tail;
	using LinkedListParent<T>::head;
	using LinkedListParent<T>::num;
	using IteratedLinkedList<T>::begin;
	using IteratedLinkedList<T>::end;
	Stack() : IteratedLinkedList<T>() {  }

	virtual Element<T>* push(T value)
	{
		/*Element<T>* elem = new Element<T>(value);
		if (this->head != NULL)
			this->tail->setNext(elem);
		else
			this->head = elem;
		elem->setPrevious(this->tail);
		this->tail = elem;
		num++;
		return this->tail;*/

		//с использованием итераторов
		Element<T>* elem = new Element<T>(value);
		ListIterator<T> it = end();
		num++;
		if (head == NULL)
		{
			head = tail = elem;
			return elem;
		}
		tail = elem;
		(*it).setNext(elem);
		elem->setPrevious(it.ptr);
		return elem;
	}

	T pop()
	{
		if (tail == NULL) {
			throw InvalidPopException("it's impossible to pop an element from empty stack");
		}
		//c помощью итераторов
		num--;
		ListIterator<T> it = end();
		if (head == tail)
		{
			head = tail = NULL;
		}
		tail = (*it).getPrevious();
		(*it).setPrevious(NULL);
		if (tail)tail->setNext(NULL);
		return (*it).getValue();


		/*if (tail == head) {
			T save = tail->getValue();
			delete tail;
			head = tail = NULL;
			num--;
			return save;
		}

		Element<T>* elem = tail->getPrevious();
		T save = tail->getValue();
		elem->setNext(NULL);
		//tail->setPrevious(NULL);
		delete tail;
		tail = elem;
		num--;
		return save;*/
	}

	ListIterator<T> insert(ListIterator<T> elem, T value)
	{
		//с помощью итераторов
		Element<T>* newElem = new Element<T>(value);
		if (elem == NULL)
		{
			//вставка в начало
			ListIterator<T> it = begin();
			if (it == NULL)
			{
				it.ptr = push(value);
				return it;
			}
			(*it).setPrevious(newElem);
			newElem->setNext(it.ptr);
			head = newElem;
			num++;
			return begin();
		}
		if (head == NULL) { push(value); return begin(); }
		if (elem.ptr == head) { return insert(NULL, value); }
		Element<T>* current = elem.ptr;
		Element<T>* previous = (*elem).getPrevious();
		previous->setNext(newElem);
		newElem->setPrevious(previous);
		newElem->setNext(current);
		current->setPrevious(newElem);
		elem--;
		num++;
		return elem;

		/*Element<T>* newElem = new Element<T>(value);
		if (elem == NULL)
		{
			//вставка в начало
			Element<T>* current = head;
			if (current == NULL)return push(value);
			current->setPrevious(newElem);
			newElem->setNext(current);
			head = newElem;
			num++;
			return begin();
		}

		if (head == NULL) { push(value); return begin(); }
		if (head == tail) { return insert(NULL, value); }
		
		Element<T>* current = elem.ptr;
		Element<T>* previous = current->getPrevious();
		if (previous == NULL)return insert(NULL, value);
		previous->setNext(newElem);
		newElem->setPrevious(previous);
		newElem->setNext(current);
		current->setPrevious(newElem);
		elem--;
		num++;
		return elem;*/
	}

	//функция вывода с помощью итераторов
	void print()
	{
		ListIterator<T> it = begin();
		cout << "\nLength: " << num << endl;
		for (it; it.ptr->getNext() != NULL; it++)
		{
			cout << *it << endl;
		}
	}

	/*friend ostream& operator<<(ostream& s, Stack<T>& lst);*/
};


////функция вывода списка с помощью итераторов
//template <class T>
//ostream& operator<< (ostream& s, Stack<T>& lst)
//{
//	ListIterator<T> it = lst.begin();
//	s << "Length: " << lst.num << '\n';
//	while (it.ptr != NULL)
//	{
//		s << *it << '\n';
//		it++;
//	}
//	return s;
//}


template<class T>
class OrderedStack : public Stack<T>
{

public:
	using LinkedListParent<T>::tail;
	using LinkedListParent<T>::head;
	using LinkedListParent<T>::num;
	using IteratedLinkedList<T>::begin;
	using IteratedLinkedList<T>::end;
	using Stack<T>::insert;
	//список отсортирован по возрастанию
	OrderedStack() : Stack<T>() { cout << "OrderedStack constructor"; }
	virtual ListIterator<T> push(T& value)
	{
		//с помощью итераторов
		ListIterator<T> it = begin();
		if(it == NULL)return Stack<T>::push(value);
		while (it != NULL)
		{
			if ((*it).getValue() > value) return insert(it, value).ptr;
			it++;
		}
		//if ((*it).getValue() > value) return insert(it, value).ptr;
		return Stack<T>::push(value);
	}
	
};


template <class T>
LinkedListParent<T>* filter(Stack<T>& lst, bool (*f)(T))
{
	Stack<T>* res = new Stack<T>;
	ListIterator<T> it = lst.begin();
	while (it != NULL)
	{
		if ( f((*it).getValue()) ) res->push((*it).getValue());

		if ((*it).getNext() == NULL)
			break;
		it++;
	}
	return res;
}

//template <class T, class U>
//U univ_filter(T& lst, bool (*f)(char))
//{
//	LinkedListParent<char>* ptr = new T;
//	ListIterator<char> it = lst.begin();
//
//	U<char> res;
//	while (it.ptr != NULL)
//	{
//		if (f((*it).getValue())) res.push((*it).getValue());
//		it++
//	}
//	return res;
//}


//только согласные
bool consonant(char c)
{
	int con = 'a' - 'A';
	if (c >= 'A' && c <= 'Z') c += con;
	return c != 'a' && c != 'o' && c != 'u' && c != 'e' && c != 'i';
}


int main()
{
	try
	{


		Stack<int> S;
		////pop() из пустого списка
		//S.pop();

		////чтение значения, когда итеротор не связан с элементом
		//ListIterator<int> ptr = S.begin();
		//*ptr;

		S.push(1);
		S.push(2);
		S.push(3);
		cout << S;
		cout << "\n";
		int n = S.pop();
		cout << "\nElement = " << n;
		cout << S;
		cout << "\nIndex 1 in the Stack class: " << S[1]->getValue();

		//используем print с помощью итераторов
		S.print(); cout << '\n';
		cout << "\nIterators:\n";
		ListIterator<int> it = S.begin();
		while (it != S.end())
		{
			cout << *it << " ";
			it++;
		}
		cout << *it<< " " << endl;

		//список char
		Stack<char> S_char;
		for (int i = 0; i < 10; i++)
		{
			char c = i + 'A';
			S_char.push(c);
		}
		//вставим 'j' как предпоследний
		ListIterator<char> i = S_char.end();
		S_char.insert(i, 'j');
		S_char.print(); cout << '\n';

		//протестируем filter на S_char
		LinkedListParent<char>* filtered_S_char = filter(S_char, consonant);
		cout << *filtered_S_char << endl;

		//протестируем универсальный фильтр univ_filter
		OrderedStack<char> filtered_S_char_ordered;
		filtered_S_char_ordered.univ_filter(S_char, consonant);
		filtered_S_char_ordered.print();


		//создадим отсортированный стек из элементов класса Patient
		Patient p1("Lenin", "Arseniy", "20.05.2004", "89603183896", "Vernadskogo pr-t 86", 122, 4);
		Patient p2("Ivanov", "Ivan", "21.05.2004", "89633183896", "Vernadskogo pr-t 78", 123, 2);
		Patient p3("Sidorov", "Petr", "19.05.2004", "89003183896", "Strominka 20", 121, 3);
		Patient p4("Kovalev", "Artem", "1.06.2004", "1234567891", "Sumskaya 9", 130, 1);

		/*Stack<Patient> S_patient;
		S_patient.push(p2);
		S_patient.push(p1);
		S_patient.push(p3);
		S_patient.push(p4); cout << '\n';
		S_patient.print();*/

		
		//отсортированный стек: Номер карты, группа крови, фамилия и имя (по возрастанию)
		OrderedStack<Patient> S_patient_ordered;
		S_patient_ordered.push(p4);
		S_patient_ordered.push(p2);
		S_patient_ordered.push(p3);
		S_patient_ordered.push(p1); cout << '\n'; cout << "\nOrdered Stack:";
		S_patient_ordered.print();

		
		/*char c; cin >> c;*/
	}
	catch(InvalidPopException e)
	{
		cout << "\nInvalidPopException has been caught: "; e.print();
	}
	catch (ReadIteratorException e)
	{
		cout << "\nReadIteratorException has been caught: "; e.print();
	}
	catch (exception e)
	{
		cout << "\nexception has been caught: "; e.what();
	}

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