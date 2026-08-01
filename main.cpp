#include <bits/stdc++.h>
#include <cstdlib>
/// Template Dutzu
using namespace std;

int const mapSize[4]= {8,12,16,20};
int const nrMines[4]= {15,50,80,100};
int const dx[8]= {-1,0,1,0,-1,-1,1,1};
int const dy[8]= {0,1,0,-1,-1,1,1,-1};
int dificulty;
int n;
class Block {
public:
    bool mine;
    int x,y;
    bool marked, open;
    int value;
    bool visited;
    int openBlock() {
        if (marked)
            return 0;
        if (mine)
            return 2;
        open=1;
        return 1;
    }
    void getMineValue(int const &x, int const &y);
};
Block m[21][21];
bool inmat(int x, int y) {
    if (x<1 || x>n || y<1 || y>n)
        return 0;
    return 1;
}
void Block::getMineValue(int const &x, int const &y) {
    for (int k=0; k<8; k++) {
        int newx=x+dx[k];
        int newy=y+dy[k];
        if (inmat(newx,newy) and m[newx][newy].mine)
            this->value++;
    }
}
void mainMenu() {
    string dif;
    while(true) {
        cout<<"Selecteaza dificultatea:\n";
        cout<<"1) Easy = 8*8, 20 mines \n";
        cout<<"2) Medium = 12*12, 50 mines \n";
        cout<<"3) Hard = 16*16, 100 mines \n";
        cout<<"4) Insane = 20*20, 200 mines \n";
        cin>>dif;
        if (dif.size() == 1 && dif[0] >= '1' && dif[0] <= '4') {
            dificulty=dif[0]-'0';
            break;
        }
        system("cls");
        cout << "=== Te rog sa alegi un numar intre 1 si 4 ===\n\n";
    }
}
bool inMap(pair<int,int> const &pick) {
    if (pick.first<1 || pick.first>n || pick.second<1 || pick.second>n)
        return 0;
    return 1;
}
void genMap(int x, int y) {
    int mines=0;
    while (mines<nrMines[dificulty-1]) {
        int xrand,yrand;
        xrand=rand()%n+1;
        yrand=rand()%n+1;
        if (xrand==x and yrand==y)
            continue;
        if (m[xrand][yrand].mine)
            continue;
        m[xrand][yrand].mine=1;
        mines++;
    }

}
void makeChoice(int const &x, const int &y) {
    queue<pair<int,int>>q;
    q.push({x,y});
    m[x][y].visited=1;
    m[x][y].getMineValue(x,y);
    int found=0;
    while (!q.empty()) {
        pair<int,int> curent=q.front();
        pair<int,int> v;
        q.pop();
        for (int k=0; k<4; k++) {
            v.first=curent.first+dx[k];
            v.second=curent.second+dy[k];
            if (inmat(v.first,v.second) and m[v.first][v.second].visited==0 and !m[v.first][v.second].mine) {
                m[v.first][v.second].visited=1;
                m[v.first][v.second].getMineValue(v.first,v.second);
                if (!m[v.first][v.second].value)
                    q.push(v);
                else if (found<5) {
                    q.push(v);
                    found++;
                }
            }
        }
    }
}
void showMap() {
    /// [12345]
    system("cls");
    cout<<"\n";
    for (int i=1; i<=n; i++,cout<<'\n') {
        for (int j=1; j<=n; j++) {
            if (!m[i][j].visited and !m[i][j].marked) {
                if (i<=9)
                    cout<<"["<<i<<" ,";
                else
                    cout<<"["<<i<<",";
                if (j<=9)
                    cout<<" "<<j<<"] ";
                else
                    cout<<j<<"] ";
            } else if (m[i][j].marked)
                cout<<"("<<"MARKS"<<") ";
            else if (m[i][j].value<=9)
                cout<<"(  "<<m[i][j].value<<"  ) ";
            else
                cout<<"( "<<m[i][j].value<<"  ) ";
            ///cout<<m[i][j].mine<<' ';
        }
    }
}
bool badChoice(int const &x, int const &y) {
    if (m[x][y].mine)
        return 1;
    return 0;
}
bool allMarked() {
    for (int i=1; i<=n; i++)
        for (int j=1; j<=n; j++) {
            if (!m[i][j].visited and !m[i][j].marked)
                return 0;
            if (m[i][j].marked and m[i][j].mine==0)
                return 0;
        }

    return 1;
}
void makeChoices() {
    bool firstPick=1;
    while (true) {
        if (allMarked()) {
            showMap();
            cout<<"Ai terminat de gasit minele! Felicitari!\n";
            break;
        }
        pair<int,int>pick;
        int operatie=0;
        if (firstPick) {
            cout<<"Alege un block! Introdu coordonatele alese, exemplu: 6 3 pentru [6,3]\n";
            cin>>pick.first>>pick.second;
        } else {
            cout<<"Alege tipul operatiei: 1 = DIG, 2 = MARK, apoi coordonatele alese, exemplu 1 4 5\n";
            cin>>operatie>>pick.first>>pick.second;
        }

        if (!inMap(pick)) {
            if (firstPick) {
                showMap();
            }
            cout<<"ALEGE UN BLOCK CORECT!\n";
            continue;
        }
        if (firstPick) {
            genMap(pick.first,pick.second);
            makeChoice(pick.first,pick.second);
            firstPick=0;
            showMap();
        } else {
            if (operatie==1) {
                if (m[pick.first][pick.second].marked) {
                    cout<<"Nu poti alege un block marcat. Mai intai scoate mark-ul\n";
                    continue;
                }
                if (badChoice(pick.first,pick.second)) {
                    cout<<"AI ALES O MINA!";
                    return;
                }
                makeChoice(pick.first,pick.second);
                showMap();
            }
            if (operatie==2) {
                if (m[pick.first][pick.second].visited) {
                    cout<<"Nu poti marca o pozitie deja alesa\n";
                    continue;
                }
                m[pick.first][pick.second].marked=!m[pick.first][pick.second].marked;
                showMap();
            }
        }
    }
}
int main() {
    mainMenu();
    n=mapSize[dificulty-1];
    showMap();
    makeChoices();
    return 0;
}
