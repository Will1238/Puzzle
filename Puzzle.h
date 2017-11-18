#include <stdlib.h>
#include <time.h>
#define PATH_MAXSIZE 100000
#define W 26
#define H 26

using namespace std;

const string wall[]={"  ","╋","┃","━",
                     "┏","┓","┗","┛",
                     "┳","┻","┣","┫"};
const string path[]={"-","|"};

struct position{                                                                //坐标类
    int x;
    int y;

    position(int _x=-1,int _y=-1){x=_x;y=_y;}
    void out(void){cout<<"("<<y+1<<","<<x+1<<") ";}
    void change(int _x,int _y){x=_x;y=_y;}
};

struct node{                                                                    //队列结点类
    position data;
    node* next;

    node(void):data(0){next=NULL;}
    node(position _data){data.x=_data.x;data.y=_data.y;next=NULL;}
};

class Pqueue{                                                                   //队列类
public:
    node* first;
    node* tail;

    Pqueue(void){first = tail = NULL;}
    void addNode(position);
    void outNode(void);
    int isEmpty(void){return (NULL==first)? 1:0;}
};

void Pqueue::addNode(position _data)                                            //入队
{
    if(isEmpty()){
        first = new node(_data);
        tail = first;
    }
    else{
        tail->next = new node(_data);
        tail = tail->next;
    }
}

void Pqueue::outNode(void)                                                      //出队
{
    node *temp = first->next;
    delete first;
    first = temp;
}

class SeekPath{                                                                 //寻路类
    position Pstack[PATH_MAXSIZE];
    int top;
    int puzzle[H][W];
    string maze[H][W];
    int mark[H][W];
    position Move[8];

public:
    position in,out;

    SeekPath(void){
        Move[0].change(0,1);
        Move[1].change(-1,0);
        Move[2].change(0,-1);
        Move[3].change(1,0);
    }

    void puzzle_initialize(void);
    int seek(position now);
    void seekQueue(void);
    int seek_shortest(position now);
    void tail(void);
    void show(void);
};

void SeekPath::puzzle_initialize(void)                                          //初始化
{

    srand((unsigned)time(NULL));
    top=0;

    for(int i=0;i<H;i++)
        for(int j=0;j<W;j++)
            puzzle[i][j]=(int)(((double)rand()/RAND_MAX)+0.4);                  //特别的，此处常数本为0.5，用于四舍五入，但考虑到实际
                                                                                //墙面过多，所以特调节常数为0.4
    puzzle[0][0]=puzzle[H-1][W-1]=0;
    in.change(0,0);
    out.change(W-1,H-1);

    for(int i=0;i<H;i++)
        for(int j=0;j<W;j++)
            mark[i][j]=0;
}

void SeekPath::seekQueue(void)                                                  //标出路径长度标记
{
    Pqueue temp;
    temp.addNode(in);
    mark[in.y][in.x]=2;
    while(!temp.isEmpty()){
        node now = *(temp.first);
        temp.outNode();
        if(now.data.x == out.x && now.data.y == out.y) break;
        for(int i=0;i<4;i++){
             int y=now.data.y+Move[i].y;                                        //定义试探坐标
             int x=now.data.x+Move[i].x;
            if(y>=0 && y<H && x>=0 && x<W){                                     //试探坐标在地图内
                if(puzzle[y][x]!=1){                                            //试探坐标不是墙，可走
                   if(mark[y][x]==0){                                           //试探坐标没试探过
                           mark[y][x] = mark[now.data.y][now.data.x]+1;
                           temp.addNode(position(x,y));
                   }
                   else if(mark[y][x] > mark[now.data.y][now.data.x]+1){        //将路径长度更新为最短
                           mark[y][x] = mark[now.data.y][now.data.x]+1;
                           temp.addNode(position(x,y));
                   }
                }
            }
        }

    }

}

int SeekPath::seek_shortest(position now)                                       //寻找最短路径
{
    if(now.x == out.x && now.y == out.y)
        return 1;

    for(int i=0;i<8;i++){
        int y=now.y+Move[i].y;
        int x=now.x+Move[i].x;
      if(y>=0 && y<H && x>=0 && x<W){
        if(puzzle[y][x]!=1 &&
           mark[y][x] == mark[now.y][now.x]+1){
            if(seek_shortest(position(x,y)))
            {
                Pstack[top].x=x;
                Pstack[top].y=y;
                top++;
                return 1;
            }
        }
      }
    }

    return 0;
}

void SeekPath::show(void)                                                       //输出最短路径
{
    if(top==1)
        cout<<"There are no path!"<<endl;
    else{
        for(int i=top-1;i>=0;i--){
            Pstack[i].out();
        }
        cout<<endl;
    }

    for(int i=0;i<H ;i++){
    //Output the first line
        if(0==i){
            for(int j=0;j<W;j++){
                if(puzzle[i][j]==0) cout<<wall[0];
                else if(puzzle[i][j]==1 && puzzle[i+1][j]==0 && j<W)  cout<<wall[3];
                else if(puzzle[i][j]==1 && puzzle[i+1][j]==1 && j<W-1)  cout<<wall[8];
                else if(puzzle[i][j]==1 && puzzle[i+1][j]==1 && j==W)  cout<<wall[5];
            }
        }
    //Output the last line
        else if(H-1==i){
            for(int j=0;j<W;j++){
                if(puzzle[i][j]==0 ) cout<<wall[0];
                else if(puzzle[i][j]==1 && j==0) cout<<wall[6];
                else if(puzzle[i][j]==1 && puzzle[i-1][j]==0 && j<W)  cout<<wall[3];
                else if(puzzle[i][j]==1 && puzzle[i-1][j]==1 && j<W-1)  cout<<wall[9];
            }
        }
    //Output the other lines
        else{
            for(int j=0;j<W;j++){
                if(puzzle[i][j]==0) cout<<wall[0];
                else if(puzzle[i][j]==1 && j==0){
                     if(puzzle[i][j+1]==1) cout<<wall[10];
                     else cout<<wall[2];
                }
                else if(puzzle[i][j]==1 && j==W-1){
                     if(puzzle[i][j-1]==1) cout<<wall[11];
                     else cout<<wall[2];
                }
                else{//puzzle[i][j]==1 && j>0 && j<w-1
                     if(puzzle[i][j-1]==1 && puzzle[i][j+1]==1) cout<<wall[1];
                     else if(puzzle[i][j-1]==1 && puzzle[i][j+1]==0) cout<<wall[11];
                     else if(puzzle[i][j-1]==0 && puzzle[i][j+1]==1)  cout<<wall[10];
                     else   cout<<wall[2];  //puzzle[i][j-1]==0 && puzzle[i][j+1]=0
                }
            }
        }
        cout<<endl;
    }cout<<endl;
    cout<<"Press \"Enter\" to continue or Input 'N' to end:";
}

void SeekPath::tail(void)                                                       //将出口入栈
{
    Pstack[top].x=in.x;
    Pstack[top].y=in.y;
    top++;
}

