#include <array>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <string_view>
#include <vector>
class BoardLogic;
class Interface
{
public:
    void showTitleAndControls(const BoardLogic& logicObj);
    std::vector<std::vector<std::string_view>>& randomizeVectorFunc(BoardLogic& logicObj);
    void drawBoard(BoardLogic& logicObj,Interface& interfaceObj);
    void showMoveCount(const BoardLogic& logicObj);
    void showMove(BoardLogic& logicObj,Interface& interfaceObj);
    void quitOrNewGame(char mov,BoardLogic& logicObj,Interface& interfaceObj);
    void youWin()
    {
        std::cout<<"                         ,\n";
        std::cout<<"                        / \\\n";
        std::cout<<"                       /   \\\n";
        std::cout<<"                ______/     \\______\n";
        std::cout<<"                \\                 /\n";
        std::cout<<"                 \\               /\n";
        std::cout<<"                  \\   You Win!  /\n";
        std::cout<<"                  /             \\\n";
        std::cout<<"                 /               \\\n";
        std::cout<<"                /_____       _____\\\n";
        std::cout<<"                      \\     /\n";
        std::cout<<"                       \\   /\n";
        std::cout<<"                        \\,/\n";
    }
};

class BoardLogic
{
private:
    std::array<std::array<std::string_view,4>,4> m_tileChars{{
            {"1","2","3","4"},
            {"5","6","7","8"},
            {"9","10","11","12"},
            {"13","14","15","\0"}
        }};
    std::vector<std::vector<std::string_view>> randVec;
    enum class M_Moves
    {
        tileUp='s',
        tileDown='w',
        tileLeft='d',
        tileRight='a'
    };
    char* m_mov{new char};
    int m_moveCount{};
public:
    int checkIfOrdered()
    {
        for(int row{}; row<4; row++)
        {
            for(int col{}; col<4; col++)
                if(m_tileChars[row][col]!=randVec[row][col])return 0;
        }
        return 1;
    }
    std::vector<int> findBlankTile();
    void checkValidityThenSwap(std::vector<int>& blankTileCoord,char& mov);
    void registerMove (char* mov)
    {
        *mov+=(*mov<97)?32:0;
        std::vector<int> blankTileCoord=findBlankTile();
        checkValidityThenSwap(blankTileCoord,*mov);
    }
    void doSwap(std::vector<int>& blankTileCoord);
    int resetMoveCount()
    {
        m_moveCount=0;
        return m_moveCount;
    }
    friend class Interface;
};

void Interface::showMoveCount(const BoardLogic& logicObj)
{
    std::cout<<"\n                      Moves: "<<logicObj.m_moveCount<<"\n";
}

void Interface:: showTitleAndControls(const BoardLogic& logicObj)
{
    for(int i{}; i<=40; i++)std::cout<<"*";
    std::cout<<"\n     ___   __       __  __      __\n";
    std::cout<<"/|  |__   |__| |  |  /   / |   |_\n";
    std::cout<<"_|_ ___|  |    |__| /_  /_ |__ |__\n";
    for(int i{}; i<=40; i++)std::cout<<"*";
    std::cout<<"\n";
    std::cout<<"\n";
    std::cout<<"Arrange the tiles such that they appear in numeric order.\n";
    std::cout<<"\n";
    std::cout<<"W= Slide Tile Up                        S= Slide Tile Down\n";
    std::cout<<"A= Slide Tile Left                      D= Slide Tile Right\n";
    std::cout<<"Q=Quit                                  N=New Game";
    showMoveCount(logicObj);
}

int linSearch(std::vector<std::vector<std::string_view>>& vec,std::string_view& str)
{
    for(auto row{vec.begin()}; row<vec.end(); row++)
    {
        for(auto col{row->begin()}; col<row->end(); col++)
        {
            if(*col==str)return 1;
        }
    }
    return 0;
}

std::vector<std::vector<std::string_view>>& Interface::randomizeVectorFunc(BoardLogic& logicObj)
{
    std::mt19937 mt{static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution rowNum{0,3};
    std::uniform_int_distribution colNum{0,3};
    if(!empty(logicObj.randVec))logicObj.randVec.clear();
    logicObj.randVec.resize(4);
    for(int row{}; row<4; row++)
    {
        for(int col{}; col<4; col++)
        {
            std::string_view vecStr=logicObj.m_tileChars[rowNum(mt)][colNum(mt)];
            if(linSearch(logicObj.randVec,vecStr))col--;
            else
            {
                logicObj.randVec[row].push_back(vecStr);
            }
        }
    }
    return logicObj.randVec;
}
void Interface::showMove(BoardLogic& logicObj,Interface& interfaceObj)
{
    std::cout<<"\n                    Your Move: ";
    std::cin>>logicObj.m_mov;
    if(*logicObj.m_mov=='q' || *logicObj.m_mov=='Q' || *logicObj.m_mov=='n' || *logicObj.m_mov=='N')quitOrNewGame(*logicObj.m_mov, logicObj,interfaceObj);
    else
    {
        logicObj.registerMove(logicObj.m_mov);
    }
}
void Interface::drawBoard(BoardLogic& logicObj,Interface& interfaceObj)
{
    std::cout<<"                 _____ _____ _____ _____\n";
    for(int row{}; row<4; row++)
    {
        std::cout<<"                |     |     |     |     |\n";
        int col{0};
        for(col; col<4; col++)
        {
            if (col==0)std::cout<<"                ";
            if(logicObj.randVec[row][col].empty())std::cout<<"|"<<logicObj.randVec[row][col]<<"     ";
            else if(logicObj.randVec[row][col].length()>=2)std::cout<<"|  "<<logicObj.randVec[row][col]<<" ";
            else std::cout<<"|  "<<logicObj.randVec[row][col]<<"  ";
        }
        std::cout<<"|";
        std::cout<<"\n                |_____|_____|_____|_____|\n";
    }
    showMove(logicObj,interfaceObj);
}

std::vector<int>BoardLogic::findBlankTile()
{
    std::vector<int> blankTileCoord;
    for(int row{}; row<4; row++)
    {
        for(int col{}; col<4; col++)
        {
            if(randVec[row][col]=="\0")
            {
                blankTileCoord.push_back(row);
                blankTileCoord.push_back(col);
            }
        }
    }
    return blankTileCoord;
}
void carryGameSequence(BoardLogic& logicObj,Interface& interfaceObj);
void randomizeBoard(BoardLogic& logicObj,Interface& interfaceObj);
void quitGame(BoardLogic& logicObj,Interface& interfaceObj)
{
    std::cout<<"quit the game (y/n)?:";
    char resp;
    std::cin>>resp;
    while(resp!='y'&& resp!='Y' && resp!='n' && resp!='N')
    {
        std::cout<<"Please Enter a Valid response:";
        std::cin>>resp;
    }
    if(resp=='Y' || resp=='y') std::exit(0);
    else
    {
        carryGameSequence(logicObj,interfaceObj);
    }
}

void newGame(BoardLogic& logicObj,Interface& interfaceObj)
{
    std::cout<<"start a new game (y/n)?:";
    char resp;
    std::cin>>resp;
    while(resp!='y'&& resp!='Y' && resp!='n' && resp!='N')
    {
        std::cout<<"Please Enter a Valid response:";
        std::cin>>resp;
    }
    if(resp=='Y' || resp=='y')
    {
        logicObj.resetMoveCount();
        randomizeBoard(logicObj,interfaceObj);
        carryGameSequence(logicObj,interfaceObj);
    }
    else
    {
        carryGameSequence(logicObj,interfaceObj);
    }
}
void Interface::quitOrNewGame(char mov,BoardLogic& logicObj,Interface& interfaceObj)
{
    std::cout<<"    Are you sure you want to ";
    if(mov=='q' || mov=='Q')quitGame(logicObj,interfaceObj);
    else newGame(logicObj,interfaceObj);
}

void swapper (std::string_view& a,std::string_view& b)
{
    std::swap(a,b);
}
void BoardLogic::doSwap(std::vector<int>& blankTileCoord)
{
    int row = blankTileCoord[0];
    int col = blankTileCoord[1];
    char mov = *m_mov;
    switch (static_cast<M_Moves>(mov))
    {
    case M_Moves::tileUp:
        swapper(randVec[row][col], randVec[row - 1][col]);
        break;
    case M_Moves::tileDown:
        swapper(randVec[row][col], randVec[row + 1][col]);
        break;
    case M_Moves::tileLeft:
        swapper(randVec[row][col], randVec[row][col - 1]);
        break;
    case M_Moves::tileRight:
        swapper(randVec[row][col], randVec[row][col + 1]);
        break;
    default:
        std::cout << "Invalid Move";
        break;
    }

}

int isValid(std::vector<int>& blankTileCoord,char& mov)
{
    if((mov=='s' && blankTileCoord[0]==0) ||
            (mov=='d' && blankTileCoord[1]==0)||
            (mov=='w' && blankTileCoord[0]==3)||
            (mov=='a' && blankTileCoord[1]==3)
      )
    {
        std::cout<<"                      Invalid Move!";
        return 0;
    }
    return 1;
}

void BoardLogic::checkValidityThenSwap(std::vector<int>& blankTileCoord,char& mov)
{
    if(isValid(blankTileCoord,mov))
    {
        m_moveCount++;
        doSwap(blankTileCoord);
    }
}
void randomizeBoard(BoardLogic& logicObj,Interface& interfaceObj)
{
    interfaceObj.randomizeVectorFunc(logicObj);
}
void carryGameSequence(BoardLogic& logicObj,Interface& interfaceObj)
{
    system("cls");
    interfaceObj.showTitleAndControls(logicObj);
    if(logicObj.checkIfOrdered())
    {
        interfaceObj.youWin();
        interfaceObj.showMove(logicObj,interfaceObj);
        return ;
    }
    else
    {
        interfaceObj.drawBoard(logicObj,interfaceObj);
    }
}
int main()
{
    BoardLogic logicObj;
    Interface interfaceObj;
    randomizeBoard(logicObj,interfaceObj);
    while(true)
    {
        carryGameSequence(logicObj,interfaceObj);
    }
    return 0;
}



