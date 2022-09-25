#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stack>

using namespace std;

#define MAX 141

class Station
{
public:
	int id; // ����ȣ
	int time; // �̵��ð�
	//int transfer; // ȯ�� Ƚ��
	Station* next;
	Station() : next(NULL) {}
};

// �׷��� (��)
class MatrixWGraph
{
private:
	string* station; // �� �̸�
	Station* graph; // �׷���
	int* route; // ������ ������, ����� ���Ե� ���� �ǹ�
public:
	// ���ͽ�Ʈ�� �˰���
	void Dijkstra(int start)
	{
		int minpos;
		// graph, route �迭 �ʱ�ȭ
		for (int i = 0; i < MAX; i++)
		{
			route[i] = -1;
			graph[i].id = -1;
			graph[i].time = 10000;
			//graph[i].transfer = 1000;
		}

		// �������� ����� ���� �����̼��� ���� �����Ѵ�.
		for (Station* s = graph[start].next; s != NULL; s = s->next)
		{
			// ��߿��� ȯ�¿��� ��� ȯ�� Ƚ���� �ð��� 0���� ����
			if (station[start] == station[s->id])
			{
				graph[s->id].time = 0;
				//graph[s->id].transfer = 0;
			}
			else
			{
				graph[s->id].time = s->time;
				//graph[s->id].transfer = s->transfer;
			}
		}

		// ��߿��� ȯ�¿��� ��� ��߿��� ���� �̸��� ���� ���� ���� 0���� �ʱ�ȭ
		for (int i = 0; i < MAX; i++)
		{
			if (station[start] == station[i])
			{
				for (Station* s = graph[i].next; s != NULL; s = s->next)
				{
					if (station[i] == station[s->id])
					{
						s->time = 0;
						//s->transfer = 0;
					}
				}
			}
		}
		// �������� �ʱ�ȭ
		route[start] = 0;
		graph[start].time = 0;
		//graph[start].transfer = 0;


		for (int i = 0, prev = start; i < MAX - 6; i++)
		{
			// �ּҰ��� �������� �ε��� ���� ���Ѵ�.
			int min = 10000, trans_min = 1000, pos = -1;

			for (int i = 0; i < MAX; i++)
			{
				if (route[i] < 0)
				{
					if (graph[i].time < min)
					{
						min = graph[i].time;
						//trans_min = graph[i].transfer;
						pos = i;
					}
				}
			}

			minpos = pos;

			//�ּ� ���� ���� �����͸� �����Ѵ�.
			route[minpos] = 0;

			// ���õ� ���� ����� ����� �ٸ� �� ���� ������Ʈ �ϸ� �ִܰ�θ� ���Ѵ�.
			for (Station* s = graph[minpos].next; s != NULL; s = s->next)
			{
				// �ð� ��
				if (graph[minpos].time + s->time < graph[s->id].time)
				{
					graph[s->id].time = graph[minpos].time + s->time;
					//graph[s->id].transfer = graph[minpos].transfer + s->transfer;
				}
				/*
				// �ð��� ���� �� ȯ�� Ƚ���� ���Ͽ� ȯ�� Ƚ���� ���� �� ���
				else if (graph[minpos].time + s->time == graph[s->id].time)
				{
					if (graph[minpos].transfer + s->transfer < graph[s->id].transfer)
					{
						graph[s->id].time = graph[minpos].time + s->time;
						graph[s->id].transfer = graph[minpos].transfer + s->transfer;
					}
				}
				*/

			}
		}
	}

	void LoadMap(const char* name, Station* temp, Station* st)
	{
		fstream file;

		// Ŭ���� ������ �ʱ�ȭ
		graph = new Station[MAX];
		station = new string[MAX];
		route = new int[MAX];

		// ���� �б�
		file.open(name, ios::in);

		int t1, t2, time;
		string s1, s2;

		while (!file.eof())
		{
			// ������ ������ �Է�
			file >> t1;
			file >> t2;
			file >> time;
			file >> s1;

			// ���� ������ ������ ���� ��� break
			if (file.eof())
				break;

			file >> s2;

			// �迭 index�� ���� �־�� �ϱ� ������ -1 �� ���ش�. (�迭�� 0���� �����̹Ƿ�)
			t1 -= 1;
			t2 -= 1;

			// ���ο� ��θ� �����.
			temp = new Station();
			temp->id = t2;
			temp->time = time;
			//if (s1 == s2) temp->transfer = 1;
			//else temp->transfer = 0;

			// ������ ���� ��θ� �����Ѵ�.
			st = &graph[t1];
			while (st->next != NULL)
			{
				st = st->next;
			}
			st->next = temp;

			temp = new Station();
			temp->id = t1;
			temp->time = time;
			//if (s1 == s2) temp->transfer = 1;
			//else temp->transfer = 0;

			st = &graph[t2];
			while (st->next != NULL)
			{
				st = st->next;
			}
			st->next = temp;

			// �� �̸��� �迭�� ����
			station[t1] = s1;
			station[t2] = s2;
		}

		// ������ �ݾ��ش�.
		file.close();
	}

	void PrintMap()
	{
		system("cls");
		cout << "\t";
		for (int i = 0; i < MAX; i++)
			cout << station[i] << " ";
		cout << endl << "\t";

		system("pause");
	}

	// ��� ���
	void PrintLoad()
	{
		system("cls");
		int start = -1, end = -1;
		int min, v, prev;
		string t;
		stack<int> s, r;

		// start ������ end ������ ���Ͽ� ���, ���� �� �Է�
		while (start == end)
		{

			while (start < 0)
			{
				cout << "��߿� : ";
				cin >> t;

				for (int i = 0; i < MAX; i++)
				{
					if (station[i] == t)
					{
						start = i;
						break;
					}
				}

				if (start < 0)
					cout << "���� �� �Դϴ�." << endl;
			}

			while (end < 0)
			{
				cout << "������ : ";
				cin >> t;

				for (int i = 0; i < MAX; i++)
				{
					if (station[i] == t)
					{
						end = i;
						break;
					}
				}

				if (end < 0)
					cout << "���� �� �Դϴ�." << endl;
			}

			if (start == end)
			{
				cout << "��߿��� �������� �����ϴ�." << endl;
				start = end = -1;
			}
		}


		cout << "���� ���� : " << station[start] << " �� " << station[end] << endl << endl;
		cout << "�� �ִܰ�� :" << endl;

		// ���ͽ�Ʈ�� �˰��� ����
		this->Dijkstra(start);

		// ������ �� �ҿ�ð��� ���� ���� ���� �����Ѵ�.
		min = graph[end].time;
		for (int i = 0; i < MAX; i++)
		{
			if (station[end] == station[i])
			{
				if (graph[i].time < min)
				{
					min = graph[i].time;
					end = i;
				}
			}
		}

		// ��߿��� �ε����� ã�� ���� �ε����� ���ÿ� �����Ѵ�.
		this->FindRoute(s, start, end);

		// ���ÿ��� �������� �̵� (���� ����)
		while (!s.empty())
		{
			r.push(s.top());
			s.pop();
		}

		// ��� ���
		for (v = 0, prev = -1; !r.empty(); v++)
		{
			if (prev != -1 && station[prev] == station[r.top()])
			{
				if (station[r.top()] != station[start])
					cout << "[ȯ��]";

				v--;
				prev = r.top();
				r.pop();
			}
			else
			{
				if (v != 0)
					cout << " �� ";
				else
					cout << " ";

				cout << station[r.top()];
				prev = r.top();
				r.pop();

			}
		}
		cout << endl << "�� �̵� �� : " << v - 1 << endl
			<< "�� �ҿ�ð� : " << graph[end].time / 60 << "�ð� " << graph[end].time % 60 << "��\t" << endl;
		system("pause");
		//<< "�� ȯ�� Ƚ�� : " << graph[end].transfer << endl;

	}

	// �ϼ��� ���ÿ��� ��� ã��
	bool FindRoute(stack<int>& s, int start, int end)
	{
		s.push(start);

		// ���� == �� �϶� true return
		if (start == end)
			return true;
		// �������� ����� �� ��� ����
		for (Station* st = graph[start].next; st != NULL; st = st->next)
		{
			// �������� + ������ ���� ����� �� ���� ������ (���ͽ�Ʈ�� �˰��򿡼� ���ÿ� ���Խ�ų �� ������ ���)
			if (graph[start].time + st->time == graph[st->id].time)// && graph[start].transfer + st->transfer == graph[st->id].transfer)
			{
				// ���� ��ΰ� ���ٸ� pop
				if (!this->FindRoute(s, st->id, end))
				{
					s.pop();
				}
				else
				{
					// ��θ� ã�Ҵٸ� ��θ� ã�Ҵٰ� true return
					return true;
				}
			}
		}
		return false;
	}
};

int main()
{
	// MatrixWGraph Ŭ���� ���� ����
	MatrixWGraph subway;

	// Station Ŭ���� ���� ����
	Station* temp = new Station();
	Station* station = new Station();

	int select = 0;

	//MatrixWGraph Ŭ������ LoadMap �Լ��� ȣ���ϸ鼭 Station Ŭ���� ���� �ΰ��� �Ѱ��ش�.
	subway.LoadMap("subway.txt", temp, station);

	while (true)
	{
		system("cls");

		cout << "*******************************" << endl;
		cout << "*       ��ã�� ���α׷�       *" << endl;
		cout << "*******************************" << "\n\n\n" << endl;
		cout << "------------------------------" << endl;
		cout << "*  1. ����ö �뼱 ���� Ȯ��  *" << endl << "*  2. ��ã�� ���α׷� ����   *" << endl << "*  3. ���α׷� ����          *" << endl << "------------------------------" << endl<< "���� �Է� : ";
		cin >> select;

		if (select == 1)
			subway.PrintMap();
		else if (select == 2)
			// MatrixWGraph Ŭ������ PrintLoad �Լ��� ȣ��
			subway.PrintLoad();
		else if (select == 3)
		{
			cout << "���α׷� ����!" << endl;
			break;
		}
		else
		{
			cout << "1~3�� �Է�!" << endl;
			system("pause");
		}
	}




	// Ŭ���� ���� �޸� ����
	delete(temp);
	delete(station);

	return 0;
}