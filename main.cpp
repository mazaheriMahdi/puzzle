#include<iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

//RANDOM IN RANGE -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
int randrange(int low,int high)
{
    return rand()%(high-low+1)+low;
}
//direction
enum Directtion{
    Up ,
    Down ,
    Left ,
    Right
};
//ONE PUZZLE MEMEBER
struct PuzzleOBJ{

    int number = 1;
    int x = 0  , y = 0;

    PuzzleOBJ& operator=(PuzzleOBJ value){
        number = value.number;
        x = value.x;
        y = value.y;
        return *this;
    }
};
//MAIN STRUCT FOR PUZZLE -=##=--=##=--=##=--=##=--=##=--=##=-
struct PuzzleWindow{
    PuzzleOBJ** puzzleObj = new PuzzleOBJ*[0];
    int sizeX  = 3, sizeY = 3;
    bool isSolvable = false , showProgress = true;
    int blankSpaceX = 0 , blankSpaceY = 0;
    //SAVE VAR TO TXT FILE -=-=-=--=-=-=--=-=-=--=-=-=--=-=-=--=-=-=--=-=-=--=-=-=--=-=-=--=-=-=--=-=-=-
    void saver(){
        ofstream file;
        file.open("data.txt");
        file << sizeX << endl << sizeY << endl;
        file << isSolvable << endl << showProgress << endl;
        file << blankSpaceX << endl << blankSpaceY << endl;
        for(int i = 0 ; i < sizeX ; i++){
            for(int j = 0 ; j< sizeY ; j++){
                file << puzzleObj[i][j].number << endl << puzzleObj[i][j].x << endl << puzzleObj[i][j].y << endl;
            }
        }
    }
    void reader(){
        ifstream file;
        file.open("data.txt");
        string str;
        getline(file , str);
        sizeX = stoi(str);
        getline(file , str);
        sizeY = stoi(str);
        getline(file , str);
        isSolvable = stoi(str);
        getline(file , str);
        showProgress = stoi(str);
        getline(file, str);
        blankSpaceX = stoi(str);
        getline(file , str);
        blankSpaceY = stoi(str);
        PuzzleOBJ** temp = new PuzzleOBJ*[sizeY];
        puzzleObj = nullptr;
        puzzleObj = temp;
        temp = nullptr;
        for( int i = 0 ;  i  < sizeX ; i++){
            PuzzleOBJ* pp = new PuzzleOBJ[sizeY];
            for(int j = 0 ; j < sizeY ; j ++){

                getline(file,str);
                pp[j].number = stoi(str);
                getline(file , str);
                pp[j].x = stoi(str);
                getline(file , str);
                pp[j].y = stoi(str);
            }
            puzzleObj[i] = pp;
            pp = nullptr;
        }
//        cout << sizeX;
//
    }


    //GENGERATE MEMBERS -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
    void generate(int x , int y){
        PuzzleOBJ** resize = new PuzzleOBJ*[y];
        int numbers[x][y];
        puzzleObj = nullptr;
        puzzleObj  = resize;
        resize = nullptr;

        int lb =  1;
        int ub= x*y;
        bool isNew = false;
        int index= 0;


        for(int i = 0 ; i < x ; i++){
            PuzzleOBJ* pp = new PuzzleOBJ[y];
            for(int j = 0 ; j< y ; j++){
                index++;
                if(i == x-1 && j==y-1){
                    (pp + j)->number=-1;
                    (pp + j)->x=i;
                    (pp + j)->y=j;

                }else{
                    (pp + j)->number=index;
                    (pp + j)->x=i;
                    (pp + j)->y=j;

                }

            }
            puzzleObj[i] = pp;
        }









    }
    //THIS FUNTION IS GOING TO SHUFFLE MEMBERS -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
    void shuffle(){
        srand(time(NULL));
        for (int i = 0; i < sizeX-1; i++)
        {
            for(int j = 0 ; j < sizeY-1;j++){
                int x = rand() % (sizeX-i) + i;
                int y = rand() % (sizeY-i) + i;
                PuzzleOBJ temp;
                temp.number = puzzleObj[i][j].number;
                temp.x = puzzleObj[i][j].x;
                temp.y = puzzleObj[i][j].y;
                puzzleObj[i][j].number = puzzleObj[x][y].number;
                puzzleObj[i][j].x = puzzleObj[x][y].x;
                puzzleObj[i][j].y = puzzleObj[x][y].y;
                puzzleObj[x][y].number = temp.number;
                puzzleObj[x][y].x = temp.x;
                puzzleObj[x][y].y = temp.y;

            }
        }

    }

    void setBlankSpaceLoc(){
        for(int i = 0 ; i < sizeX ; i++){
            if(isSolvable)break;
            for(int j = 0 ; j < sizeY ; j++){
                if(puzzleObj[i][j].number == -1){
                    blankSpaceX = i;
                    blankSpaceY = j;
                    isSolvable = true;
                    break;

                }
            }
        }
    }
    //MAKE PUZZLE SOLVABLE -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
    void makeItSolvable(){
        bool isfound = false;
        while (!isSolvable){
            //FIN BLANK SPACE PLACE -=##=--=##=--=##=--=##=--=##=--=##=--=##=-
            setBlankSpaceLoc();

            //COUNT NUMBER OF INVERSION IN PUZZLE -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
            int inversion = 0 ;
            for(int o = 0 ; o < sizeX ; o++){
                for(int i = 0 ; i < sizeY ; i++){
                    int selectedNumber = puzzleObj[o][i].number;
//                    cout << "selected number : " << selectedNumber << endl;
                    bool temp = true;
                    for(int j = o ; j  < sizeX ; j++){
                        for(int k = 0 ; k < sizeY ; k++){
                            if(temp) {
                                k = i + 1;
                                temp = false;
                            };
                            if(( selectedNumber >  puzzleObj[j][k].number) && puzzleObj[j][k].number != -1){
                                inversion++;
//                                cout << "inversion ++ for : " << selectedNumber << endl;
                            }else{

                            }

                        }
                    }
                }
            }
//            cout << inversion << endl;

            //CHEKC IF IT SOLVABLE OR NOT -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
            if((sizeX % 2) == 1){
                if ((inversion % 2)==0){
                    isSolvable = true;
                    break;
                }else{
                    isSolvable = false;
                    shuffle();

                }

            }else if((sizeX % 2) == 0 ){
                cout << "we enter the else " << endl;
                if(((blankSpaceX % 2) == 0)  && (inversion%2) == 1){
                    isSolvable = true;
                    break;
                }else if( ( ( blankSpaceX % 2 ) == 1 )  && (inversion%2) == 0){
                    isSolvable = true;
                    break;
                }else{
                    isSolvable = false;
                    shuffle();
                }

            }

        }






    }
    bool isAllSet(){
        int index = 0 ;
        int temp;
        bool isTrue = true;
        for(int i = 0 ; i < sizeX; i++){
            for(int j = 0 ; j < sizeY ; j++){
                index++;
                if(puzzleObj[i][j].number != -1){
                    temp = puzzleObj[i][j].number;
                    if(temp != index){

                        isTrue = false;

                    }
                }

            }
        }
        return isTrue;
    }


    string progress(){
        int counter = 0;
        int index = 0 ;
        int temp;
        bool isTrue = true;
        for(int i = 0 ; i < sizeX; i++){
            for(int j = 0 ; j < sizeY ; j++){
                index++;
                if(puzzleObj[i][j].number != -1){
                    temp = puzzleObj[i][j].number;
                    if(temp == index){

                        counter++;


                    }else{
                        break;
                    }
                }

            }
        }
        string pr = "\033[1;31;41m";
        for(int i = 0 ; i < counter; i++){
            pr +=" ";
        }
        pr+= "\033[0m\n";
        return pr;
    }

    void moveBlank(Directtion direction){
        int temp;
        if(direction == Directtion::Right){
            if(blankSpaceY + 1 < sizeY){
                puzzleObj[blankSpaceX][blankSpaceY].y++;
                puzzleObj[blankSpaceX][blankSpaceY+1].y--;
                temp  = puzzleObj[blankSpaceX][blankSpaceY].number;
                puzzleObj[blankSpaceX][blankSpaceY].number = puzzleObj[blankSpaceX][blankSpaceY+1].number;
                puzzleObj[blankSpaceX][blankSpaceY+1].number = temp;
                blankSpaceY++;
            }
        }else if(direction == Directtion::Left){
            if(blankSpaceY != 0){
                puzzleObj[blankSpaceX][blankSpaceY].y--;
                puzzleObj[blankSpaceX][blankSpaceY-1].y++;
                temp  = puzzleObj[blankSpaceX][blankSpaceY].number;
                puzzleObj[blankSpaceX][blankSpaceY].number = puzzleObj[blankSpaceX][blankSpaceY-1].number;
                puzzleObj[blankSpaceX][blankSpaceY-1].number = temp;
                blankSpaceY--;
            }
        }else if(direction == Directtion::Up) {
            if (blankSpaceX != 0) {
                puzzleObj[blankSpaceX][blankSpaceY].x--;
                puzzleObj[blankSpaceX - 1][blankSpaceY].x++;
                temp = puzzleObj[blankSpaceX][blankSpaceY].number;
                puzzleObj[blankSpaceX][blankSpaceY].number = puzzleObj[blankSpaceX - 1][blankSpaceY].number;
                puzzleObj[blankSpaceX - 1][blankSpaceY].number = temp;
                blankSpaceX--;
            }
        }else{
            if (blankSpaceX +1 <  sizeX) {
                puzzleObj[blankSpaceX][blankSpaceY].x++;
                puzzleObj[blankSpaceX + 1][blankSpaceY].x--;
                temp = puzzleObj[blankSpaceX][blankSpaceY].number;
                puzzleObj[blankSpaceX][blankSpaceY].number = puzzleObj[blankSpaceX + 1][blankSpaceY].number;
                puzzleObj[blankSpaceX + 1][blankSpaceY].number = temp;
                blankSpaceX++;
            }

        }
    }


};
ostream& operator<<(ostream& stream , PuzzleWindow puzzle){

    for(int i = 0 ; i < puzzle.sizeX ; i++){
        for(int j = 0 ; j < puzzle.sizeY ; j ++){
            if(puzzle.puzzleObj[i][j].number == -1){
                stream << "\t";
            }else{
                stream << puzzle.puzzleObj[i][j].number << "\t";

            }
        }
        stream << endl;
    }
    return stream;
}
char keyHandler(){
    char c;
    system("stty raw");
    c = getchar();
    // terminate when "." is pressed
    system("stty cooked");
    system("clear");
    return c;
}
//
void menu( PuzzleWindow &puzzleWindow);
//START PROGRAM WITH THIS FUNTION -=##=--=##=--=##=--=##=--=##=--=##=--=##=--=##=-
void startProgram(PuzzleWindow &puzzle ){
    while(true) {
        char c = keyHandler();
        if(c == '.') {
            exit(0);
        }else if(c == 'B'){
            puzzle.moveBlank(Directtion::Up);
        }else if(c == 'C'){
            puzzle.moveBlank(Directtion::Left);
        }else if(c == 'D'){
            puzzle.moveBlank(Directtion::Right);
        }else if(c == 'A'){
            puzzle.moveBlank(Directtion::Down);
        }else if(c == 's' || c=='S'){
            puzzle.saver();
            cout << "Your game has been saved :)" << endl;
        }
        if(puzzle.isAllSet()) {
            cout << "solved";
            break;
        };
        cout << "\033[1;31;1m";
        cout << "#__________________________________________________________#" << endl;
        cout << "#use arrow keys to move numbers heat . on keyboard to quit.#" << endl;
        cout << "#                 hit 's' to save your game                #" << endl;
        cout << "#                        good luck :)                      #" << endl;
        cout << "#==========================================================#" << endl;
        cout << "\033[0m";
        cout << puzzle;
        cout << "+ |Progress : "<< puzzle.progress();

//        cout << puzzle.blankSpaceX << " " << puzzle.blankSpaceY << endl;
    }
}
void menu( PuzzleWindow &puzzleWindow){
    int x , y;
    string options [] = {"Continuation of the previous game" , "5*5 Puzzle" , "4*4 Puzzle" , "3*3 Puzzle" , "Manual Puzzle" , "Exit"};
    int index = 0 ;

    while (true){
        system("clear");
        cout << "\033[1;31;1m";
        cout <<  R"(   _________                         __                      ___ )"  << endl;
        cout <<  R"(  <  / ____/  ____  __  __________  / /__              _____/ (_))"  << endl;
        cout <<  R"(  / /___ \   / __ \/ / / /_  /_  / / / _ \            / ___/ / / )"  << endl;
        cout <<  R"( / /___/ /  / /_/ / /_/ / / /_/ /_/ /  __/           / /__/ / /  )"  << endl;
        cout <<  R"(/_/_____/  / .___/\__,_/ /___/___/_/\___/  ______    \___/_/_/   )"  << endl;
        cout <<  R"(          /_/                             /_____/                )"  << endl;
        cout << "\033[0m";
        for(int i = 0 ; i < size(options) ; i++){
            if(i == index){
                cout << "+| " << "\033[1;30;41;1m  "+options[i]+"  \033[0m \033[1;30;41m hit E on keyboard to select.\033[0m" <<  endl;
            }else{
                cout << "+| " << options[i] << endl;
            }
        }
        char c = keyHandler();
        if(c == 'A'){
            if(index > 0 ){
                index--;
            }
        }else if(c == 'B'){
            if(index < size(options) - 1){
                index++;
            }
        }else if(c == 'E' || c=='e'){
            switch (index) {
                case  0 :
                    x = -1;
                    y = -1;
                    break;
                case 1:
                    x = 5;
                    y = 5;
                    break;
                case 2:
                    x = 4;
                    y = 4;
                    break;
                case 3:
                    x = 3;
                    y = 3;
                    break;
                case 4:
                    system("clear");
                    cout << "Enter number of rows : ";
                    cin >> x;
                    cout << "Enter Number of columns : ";
                    cin >> y;
                    break;
                case 5:
                    system("stty cooked");
                    exit(0);
                    break;

            }
            system("clear");
            break;
        }


    }

    if(x != -1){
        puzzleWindow.sizeY = y;
        puzzleWindow.sizeX = x;
        puzzleWindow.generate( x, y);
        puzzleWindow.shuffle();
        puzzleWindow.makeItSolvable();
        puzzleWindow.setBlankSpaceLoc();
        startProgram(puzzleWindow);
    }else {
//        cout << "starting to input";
        puzzleWindow.reader();
        puzzleWindow.setBlankSpaceLoc();
        startProgram(puzzleWindow);
//        cout << puzzleWindow;
    }




}



int main() {
    PuzzleWindow puzzleWindow;
    menu(puzzleWindow);
}