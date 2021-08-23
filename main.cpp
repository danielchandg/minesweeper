//include all needed files/libraries
#include "SFML/Graphics.hpp" //graphics library
#include "SFML/Window.hpp" //graphics library
#include <iostream> //contains functions like cout, cin
#include <vector> //ability to use vectors (dynamic arrays)
#include <cmath>
#include <set>
#include <stdio.h> //implementing all the necessary libraries
#include <time.h> //has functions to manipulate the date and time info
#include <stdlib.h>
#include <queue>
#include <string>

using namespace std;

 // pictures of the bombs/mines and the flags taken from your computer
 // Edit this to the directory of the picture folder. It should end in a /
 string path = "C:/Users/Danie/minesweeper/pictures/";


//using a render window for 2D graphics
sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");

//structure for one square of minesweeper
struct square
{
 sf::Sprite minesquare;
 bool isVisible;
 int value;
 int row;
 int column;
 bool flag;
};

//controls the creation of the bombs and where to place them on the board
void generateBombs(vector<vector<square> >&board, int number_of_bombs, int x, int y)
{
 set<int> bombs;
 int start[] = {0,0,-1,0,-1,1,0,1,1,1,1,0,1,-1,0,-1,-1,-1}; //where to place the bombs initially
 int fake_bombs = 0;
 int rows = board.size(), columns = board[0].size();

 //first coordinate is a special case because there can be no bomb there, so puts "fake" bombs there
 for(int i=0; i<17; i+=2)
 {
   if (x+start[i] >= 0 && x+start[i] < rows && y+start[i+1] >=0 && y+start[i+1] < columns)
   {
     bombs.insert(((x+start[i])*columns) + ((y+start[i+1])%columns));
     fake_bombs++;
   }
 }

 srand(time(NULL));
 int num_bombs = bombs.size();

 //puts random bombs on the board
 while(num_bombs-fake_bombs < number_of_bombs && num_bombs < rows * columns)
 {
   bombs.insert(rand() % (rows * columns));
   num_bombs = bombs.size();
 }

 //takes away the fake bombs
 for (int i=0; i<17; i+=2)
 {
   if (x+start[i] >= 0 && x+start[i] < rows && y+start[i+1] >= 0 && y+start[i+1] < columns)
   {
     bombs.erase(((x+start[i]) * columns) + ((y+start[i+1]) % columns));
   }
 }

 //initializes the board for gameplay with numbers on spaces based on surrounding bombs
 for (set<int>::iterator it = bombs.begin(); it != bombs.end(); it++)
 {
   board[*it / columns][*it % columns].value = -1;
   for(int i=2; i<17; i+=2)
   {
     if (*it / columns+start[i] >= 0 && *it / columns+start[i] < rows && *it % columns+start[i+1] >= 0 && *it % columns+start[i+1] < columns &&  board[*it / columns+start[i]][*it % columns+start[i+1]].value >= 0)
     {
       board[*it / columns+start[i]][*it % columns+start[i+1]].value++;
     }
   }
 }
}

//driver method for our minesweeper
int main(int argc, char *argv[])
{

 //initializing the SMFL textures
 sf::Texture mine0_texture, mine1_texture, mine2_texture, mine3_texture, mine4_texture, mine5_texture, mine6_texture, mine7_texture, mine8_texture, minebomb_texture, mineboom_texture, mineflag_texture, minepressed_texture, mineflagpressed_texture, minesquare_texture, backdrop_texture, backdrop1_texture, button_custom_texture, button_custom1_texture, button_easy_texture, button_easy1_texture, button_expert_texture, button_expert1_texture, button_medium_texture, button_medium1_texture, cursor1_texture, cursor2_texture, back_button_texture, backdrop2_texture, slider_texture;

 mine0_texture.loadFromFile(path + "mine0.png");
 mine1_texture.loadFromFile(path + "mine1.png");
 mine2_texture.loadFromFile(path + "mine2.png");
 mine3_texture.loadFromFile(path + "mine3.png");
 mine4_texture.loadFromFile(path + "mine4.png");
 mine5_texture.loadFromFile(path + "mine5.png");
 mine6_texture.loadFromFile(path + "mine6.png");
 mine7_texture.loadFromFile(path + "mine7.png");
 mine8_texture.loadFromFile(path + "mine8.png");
 minebomb_texture.loadFromFile(path + "minebomb.png");
 mineboom_texture.loadFromFile(path + "mineboom.png");
 mineflag_texture.loadFromFile(path + "mineflag.png");
 minepressed_texture.loadFromFile(path + "minepressed.png");
 mineflagpressed_texture.loadFromFile(path + "mineflagpressed.png");
 minesquare_texture.loadFromFile(path + "minesquare.png");
 backdrop_texture.loadFromFile(path + "backdrop.png");
 backdrop1_texture.loadFromFile(path + "backdrop1.png");
 backdrop2_texture.loadFromFile(path + "backdrop2.png");
 button_custom_texture.loadFromFile(path + "button custom.png");
 button_custom1_texture.loadFromFile(path + "button custom1.png");
 button_easy_texture.loadFromFile(path + "button easy.png");
 button_easy1_texture.loadFromFile(path + "button easy1.png");
 button_expert_texture.loadFromFile(path + "button expert.png");
 button_expert1_texture.loadFromFile(path + "button expert1.png");
 button_medium_texture.loadFromFile(path + "button medium.png");
 button_medium1_texture.loadFromFile(path + "button medium1.png");
 cursor1_texture.loadFromFile(path + "cursor1.png");
 cursor2_texture.loadFromFile(path + "cursor2.png");
 back_button_texture.loadFromFile(path + "back button.png");
 slider_texture.loadFromFile(path + "slider.png");
 sf::IntRect cursor1_rect(0,0,88,108);
 sf::IntRect cursor2_rect(0,0,98,104);

 //initialize all needed square spaces and variables
 int phase = 0;
 bool hasBombs=false;
 vector<vector<square> > board;
 queue<square> q, q2;
 square initial_square, next;
 initial_square.minesquare.setTexture(minesquare_texture);
 initial_square.minesquare.setPosition(0,0);
 initial_square.minesquare.setScale(0.5f,0.5f);
 initial_square.isVisible = false;
 initial_square.value = 0;
 initial_square.row = 0;
 initial_square.column = 0;
 initial_square.flag = false;

 //initialize graphics
 sf::Sprite backdrop;
 backdrop.setPosition(0,0);
 sf::Sprite cursor;
 cursor.setScale(0.5f,0.5f);
 cursor.setOrigin(18.0f,0.0f);

 sf::Sprite button_easy, button_medium, button_expert, button_custom;
 sf::Sprite back_button;
 sf::Sprite slider_row, slider_column, slider_bombs;
 button_easy.setTexture(button_easy_texture);
 button_medium.setTexture(button_medium_texture);
 button_expert.setTexture(button_expert_texture);
 button_custom.setTexture(button_custom_texture);
 button_easy.setScale(0.7f,0.7f);
 button_medium.setScale(0.7f,0.7f);
 button_expert.setScale(0.7f,0.7f);
 button_custom.setScale(0.7f,0.7f);
 button_easy.setPosition(30,400);
 button_medium.setPosition(230,400);
 button_expert.setPosition(430,400);
 button_custom.setPosition(630,400);
 back_button.setTexture(back_button_texture);
 back_button.setScale(0.6f,0.6f);
 back_button.setPosition(10,25);

 // create sliders for custom menu
 slider_row.setTexture(slider_texture);
 slider_column.setTexture(slider_texture);
 slider_bombs.setTexture(slider_texture);
 float slider_size=0.5f;
 int sliderX=70, sliderX2=600;
 slider_row.setScale(slider_size,slider_size);
 slider_column.setScale(slider_size,slider_size);
 slider_bombs.setScale(slider_size,slider_size);
 slider_row.setPosition(sliderX,240);
 slider_column.setPosition(sliderX,352);
 slider_bombs.setPosition(sliderX,469);

 // create font
  sf::Font font;
  font.loadFromFile(path + "OpenSans-Regular.ttf");
  sf::Text game_over("Game Over!", font);
  sf::Text you_win("You Win!", font);
  sf::Text text_row("5",font);
  sf::Text text_column("5",font);
  sf::Text text_bombs("5",font);
  text_row.setCharacterSize(40);
  text_column.setCharacterSize(40);
  text_bombs.setCharacterSize(40);
  you_win.setCharacterSize(100);
  game_over.setCharacterSize(100);
  text_row.setFillColor(sf::Color::Black);
  text_column.setFillColor(sf::Color::Black);
  text_bombs.setFillColor(sf::Color::Black);
  you_win.setFillColor(sf::Color::Blue);
  game_over.setFillColor(sf::Color::Red);
  text_row.setStyle(sf::Text::Bold);
  text_column.setStyle(sf::Text::Bold);
  text_bombs.setStyle(sf::Text::Bold);
  you_win.setStyle(sf::Text::Bold);
  game_over.setStyle(sf::Text::Bold);
  text_row.setPosition(675,223);
  text_column.setPosition(675,339);
  text_bombs.setPosition(675,457);
  you_win.setPosition(130,250);
  game_over.setPosition(150,250);

  // initialize board characteristics
  int cursor_state = 0, flag_state = 0, game_state = 0;
 int rows, columns, number_of_bombs, squares_left;
 int start[]={-1,0,-1,1,0,1,1,1,1,0,1,-1,0,-1,-1,-1};

 // start game play process while window is open and game has started.
 // Runs at 60 fps, so this loop runs 60 times per second.
 while (window.isOpen())
 {

   sf::Event event;

   while (window.pollEvent(event))
   {
     if (event.type == sf::Event::Closed) window.close();
   }
   window.clear();

   backdrop.setTexture(backdrop_texture);
   if(phase==2||phase==1)
     backdrop.setTexture(backdrop1_texture);
   if(phase==3)
     backdrop.setTexture(backdrop2_texture);
   window.draw(backdrop);

   sf::Vector2i localPosition = sf::Mouse::getPosition(window);
   cursor.setPosition(localPosition.x,localPosition.y);
   sf::Vector2f cursor_pos = cursor.getPosition();

   //beginning phase of the program for user to click on and select difficulty
   if(phase==0)
   {
     button_easy.setTexture(button_easy_texture);
     button_medium.setTexture(button_medium_texture);
     button_expert.setTexture(button_expert_texture);
     button_custom.setTexture(button_custom_texture);
     button_easy.setColor(sf::Color::White);
     button_medium.setColor(sf::Color::White);
     button_expert.setColor(sf::Color::White);
     button_custom.setColor(sf::Color::White);

     cursor.setTexture(cursor2_texture);
     cursor.setTextureRect(cursor2_rect);
     sf::Vector2f button_easy_pos = button_easy.getPosition();
     sf::Vector2f button_easy_scale = button_easy.getScale();
     sf::Vector2f button_medium_pos = button_medium.getPosition();
     sf::Vector2f button_medium_scale = button_medium.getScale();
     sf::Vector2f button_expert_pos = button_expert.getPosition();
     sf::Vector2f button_expert_scale = button_expert.getScale();
     sf::Vector2f button_custom_pos = button_custom.getPosition();
     sf::Vector2f button_custom_scale = button_custom.getScale();

     if(cursor_pos.x>=button_easy_pos.x&&cursor_pos.y>=button_easy_pos.y&&cursor_pos.x<=(button_easy_pos.x+button_easy_scale.x*203)&&cursor_pos.y<=(button_easy_pos.y+button_easy_scale.y*203))
     {
       button_easy.setColor(sf::Color(200,255,200));
       if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
         cursor_state = 1;
         button_easy.setTexture(button_easy1_texture);
       }
       //puts minesweeper in easy difficulty mode
       else if(cursor_state==1)
       {
         rows = 9;
         columns = 9;
         number_of_bombs = 10;

         phase = 1;
       }
       else
       {
         cursor_state = 0;
       }
     }

     else if(cursor_pos.x>=button_medium_pos.x&&cursor_pos.y>=button_medium_pos.y&&cursor_pos.x<=(button_medium_pos.x+button_medium_scale.x*203)&&cursor_pos.y<=(button_medium_pos.y+button_medium_scale.y*203))
     {
       button_medium.setColor(sf::Color::Yellow);
       if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
         cursor_state = 1;
         button_medium.setTexture(button_medium1_texture);
       }
       //puts minesweeper in medium difficulty mode
       else if(cursor_state==1)
       {
         rows = 16;
         columns = 16;
         number_of_bombs = 40;
         phase = 1;
       }
       else{
         cursor_state = 0;
       }
     }

     else if(cursor_pos.x>=button_expert_pos.x&&cursor_pos.y>=button_expert_pos.y&&cursor_pos.x<=(button_expert_pos.x+button_expert_scale.x*203)&&cursor_pos.y<=(button_expert_pos.y+button_expert_scale.y*203))
     {
       button_expert.setColor(sf::Color(255,165,0));

       if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
         cursor_state = 1;
         button_expert.setTexture(button_expert1_texture);
       }
       //puts minesweeper in expert difficulty mode
       else if(cursor_state==1)
       {
         rows = 16;
         columns = 30;
         number_of_bombs = 100;
         phase = 1;
       }

       else
       {
         cursor_state = 0;
       }
     }

     else if(cursor_pos.x>=button_custom_pos.x&&cursor_pos.y>=button_custom_pos.y&&cursor_pos.x<=(button_custom_pos.x+button_custom_scale.x*203)&&cursor_pos.y<=(button_custom_pos.y+button_custom_scale.y*203))
     {
       button_custom.setColor(sf::Color(128,0,128));
       if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
         cursor_state = 1;
         button_custom.setTexture(button_custom1_texture);
       }
       else if(cursor_state==1)
       {
         //puts minesweeper in custom difficulty mode
         cursor_state=0;
         game_state = 0;
         phase = 3;
       }
       else
       {
         cursor_state = 0;
       }
     }

     //otherwise the mouse input does nothing
     else
     {
       cursor.setTexture(cursor1_texture);
       cursor.setTextureRect(cursor1_rect);
       cursor_state = 0;
     }

     //the window shows the 4 buttons: easy, medium, expert, and custom
     window.draw(button_easy);
     window.draw(button_medium);
     window.draw(button_expert);
     window.draw(button_custom);
   }

   //difficulty is now selected and program continues
   else if(phase==1)
   {
     game_state = 0;

     //starting game board dimensions are initialized; board is created
     int min_x = 70;
     int max_x = 790;
     int min_y = 20;
     int max_y = 600;
     int space_between_mines = 5;

     sf::Vector2f scale, square_position;
     square_position.x=(max_x-min_x)/columns;
     square_position.y=(max_y-min_y)/rows;
     scale.x = (((float)(max_x-min_x))/(float)(columns))/((float)(67+space_between_mines));
     scale.y = (((float)(max_y-min_y))/(float)(rows))/((float)(67+space_between_mines));
     scale.x = min(scale.x,scale.y);
     scale.y = min(scale.x,scale.y);

     square_position.x=min(square_position.x,square_position.y);
     square_position.y=min(square_position.x,square_position.y);
     initial_square.minesquare.setScale(scale);
     vector<square> board_row;

     while(!board_row.empty())
     {
        board_row.pop_back();
     }

     for(int i=0;i<columns;i++)
       board_row.push_back(initial_square);
     for(int i=0;i<rows;i++)
     {
       board.push_back(board_row);
       for(int j=0;j<columns;j++)
       {
         board[i][j].row = i;
         board[i][j].column = j;
         board[i][j].minesquare.setPosition(min_x+j*square_position.x, min_y+i*square_position.y);
       }
     }
     hasBombs = false;
     phase = 2;
     cursor_state=0;
   }

   // custom difficulty menu
   else if(phase==3)
   {
     sf::Vector2f slider_row_pos = slider_row.getPosition();
     sf::Vector2f slider_column_pos = slider_column.getPosition();
     sf::Vector2f slider_bombs_pos = slider_bombs.getPosition();
     if(sf::Mouse::isButtonPressed(sf::Mouse::Left)&&cursor_state>0)
     {
        cursor.setTexture(cursor2_texture);
        cursor.setTextureRect(cursor2_rect);
        if(cursor_state==1)
        {
            slider_row.setPosition(cursor_pos.x-25,slider_row_pos.y);
            if(cursor_pos.x-25<sliderX)
            {
                slider_row.setPosition(sliderX,slider_row_pos.y);
            }
            if(cursor_pos.x-25>sliderX2)
            {
                slider_row.setPosition(sliderX2,slider_row_pos.y);
            }
        }
        else if(cursor_state==2)
        {
            slider_column.setPosition(cursor_pos.x-25,slider_column_pos.y);
            if(cursor_pos.x-25<sliderX)
            {
                slider_column.setPosition(sliderX,slider_column_pos.y);
            }
            if(cursor_pos.x-25>sliderX2)
            {
                slider_column.setPosition(sliderX2,slider_column_pos.y);
            }
        }
        else if(cursor_state==3)
        {
            slider_bombs.setPosition(cursor_pos.x-25,slider_bombs_pos.y);
            if(cursor_pos.x-25<sliderX)
            {
                slider_bombs.setPosition(sliderX,slider_bombs_pos.y);
            }
            if(cursor_pos.x-25>sliderX2)
            {
                slider_bombs.setPosition(sliderX2,slider_bombs_pos.y);
            }
        }
     }
     slider_row_pos = slider_row.getPosition();
     rows = (95 * (slider_row_pos.x-sliderX) / (sliderX2-sliderX)) +5;
     slider_column_pos = slider_column.getPosition();
     columns = (95 * (slider_column_pos.x-sliderX) / (sliderX2-sliderX)) +5;
     slider_bombs_pos = slider_bombs.getPosition();
     number_of_bombs = (rows * columns / 5 -rows * columns / 20) * (slider_bombs_pos.x-sliderX) / (sliderX2-sliderX)+(rows * columns / 20);
     text_row.setString(to_string(rows));
     text_column.setString(to_string(columns));
     text_bombs.setString(to_string(number_of_bombs));
     if(cursor_state==0)
     {
        if(cursor_pos.x>=slider_row_pos.x && cursor_pos.y>=slider_row_pos.y && cursor_pos.x<=(slider_row_pos.x+slider_size*102) && cursor_pos.y<=(slider_row_pos.y+slider_size*59))
        {
            cursor.setTexture(cursor2_texture);
            cursor.setTextureRect(cursor2_rect);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                cursor_state=1;
        }
        else if(cursor_pos.x>=slider_column_pos.x && cursor_pos.y>=slider_column_pos.y && cursor_pos.x<=(slider_column_pos.x+slider_size*102) && cursor_pos.y<=(slider_column_pos.y+slider_size*59))
        {
            cursor.setTexture(cursor2_texture);
            cursor.setTextureRect(cursor2_rect);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                cursor_state=2;
        }
        else if(cursor_pos.x>=slider_bombs_pos.x && cursor_pos.y>=slider_bombs_pos.y && cursor_pos.x<=(slider_bombs_pos.x+slider_size*102) && cursor_pos.y<=(slider_bombs_pos.y+slider_size*59))
        {
            cursor.setTexture(cursor2_texture);
            cursor.setTextureRect(cursor2_rect);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                cursor_state=3;
        }
        else
        {
            cursor.setTexture(cursor1_texture);
            cursor.setTextureRect(cursor1_rect);
        }
     }
     if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
     {
        cursor_state=0;
     }
     if(cursor_state>0)
     {
        cursor.setTexture(cursor2_texture);
        cursor.setTextureRect(cursor2_rect);
     }
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
     {
        phase=1;
     }
     window.draw(slider_row);
     window.draw(slider_column);
     window.draw(slider_bombs);
     window.draw(text_row);
     window.draw(text_column);
     window.draw(text_bombs);
   }

   // board is drawn
   if(phase==2)
   {
     cursor.setTexture(cursor1_texture);
     cursor.setTextureRect(cursor1_rect);

     // ensures that if adjacent squares are empty they are cleared
     while(!q.empty())
     {
       square cur=q.front();
       q.pop();
       if(cur.row<0) continue;
       if(cur.row>=rows) continue;
       if(cur.column<0) continue;
       if(cur.column>=columns) continue;
       if(board[cur.row][cur.column].isVisible) continue;
       board[cur.row][cur.column].isVisible = true;
       if(board[cur.row][cur.column].value==0)
       {
         next = cur;
         for(int i=0; i<15; i+=2){
           next.row = cur.row+start[i];
           next.column = cur.column+start[i+1];
           if(next.row>=0 && next.row<rows && next.column>=0 && next.column<columns)
             q2.push(next);
         }
       }
     }

     // updates the second queue
     while(!q2.empty())
     {
       q.push(q2.front());
       q2.pop();
     }

     // checks if you win
     squares_left = 0;
     for(int i=0; i<rows; i++)
        for(int j=0;j<columns;j++)
            if(!board[i][j].isVisible) squares_left++;

     if(squares_left<=number_of_bombs&&game_state==0)
     {
        game_state = 2;
        for(int i=0; i<rows; i++)
            for(int j=0; j<columns; j++)
                if(board[i][j].value==-1)
                    board[i][j].isVisible = true;
     }

     // draws back button
     sf::Vector2f back_button_pos = back_button.getPosition();
     sf::Vector2f back_button_scale = back_button.getScale();
     if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && cursor_state==-1)
     {
        back_button.setColor(sf::Color::Green);
        if(cursor_pos.x>=back_button_pos.x && cursor_pos.y>=back_button_pos.y && cursor_pos.x<=(back_button_pos.x+back_button_scale.x*75) && cursor_pos.y<=(back_button_pos.y+back_button_scale.y*71))
        {
            cursor.setTexture(cursor2_texture);
            cursor.setTextureRect(cursor2_rect);
        }
        else
        {
            cursor.setTexture(cursor1_texture);
            cursor.setTextureRect(cursor1_rect);
        }
     }
     else if(cursor_state==-1)
     {
         cursor_state=0;
         if(cursor_pos.x>=back_button_pos.x && cursor_pos.y>=back_button_pos.y && cursor_pos.x<=(back_button_pos.x+back_button_scale.x*75) && cursor_pos.y<=(back_button_pos.y+back_button_scale.y*71))
         {
            phase = 0;
            game_state = 0;
            hasBombs = false;
            rows = 0;
            columns = 0;
            number_of_bombs = 0;
            while(!board.empty())
                board.pop_back();
            continue;
         }
     }
     else if(cursor_pos.x>=back_button_pos.x && cursor_pos.y>=back_button_pos.y && cursor_pos.x<=(back_button_pos.x+back_button_scale.x*75) && cursor_pos.y<=(back_button_pos.y+back_button_scale.y*71))
     {
         cursor.setTexture(cursor2_texture);
         cursor.setTextureRect(cursor2_rect);
         back_button.setColor(sf::Color(255,220,255));
         if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
         {
            if(cursor_state==0)
            {
                cursor_state = -1;
                back_button.setColor(sf::Color::Green);
            }
         }
     }
     else
     {
        back_button.setColor(sf::Color::White);
     }

     //draws basic gameboard layout
     for(int i=0; i<rows; i++)
     {
       for(int j=0; j<columns; j++)
       {
         sf::Vector2f minesquare_pos = board[i][j].minesquare.getPosition();
         sf::Vector2f minesquare_scale = board[i][j].minesquare.getScale();
         if(!board[i][j].isVisible)
         {
           board[i][j].minesquare.setColor(sf::Color::White);
           board[i][j].minesquare.setTexture(minesquare_texture);
           if(board[i][j].flag)
            board[i][j].minesquare.setTexture(mineflag_texture);
         }
         //board numbers appear on the game board
         if(board[i][j].isVisible)
         {
           board[i][j].minesquare.setColor(sf::Color::White);
           if(board[i][j].value==0)
            board[i][j].minesquare.setTexture(mine0_texture);
           else if(board[i][j].value==1)
            board[i][j].minesquare.setTexture(mine1_texture);
           else if(board[i][j].value==2)
            board[i][j].minesquare.setTexture(mine2_texture);
           else if(board[i][j].value==3)
            board[i][j].minesquare.setTexture(mine3_texture);
           else if(board[i][j].value==4)
            board[i][j].minesquare.setTexture(mine4_texture);
           else if(board[i][j].value==5)
            board[i][j].minesquare.setTexture(mine5_texture);
           else if(board[i][j].value==6)
            board[i][j].minesquare.setTexture(mine6_texture);
           else if(board[i][j].value==7)
            board[i][j].minesquare.setTexture(mine7_texture);
           else if(board[i][j].value==8)
            board[i][j].minesquare.setTexture(mine8_texture);
           else if(board[i][j].value==-1)
            board[i][j].minesquare.setTexture(minebomb_texture);
           else
            board[i][j].minesquare.setTexture(mineboom_texture);
         }

         //initializes more graphics
         else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && cursor_state==i*columns+j+1)
         {
           board[i][j].minesquare.setTexture(minepressed_texture);
           if(cursor_pos.x>=minesquare_pos.x && cursor_pos.y>=minesquare_pos.y && cursor_pos.x<=(minesquare_pos.x+minesquare_scale.x*67) && cursor_pos.y<=(minesquare_pos.y+minesquare_scale.y*67))
           {
             cursor.setTexture(cursor2_texture);
             cursor.setTextureRect(cursor2_rect);
           }
           else
           {
             cursor.setTexture(cursor1_texture);
             cursor.setTextureRect(cursor1_rect);
           }
         }

         // when the mouse is not pressed on the first run through
         else if(cursor_state==i*columns+j+1)
         {
           if(cursor_pos.x>=minesquare_pos.x && cursor_pos.y>=minesquare_pos.y && cursor_pos.x<=(minesquare_pos.x+minesquare_scale.x*67) && cursor_pos.y<=(minesquare_pos.y+minesquare_scale.y*67))
           {

             //put bombs on the game board
             if(!hasBombs)
             {
                generateBombs(board, number_of_bombs, i, j);
                hasBombs = true;
             }
             if(board[i][j].value==-1)
             {
                    game_state = 1;
                    for(int k=0; k<rows; k++)
                    {
                        for(int l=0; l<columns; l++)
                        {
                            if(board[k][l].value==-1)
                            {
                                board[k][l].isVisible = true;
                                board[k][l].value = -2;
                            }
                        }
                    }
             }
             if(game_state==0) q.push(board[i][j]);
           }
           cursor_state=0;
         }

         // code that interacts with flags on board
         else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && flag_state==i*columns+j+1 && game_state==0)
         {
            if(board[i][j].flag)
                board[i][j].minesquare.setTexture(mineflagpressed_texture);
            else board[i][j].minesquare.setTexture(minepressed_texture);
            if(cursor_pos.x>=minesquare_pos.x && cursor_pos.y>=minesquare_pos.y && cursor_pos.x<=(minesquare_pos.x+minesquare_scale.x*67) && cursor_pos.y<=(minesquare_pos.y+minesquare_scale.y*67))
            {
              cursor.setTexture(cursor2_texture);
              cursor.setTextureRect(cursor2_rect);
            }
            else
            {
              cursor.setTexture(cursor1_texture);
              cursor.setTextureRect(cursor1_rect);
            }
         }

         // code that checks if flags need to be changed
         else if(flag_state==i*columns+j+1 && game_state==0)
         {
            if(cursor_pos.x>=minesquare_pos.x && cursor_pos.y>=minesquare_pos.y && cursor_pos.x<=(minesquare_pos.x+minesquare_scale.x*67) && cursor_pos.y<=(minesquare_pos.y+minesquare_scale.y*67))
            {
                if(board[i][j].flag)
                {
                    board[i][j].flag = false;
                    board[i][j].minesquare.setTexture(minesquare_texture);
                }
                else
                {
                    board[i][j].flag = true;
                    board[i][j].minesquare.setTexture(mineflag_texture);
                }
                cursor.setTexture(cursor2_texture);
                cursor.setTextureRect(cursor2_rect);
            }
            flag_state=0;
         }

         //Actions that occur when regularly clicking on a space
         else if(cursor_pos.x>=minesquare_pos.x && cursor_pos.y>=minesquare_pos.y && cursor_pos.x<=(minesquare_pos.x+minesquare_scale.x*67) && cursor_pos.y<=(minesquare_pos.y+minesquare_scale.y*67))
         {
           if(game_state==0)
           {
                cursor.setTexture(cursor2_texture);
                cursor.setTextureRect(cursor2_rect);
                board[i][j].minesquare.setColor(sf::Color(255,255,210));
           }

           //put flag on left click
           if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && game_state==0)
           {
             if(cursor_state==0 && !board[i][j].flag)
             {
               cursor_state = i*columns+j+1;
               board[i][j].minesquare.setTexture(minepressed_texture);
             }
           }

           //open flag on right click
           else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && game_state==0)
           {
             if(flag_state==0)
             {
                flag_state = i*columns+j+1;
                if(board[i][j].flag)
                    board[i][j].minesquare.setTexture(mineflagpressed_texture);
                else
                    board[i][j].minesquare.setTexture(minepressed_texture);
             }
           }
         }

         //fills up the rest of the board with appropriate spaces
         else
         {
           board[i][j].minesquare.setColor(sf::Color::White);
           if(board[i][j].flag) board[i][j].minesquare.setTexture(mineflag_texture);
           else board[i][j].minesquare.setTexture(minesquare_texture);
         }

         //board is updated
         window.draw(board[i][j].minesquare);
       }
     }
     window.draw(back_button);
     if(game_state==1)
        window.draw(game_over);
     if(game_state==2)
        window.draw(you_win);
   }

   //these are fixed methods of the window
   window.setFramerateLimit(60);
   window.setMouseCursorVisible(false);
   window.draw(cursor);
   window.display();
 }

 return 0;
}
