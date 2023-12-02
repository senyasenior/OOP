// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include<queue>
#include<stack>

using namespace std;

int main()
{

	vector<vector<int> > mat =
	{
	{ 0, 15, 10, 0, 0, 0, 1, 0, 0, 0 }, // матрица смежности
	{ 15, 0, 15, 1, 0, 0, 0, 0, 0, 0  },
	{ 10, 15, 0, 0, 0, 0, 0, 0, 0, 0  },
	{ 0, 1, 0, 0, 1, 0, 0, 0, 0, 0  },
	{ 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 },
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	};

	vector <int> used(10, 0);
	vector<int> dist(10, -1);

	//BFS
	queue<int> q;
	q.push(0);
	dist[0] = 0;
	while (!q.empty())
	{
		int cur_vertex = q.front(); q.pop();
		for (int i = 0; i < 10; i++)
			if (mat[cur_vertex][i] != 0 && used[i] == 0)
			{
				q.push(i); dist[i] = dist[cur_vertex] + 1; used[i] = 1;
			}
		used[cur_vertex] = 2;
	}

	std::cout << "\nBFS Visited vertices ";
	for (int i = 0; i < 10; i++) std::cout << used[i] << " ";
	std::cout << "\nBFS Distances ";
	for (int i = 0; i < 10; i++) std::cout << dist[i] << " ";

	for (int i = 0; i < 10; i++) dist[i] = used[i] = 0;
	stack<int> s;
	s.push(0);
	dist[0] = 0;
	int step = 1;
	while (!s.empty())
	{
		int cur_vertex = s.top(); s.pop();
		for (int i = 0; i < 10; i++)
			if (mat[cur_vertex][i] != 0 && used[i] == 0)
			{
				s.push(i); used[i] = 1;
			}
		used[cur_vertex] = 2;
		dist[cur_vertex] = step++;
	}


	std::cout << "\nDFS Visited vertices ";
	for (int i = 0; i < 10; i++) std::cout << used[i] << " ";
	std::cout << "\nDFS order ";
	for (int i = 0; i < 10; i++) std::cout << dist[i] << " ";

	for (int i = 0; i < 10; i++) dist[i] = 10000;
	q.push(0);
	dist[0] = 0;
	while (!q.empty())
	{
		int cur_vertex = q.front(); q.pop();
		for (int i = 0; i < 10; i++)
			if (mat[cur_vertex][i] != 0 && dist[i] > dist[cur_vertex] + mat[cur_vertex][i])
			{
				q.push(i); dist[i] = dist[cur_vertex] + mat[cur_vertex][i];
			}
		std::cout << "\nHandling vertex " << cur_vertex << " ";
		for (int i = 0; i < 10; i++) std::cout << dist[i] << " ";
	}
	std::cout << "\nDijsktra distances ";
	for (int i = 0; i < 10; i++) std::cout << dist[i] << " ";


	char c; cin >> c;
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