//Практическая работа 6, вариант 16

#include <iostream>
#include<queue>
#include<stack>
#include<vector>
#include<set>

using namespace std;

#define INFINITY 10000 // условное число, обозначающее бесконечность


void FillVectorWith(vector<int>& v, int value, int size = 0)
{
	if (size != 0)
	{
		v.resize(0);
		for (int i = 0; i < size; i++)
			v.push_back(value);
	}
	for (int i = 0; i < v.size(); i++)
		v[i] = value;
}

void FillMatrixWith(vector<vector<int> >& matrix, int value, int width = 0, int height = 0)
{
	if (width != 0 && height != 0)
	{
		matrix.resize(0);
		int i, j;
		for (i = 0; i < height; i++)
		{
			vector<int> v1;
			for (j = 0; j < width; j++)
			{
				v1.push_back(value);
			}
			matrix.push_back(v1);
		}
	}
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
			matrix[i][j] = value;
	}
}

int FindPath(vector<vector<int> >& f, vector<vector<int> >& c, int source, int target, int vertices) // source - исток, target - сток
{
	int i;
	int CurVertex;
	// Flow - значение потока через данную вершину на данном шаге поиска
	// Link[i] хранит номер предыдущей вершины на пути i -> исток
	vector<int> Flow; FillVectorWith(Flow, 0, vertices);
	vector<int> Link; FillVectorWith(Link, -1, vertices);
	Flow[source] = INFINITY;
	vector<int> used(vertices, 0);
	//queue<int> q; q.push(source);
	priority_queue<pair<int, int>, vector<pair<int, int>>> Q; Q.push(make_pair(0, source));
	//while (!q.empty())
	while (!Q.empty())
	{
		//int vertex = q.front();
		int vertex = Q.top().second;
		if (vertex == target) break;
		Q.pop();
		//if (used[vertex])continue;
		
		//q.pop();

		for (int i = 0; i < vertices; i++)
		{
			//есть ребро vertex->i, в i не заходили
			if (c[vertex][i] - f[vertex][i] > 0 && used[i] == 0)
			{
				Q.push(make_pair(c[vertex][i], i));//q.push(i);
				Flow[i] = (c[vertex][i] - f[vertex][i] < Flow[vertex]) ? c[vertex][i] - f[vertex][i] : Flow[vertex];
				Link[i] = vertex;
			}
		}

		//Link[Q.top().second] = vertex;
		used[vertex] = 1;
	}

	if (Link[target] == -1) return 0;
	CurVertex = target;
	while (CurVertex != source)
	{
		f[Link[CurVertex]][CurVertex] += Flow[target];
		CurVertex = Link[CurVertex];
	}
	//cout << "\n" << Flow[target] << ": ";
	//for (int i = 0; i < vertices; i++) cout << Link[i] << " ";
	return Flow[target];
}


class Edge
{
public:
	int from, to, weight;
	Edge(int from = -1, int to = -1, int weight = 0) : from(from), to(to), weight(weight) {}
	Edge(const Edge& E)
	{
		from = E.from;
		to = E.to;
		weight = E.weight;
	}
	int operator<(Edge& E)
	{
		return (weight < E.weight);
	}
	friend ostream& operator<<(ostream& s, Edge& e);
};

ostream& operator<<(ostream& s, Edge& e)
{
	s << "From: " << e.from << ", to: " << e.to << ", weight: " << e.weight;
	return s;
}


int max_flow(vector<vector<int> >& f, vector<vector<int> >& c, int source, int target, int vertices) // source - исток, target - сток)
{
	FillMatrixWith(f, 0, c.size(), c.size());
	int res = 0;
	int new_flow;
	do
	{
		new_flow = FindPath(f, c, source, target, vertices);
		res += new_flow;
	} while (new_flow);
	return res;
}


bool Hamilton(vector<vector<int>>&matrix, vector <bool>& visited, vector<int>& path, int current)
{
	path.push_back(current);

	cout << '\n';
	for (int i = 0; i < path.size(); i++)
		cout << path[i] << " ";
	
	visited[current] = true;
	if (path.size() ==matrix.size() &&matrix[path[0]][path[path.size() - 1]] > 0)
		return true;

	if (path.size() ==matrix.size() &&matrix[path[0]][path[path.size() - 1]] == 0)
	{
		visited[current] = false;
		path.pop_back();
		return false;
	}

	for (int i = 0; i <matrix.size(); i++)
	{
		if (matrix[current][i] > 0 && !visited[i])
		{
			if (Hamilton(matrix, visited, path, i))
				return true;
		}
	}

	visited[current] = false;
	path.pop_back();
	return false;
}


void painting_graph(vector<vector<int>>& matrix, vector<int>& color, int curr = 0)
{
	set<int> neighbours;

	queue<int> q;
	q.push(curr);
	while (!q.empty())
	{
		int vertex = q.front();
		q.pop();

		if (color[vertex] != 0)
			continue;
		neighbours.clear();

		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[vertex][j] )
			{
				if (color[j] == 0)
				{
					q.push(j);
				}
				else
				{
					neighbours.insert(color[j]);
				}
			}
		}

		int color_counter = 1;
		auto it = neighbours.begin();
		do
		{
			set<int>::iterator it = neighbours.find(color_counter);
			if (it == neighbours.end()) break;
			color_counter++;
		} while (true);

		color[vertex] = color_counter;
	}
}


int main()
{
	vector<vector<int> > c =
	{
	{ 0, 5, 3, 1, 0, 3, 4, 3, 6, 0, 4 },
	{ 5, 0, 6, 7, 9, 4, 3, 3, 6, 9, 6 },
	{ 3, 6, 0, 4, 5, 1, 9, 5, 3, 1, 8 },
	{ 1, 7, 4, 0, 5, 5, 2, 4, 2, 5, 8 },
	{ 0, 9, 5, 5, 0, 3, 8, 2, 6, 4, 3 },
	{ 3, 4, 1, 5, 3, 0, 3, 2, 2, 2, 8 },
	{ 4, 3, 9, 2, 8, 3, 0, 7, 6, 7, 6 },
	{ 3, 3, 5, 4, 2, 2, 7, 0, 4, 3, 4 },
	{ 6, 6, 3, 2, 6, 2, 6, 4, 0, 7, 3 },
	{ 0, 9, 1, 5, 4, 2, 7, 3, 7, 0, 9 },
	{ 4, 6, 8, 8, 3, 8, 6, 4, 3, 9, 0 }
	};

	vector<vector<int>> test =
	{
	{ 0, 6, 1, 2, 0, 0},
	{ 6, 0, 5, 0, 3, 0},
	{ 1, 5, 0, 5, 6, 4},
	{ 2, 0, 5, 0, 0, 2},
	{ 0, 3, 6, 0, 0, 7},
	{ 0, 0, 4, 2, 7, 0},
	};

	vector<vector<int>> f;

	int mx_flow = max_flow(f, c, 0, c.size() - 1, c.size());
	cout << "\nMax_flow: "<< mx_flow << endl;

	vector<bool> visited(c.size(), 0);
	vector<int> path;

	if (Hamilton(c, visited, path, 0))
		cout << "There's hamilton's cycle\n";
	else
		cout << "There's NO hamilton's cycle\n";

	vector<int> colors(test.size());

	painting_graph(test, colors);
	cout << endl;
	for (int i = 0; i < colors.size(); i++)
		cout << colors[i] << ' ';

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
