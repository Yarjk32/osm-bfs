#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

vector<vector<bool> > graph, used; //graph - ������� �������� �����, used - ���������� ����� �����
queue<vector<int> > q; //q - ����� ��� bfs
vector<int> route; //route - �������
int finish = -1, n; //finish - ������ ������ �����, n - ������� ������������ �����

int bfs() {
    //�������� �� ��'������ �����
    if (q.empty()) return -1;

    route = q.front();
    q.pop();
    int i, last = route.back(); //i - ������ ��� �����, last - ������� ����� ��������

    //����� �� �� ������������ ������� �����
    for (i = 0; i < n; i++) if (graph[last][i] && !used[last][i]) {
            route.push_back(i);
            if (i == finish) return route.size();
            q.push(route);
            route.pop_back();
            used[i][last] = 1;
            used[last][i] = 1;
    }
    return bfs();
}

int main()
{
    //�������� �������� �������� �����
    ifstream fin ("input.osm");
    if (!fin.is_open()) {
        cout << "File \"input.osm\" is not finded.\n";
        system("pause");
        return 0;
    }

    string line, lastNode, startNode, finishNode; //line - ������ ����� ��� �������������� �� id �����, lastNode - ��������� �����, startNode - �������� �����, finishNode - ������ �����
    int start = -1, tmp = 0, i, j, lastNodeIndex, nowNodeIndex, result, allNodes = 0; //start - ������ �������� �����, tmp - ��� ��������� ������� ��������, i - ������ ��� �����, j - ���������� ������ ��� �����, lastNodeIndex - ������ ���������� �����, nowNodeIndex - ������ ��������� �����, result - ������� ����� ���������� ��������, allNodes - ������� ��� �����, ������� �� ������
    vector<string> nodesIndexes; //nodesIndexes - ������ �����
    bool f; //f - ���������
    vector<bool> g0; //g0 - ����� ��� ��������� �������

    //��������� ������� �����
    cout << "Start node: ";
    cin >> startNode;
    cout <<  "Finish node: ";
    cin >> finishNode;

    //���������� �����
    cout << "Indexing nodes.\n";
    while(!fin.eof()) {
        f = 1;
        if (allNodes % 5000 == 0 && allNodes != tmp) {
            tmp = allNodes;
            cout << "Indexing nodes: found " << allNodes << " nodes, " << tmp << " nodes in index.\n";
        }
        getline(fin, line);
        if (line.find("<nd ref=") < 10000) {
            allNodes++;
            line.erase(0, line.find('=') + 2);
            line.erase(line.find_first_of("'\""), line.find('>') - line.find_first_of("'\"") + 1);
            for (i = 0; i < nodesIndexes.size(); i++) if (line == nodesIndexes[i]) {
                    f = 0;
                    break;
            }
            if (f) {
                nodesIndexes.push_back(line);
                if (line == startNode) start = nodesIndexes.size() - 1;
                if (line == finishNode) finish = nodesIndexes.size() - 1;
            }
        }
    }
    fin.close();
    cout << "Indexing nodes finished: found " << allNodes << " nodes, " << nodesIndexes.size() << " nodes in index.\n";

    //�������� �� �������� ������� ����� � �������� ����
    if (start < 0 || finish < 0) {
        if (start < 0 && finish < 0) cout << "Start and finish nodes are not found at \"input.osm\".";
        else {
            if (start < 0) cout << "Start node is not found at \"input.osm\".";
            else cout << "Finish node is not found at \"input.osm\".";
        }
        return 0;
    }

    n = nodesIndexes.size();
    route.push_back(start);
    q.push(route);
    tmp = 0;

    //�������� ���'�� ��� ������� �������� �� ������� ������������ �����
    cout << "Getting memory for graph.\n";
    for (i = 0; i < n; i++) {
        if (i % 10000 == 0 && i != tmp) {
            tmp = i;
            cout << "Getting memory for graph: " << tmp << '/' << n <<".\n";
        }
        g0.clear();
        for (j = 0; j < n; j++) g0.push_back(0);
        graph.push_back(g0);
        used.push_back(g0);
    }
    cout << "Getting memory for graph finished: " << n << '/' << n <<".\n";

    j = 0;
    tmp = 0;
    f = 0;

    //���������� ������� ��������
    cout << "Extracting graph.\n";
    fin.open("input.osm");
    while(!fin.eof()) {
        if (j % 5000 == 0 && j != tmp) {
            tmp = j;
            cout << "Extracting graph: " << tmp << '/' << allNodes << " nodes processed.\n";
        }
        getline(fin, line);
        if (line.find("<nd ref=\"") < 10000) {
            j++;
            line.erase(0, line.find('=') + 2);
            line.erase(line.find_first_of("'\""), line.find('>') - line.find_first_of("'\"") + 1);
            if (f) {
                for (i = 0; i < n; i++) if (nodesIndexes[i] == line) {
                    nowNodeIndex = i;
                    break;
                }
                graph[nowNodeIndex][lastNodeIndex] = 1;
                graph[lastNodeIndex][nowNodeIndex] = 1;
                lastNodeIndex = nowNodeIndex;
            }
            else {
                lastNode = line;
                for (i = 0; i < n; i++) if (nodesIndexes[i] == lastNode) {
                    lastNodeIndex = i;
                    break;
                }
                f = 1;
            }
        }
        else f = 0;
    }
    cout << "Extracting graph finished: " << j << '/' << allNodes << " nodes processed.\n";
    fin.close();

    //�������� bfs �� ��������� ������� ����� ��������
    cout << "BFS.\n";
    result = bfs();
    cout << "BFS finished.\n";

    //���� �������� ����
    if (result < 0) {
        cout << "Route doesn't exist.\n";
        system("pause");
        return 0;
    }

    //������� ��������
    cout << "Exporting route.\n";
    ofstream fout ("out.txt");
    for (i = 0; i < route.size(); i++) fout << nodesIndexes[route[i]] << ' ';
    fout.close();
    cout << "Route exported successfully.\n";

    system("pause");
    return 0;
}
