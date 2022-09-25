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
	int id; // 역번호
	int time; // 이동시간
	//int transfer; // 환승 횟수
	Station* next;
	Station() : next(NULL) {}
};

// 그래프 (맵)
class MatrixWGraph
{
private:
	string* station; // 역 이름
	Station* graph; // 그래프
	int* route; // 음수면 미포함, 양수면 포함된 값을 의미
public:
	// 다익스트라 알고리즘
	void Dijkstra(int start)
	{
		int minpos;
		// graph, route 배열 초기화
		for (int i = 0; i < MAX; i++)
		{
			route[i] = -1;
			graph[i].id = -1;
			graph[i].time = 10000;
			//graph[i].transfer = 1000;
		}

		// 시작점과 연결된 값에 스테이션의 값을 대입한다.
		for (Station* s = graph[start].next; s != NULL; s = s->next)
		{
			// 출발역이 환승역일 경우 환승 횟수와 시간을 0으로 지정
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

		// 출발역이 환승역일 경우 출발역과 같은 이름을 가진 역의 값을 0으로 초기화
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
		// 시작지점 초기화
		route[start] = 0;
		graph[start].time = 0;
		//graph[start].transfer = 0;


		for (int i = 0, prev = start; i < MAX - 6; i++)
		{
			// 최소값을 가진점의 인덱스 값을 구한다.
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

			//최소 점을 가진 데이터를 저장한다.
			route[minpos] = 0;

			// 선택된 점에 연결된 역들로 다른 점 값을 업데이트 하며 최단경로를 구한다.
			for (Station* s = graph[minpos].next; s != NULL; s = s->next)
			{
				// 시간 비교
				if (graph[minpos].time + s->time < graph[s->id].time)
				{
					graph[s->id].time = graph[minpos].time + s->time;
					//graph[s->id].transfer = graph[minpos].transfer + s->transfer;
				}
				/*
				// 시간이 같을 때 환승 횟수를 비교하여 환승 횟수가 작은 값 출력
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

		// 클래스 데이터 초기화
		graph = new Station[MAX];
		station = new string[MAX];
		route = new int[MAX];

		// 파일 읽기
		file.open(name, ios::in);

		int t1, t2, time;
		string s1, s2;

		while (!file.eof())
		{
			// 변수에 데이터 입력
			file >> t1;
			file >> t2;
			file >> time;
			file >> s1;

			// 파일 마지막 공백이 있을 경우 break
			if (file.eof())
				break;

			file >> s2;

			// 배열 index에 값을 넣어야 하기 때문에 -1 을 해준다. (배열이 0부터 시작이므로)
			t1 -= 1;
			t2 -= 1;

			// 새로운 경로를 만든다.
			temp = new Station();
			temp->id = t2;
			temp->time = time;
			//if (s1 == s2) temp->transfer = 1;
			//else temp->transfer = 0;

			// 서로의 점에 경로를 연결한다.
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

			// 역 이름을 배열에 저장
			station[t1] = s1;
			station[t2] = s2;
		}

		// 파일을 닫아준다.
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

	// 경로 출력
	void PrintLoad()
	{
		system("cls");
		int start = -1, end = -1;
		int min, v, prev;
		string t;
		stack<int> s, r;

		// start 변수와 end 변수를 비교하여 출발, 도착 역 입력
		while (start == end)
		{

			while (start < 0)
			{
				cout << "출발역 : ";
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
					cout << "없는 역 입니다." << endl;
			}

			while (end < 0)
			{
				cout << "도착역 : ";
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
					cout << "없는 역 입니다." << endl;
			}

			if (start == end)
			{
				cout << "출발역과 도착역이 같습니다." << endl;
				start = end = -1;
			}
		}


		cout << "선택 구간 : " << station[start] << " → " << station[end] << endl << endl;
		cout << "▶ 최단경로 :" << endl;

		// 다익스트라 알고리즘 시작
		this->Dijkstra(start);

		// 도착역 중 소요시간이 가장 작은 값을 선택한다.
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

		// 출발역의 인덱스와 찾은 값의 인덱스를 스택에 저장한다.
		this->FindRoute(s, start, end);

		// 스택에서 스택으로 이동 (역순 변경)
		while (!s.empty())
		{
			r.push(s.top());
			s.pop();
		}

		// 경로 출력
		for (v = 0, prev = -1; !r.empty(); v++)
		{
			if (prev != -1 && station[prev] == station[r.top()])
			{
				if (station[r.top()] != station[start])
					cout << "[환승]";

				v--;
				prev = r.top();
				r.pop();
			}
			else
			{
				if (v != 0)
					cout << " → ";
				else
					cout << " ";

				cout << station[r.top()];
				prev = r.top();
				r.pop();

			}
		}
		cout << endl << "▶ 이동 역 : " << v - 1 << endl
			<< "▶ 소요시간 : " << graph[end].time / 60 << "시간 " << graph[end].time % 60 << "분\t" << endl;
		system("pause");
		//<< "▶ 환승 횟수 : " << graph[end].transfer << endl;

	}

	// 완성된 스택에서 경로 찾기
	bool FindRoute(stack<int>& s, int start, int end)
	{
		s.push(start);

		// 시작 == 끝 일때 true return
		if (start == end)
			return true;
		// 시작점에 연결된 역 모두 조사
		for (Station* st = graph[start].next; st != NULL; st = st->next)
		{
			// 시작점값 + 역값과 역과 연결된 점 값이 같으면 (다익스트라 알고리즘에서 스택에 포함시킬 때 연결한 경로)
			if (graph[start].time + st->time == graph[st->id].time)// && graph[start].transfer + st->transfer == graph[st->id].transfer)
			{
				// 다음 경로가 없다면 pop
				if (!this->FindRoute(s, st->id, end))
				{
					s.pop();
				}
				else
				{
					// 경로를 찾았다면 경로를 찾았다고 true return
					return true;
				}
			}
		}
		return false;
	}
};

int main()
{
	// MatrixWGraph 클래스 변수 선언
	MatrixWGraph subway;

	// Station 클래스 변수 선언
	Station* temp = new Station();
	Station* station = new Station();

	int select = 0;

	//MatrixWGraph 클래스의 LoadMap 함수를 호출하면서 Station 클래스 변수 두개를 넘겨준다.
	subway.LoadMap("subway.txt", temp, station);

	while (true)
	{
		system("cls");

		cout << "*******************************" << endl;
		cout << "*       길찾기 프로그램       *" << endl;
		cout << "*******************************" << "\n\n\n" << endl;
		cout << "------------------------------" << endl;
		cout << "*  1. 지하철 노선 정보 확인  *" << endl << "*  2. 길찾기 프로그램 실행   *" << endl << "*  3. 프로그램 종료          *" << endl << "------------------------------" << endl<< "숫자 입력 : ";
		cin >> select;

		if (select == 1)
			subway.PrintMap();
		else if (select == 2)
			// MatrixWGraph 클래스의 PrintLoad 함수를 호출
			subway.PrintLoad();
		else if (select == 3)
		{
			cout << "프로그램 종료!" << endl;
			break;
		}
		else
		{
			cout << "1~3만 입력!" << endl;
			system("pause");
		}
	}




	// 클래스 변수 메모리 해제
	delete(temp);
	delete(station);

	return 0;
}