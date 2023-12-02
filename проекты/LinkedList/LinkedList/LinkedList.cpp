

#include <iostream>
using namespace std;

template<class T>
class Element
{
protected:
	Element* next;
	Element* prev;
	T info;
public:

	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}

	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}

	Element(const Element& el)
	{
		next = el.next;
		prev = el.prev;
		info = el.info;
	}

	T getInfo() { return info; }
	void setInfo(T value) { info = value; }
	Element<T>* getNext() { return next; }
	void setNext(Element<T>* value) { next = value; }
	Element<T>* getPrev() { return prev; }
	void setPrev(Element<T>* value) { prev = value; }

	template<class T1>
	friend ostream& operator<<(ostream& s, Element<T1>& el);

};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
	s << el.info;
	return s;
}

class my_class
{
public:
	string name; //название страны
	string capital; //столица
	string language; //преобладающий язык
	int population; //население тыс.
	double area; //площадь тыс. кв. км.

	my_class()
	{
		name = "Russia";
		capital = "Moscow";
		language = "russian";
		population = 143400;
		area = 171000;
	}
};

template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	

	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}

	LinkedList(T* arr, int len)
	{
		if (len == 0)
			head = tail = NULL;
		else
		{
			head = arr[0];
			tail = arr[-1];
		}
		count = len;

	}

	//удалить первый/последний элемент и вернуть удаленное значение
	virtual Element<T>* pop() = 0;
	//добавить элемент в список
	virtual Element<T>* push(T value) = 0;

	//доступ по индексу
	Element<T>& operator[](int index)
	{
		if (index < 0 || index >= count) return;

		Element<T>* current = head;

		for (int i = 0;
			current != NULL && i < index;
			current = current->getNext(), i++);

		return *current;
	}
	template<class T1>
	friend ostream& operator<<(ostream& s, LinkedList<T1>& el);

	//доработать деструктор
	virtual ~LinkedList()
	{
		Element<T>* previous;
		for (Element<T>* current = head; current != NULL;)
		{
			previous = current;
			current = current->getNext();
			delete previous;
		}
	}
};

template<class T>
class Queue : public LinkedList<T>
{
public:
	using LinkedList<T>::tail;
	using LinkedList<T>::head;
	using LinkedList<T>::count;


	Queue<T>() : LinkedList<T>() {}
	virtual ~Queue() { cout << "\nQueue class destructor"; }

	virtual Element<T>* pop()
	{
		Element<T>* res = head;

		if (head == NULL) return NULL;

		if (head == tail)
		{
			count = 0;
			head = tail = NULL;
			return res;
		}

		head = head->getNext();
		return res;
	}
	virtual Element<T>* push(T value)
	{
		Element<T>* newElem = new Element<T>(value);
		if (tail != NULL)
		{
			tail->setNext(newElem);
			tail = tail->getNext();
		}
		else
			head = tail = newElem;

		count++;
		return tail;
	}

	virtual Element<T>* insert(T value, Element<T>* previous = tail)
	{
		if (previous == tail) return push(value);

		Element<T>* inserted = new Element<T>(value);
		Element<T>* next = previous->getNext();
		previous->setNext(inserted);
		inserted->setNext(next);
		count++;
		return inserted;
	}
};

int main()
{
	Queue<int>M;
	for (int i = 0; i < 10; i++)
	{
		M.push(i + 1);
		cout << M;
	}

	char c; cin >> c;
	return 0;
}
