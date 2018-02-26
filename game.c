#include<stdio.h>
#include <stdbool.h>
#include<unistd.h>
#include <stdio_ext.h>
#include <time.h>
#include <stdlib.h>

void gotoxy(int x,int y)
{
  printf("%c[%d;%df",0x1B,y,x);
  fflush(stdout);
}

int notover,cur_ang,cur_pos_x,cur_pos_y;
int points = 0;
int steps = 0;
char input[4][4];
bool gold_grabbed = false;
bool cant_shoot = false;
int arrow_used = 0;
struct Hunter_map{
  bool stench;
  bool breez;
  bool wall;
  bool glitter;
  bool scream;
  bool visited;
  bool gold;
  bool pit;
  bool whumpus;
}map[4][4];

//DISPLAY

void display(int px,int py,int angle,bool cant_move)
{ system("tput clear");
  int count = 4;
  gotoxy((py+1)*10+6,(px)*8+3);
    if(angle == 0)
     printf(">" );
    if(angle == 1)
      printf("^" );
    if(angle == 2)
      printf("<" );
    if(angle == 3)
      printf("!" );

  for (int i = 0; i < count; i++)
  {
        gotoxy(10,i*8);
        if(i)
        printf("-----------------------------------------");
    for (int j = 0; j < count  ; j++)
     {
       gotoxy((j+1)*10,i*8+1);
        printf("|");

        if(map[i][j].stench && map[i][j].visited)
        {
          printf("S");
        }
        gotoxy((j+1)*10,i*8+2);
        printf("|");
        if(map[i][j].breez&& map[i][j].visited)
        {

          printf("B");
        }
        gotoxy((j+1)*10,i*8+3);
        printf("|");
        if(map[i][j].glitter&& map[i][j].visited)
        {

          printf("g");
        }
        gotoxy((j+1)*10,i*8+4);
        printf("|");
        if(map[i][j].scream&& map[i][j].visited)
        {

          printf("scr");
        }
        gotoxy((j+1)*10,i*8+5);
        printf("|");
        gotoxy((j+1)*10,i*8+6);
        printf("|");


      if(map[i][j].wall&& map[i][j].visited)
        {

          printf("wall");
        }
        gotoxy((j+1)*10,i*8+7);
        printf("|");
        if(map[i][j].visited)
        {  gotoxy((j+1)*10+2,i*8+7);
          if(map[i][j].gold)
            printf("GOLD");
          if(map[i][j].whumpus)
          printf("WHUM");
          if(map[i][j].pit)
          printf("PIT");
        }
        if(!map[i][j].visited){
          gotoxy((j+1)*10+3,i*8+3);
          printf("?");
        }
        for ( int k = 0; k < 8; k++) {

        gotoxy((j+2)*10,i*8+k);
        printf("|");
      }
     }

  }

  // gotoxy(10,45);
   printf("\n         -----------------------------------------");
  if(map[cur_pos_x][cur_pos_y].whumpus)
  { gotoxy(60,9);
      printf("%s","---------------------------------------" );
    gotoxy(60,10);
           printf("You  are eaten by whumpus    Game Over:");
    gotoxy(60,11);
         printf("%s","---------------------------------------" );

  }

  if(gold_grabbed && cur_pos_x ==3 &&cur_pos_y == 0)
  {  gotoxy(60,9);
      printf("%s","-----------------------------------" );
      gotoxy(60,10);
      printf("WELLDONE YOU COMPLETED THE GAME:::");
      gotoxy(60,11);
      printf("%s","-----------------------------------" );

  }


  if(map[cur_pos_x][cur_pos_y].pit)
  { gotoxy(60,9);
      printf("%s","----------------------------" );

    gotoxy(60,10);
           printf("You fell in a pit Game Over:");
   gotoxy(60,11);
       printf("%s","----------------------------" );

  }
  if(cant_move)
  {
    gotoxy(55,9);
  printf("%s","----------------------------------------------" );

    gotoxy(55,10);
       printf("CANT MAKE THIS MOVE :: PLEASE MAKE CORRECT ONE");
    gotoxy(55,11);
       printf("%s","----------------------------------------------" );

  }
  if(cant_shoot)
  {
    gotoxy(70,10);
   printf("You killed the whumpus .. GREAT::");
  }
  if(arrow_used > 2)
  {
    gotoxy(70,20);
   printf(" maximum arrows used.. ::");
  }
  gotoxy(100,17);
  printf("%s","GAME SCORES" );
  gotoxy(100,18);
  printf("%s","-----------------" );

  gotoxy(100,21);
  printf("POINTS:%d",points );
  gotoxy(100,23);
  printf("Steps:%d",steps);
    gotoxy(100,5);
    printf("%s","GAME CONTROLS" );
      gotoxy(98,6);
      printf("%s","-----------------" );
  gotoxy(100,7);
  printf("w:MOVE FORWARD:");
  gotoxy(100,9);
  printf("a:MOVE LEFT");
  gotoxy(100,11);
  printf("d:MOVE RIGHT");
  gotoxy(100,13);
  printf("s:SHOOT");
  gotoxy(100,15);
  printf("g:GRAB THE GOLD");
  gotoxy(80,25);
  printf("%s","NEXT ACTION::" );
}



void initial()
{
 for (int i = 0; i < 4; i++) {
   for (int j = 0; j < 4; j++) {
    map[i][j].stench=false;
    map[i][j].breez= false;
    map[i][j].wall = false;
    map[i][j].glitter = false;
    map[i][j].scream = false;
    map[i][j].visited = false;
    map[i][j].gold = false;
    map[i][j].pit = false;
    map[i][j].whumpus = false;
   }
  }
  cur_ang = 0;
  cur_pos_x = 3;
  cur_pos_y = 0;
  notover = 1;
}

char generate()
{
  printf("enter the things to be placed in order\nW --> whumpus\nP-->pit \nG -->gold and Treasure");
   for(int i =0;i<4;i++)
    for(int j=0;j<4;j++)
    {
      int r;
        r = (rand())%6;
       char ch;
      if(r == 0)
         ch = 'g';
      else if(r == 1)
              ch = 'w';
      else if(r == 4)
              ch = 'p';
      else
          ch = ' ';
     if(ch!=' ' && i==3 && j==0)
        ch =' ';
    input[i][j] = ch;

    }

}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  char ch;
  int ch1;
  int x,y;
  bool breakz = false;
  bool cant_move = false;
  initial();
  // printf("%d\n", notover);

  //generate
  generate();
  //READD
  for(int i =0;i<4;i++)
     for(int j=0;j<4;j++)
  {
    if(input[i][j] == 'w')
      { map[i][j].whumpus =true;
         if(i != 0)
          {
           map[i-1][j].stench = true;
           }
          if(j !=0 )
             map[i][j-1].stench=1;
         if(i !=3)
      map[i+1][j].stench = 1;
    if(j != 3)
      map[i][j+1].stench = 1;

     }
     if(input[i][j] == 'p')
       {
         map[i][j].pit =true;
          if(i != 0)
           {
            map[i-1][j].breez = 1;
            }
           if(j !=0 )
              map[i][j-1].breez=1;
          if(i !=3)
       map[i+1][j].breez = 1;
     if(j != 3)
       map[i][j+1].breez = 1;

      }
      if(input[i][j] == 'g')
        { map[i][j].gold =true;
          map[i][j].glitter = true;
      //      if(i != 0)
      //       {
      //        map[i-1][j].glitter = 1;
      //        }
      //       if(j !=0 )
      //          map[i][j-1].glitter=1;
      //      if(i !=3)
      //   map[i+1][j].glitter = 1;
      // if(j != 3)
      //   map[i][j+1].glitter = 1;

       }

   printf("%c\n",input[i][j]);

  }
  //OVER


  // scanf("%d",&ch1 );
  display(cur_pos_x,cur_pos_y-1,cur_ang,cant_move);
    while (notover)
    {
      cant_move =false;
      steps +=1;
      map[cur_pos_x][cur_pos_y].visited = true;
      __fpurge(stdin);
      scanf("%c",&ch);
        if (ch == 'd') {
        points -= 1;
       cur_ang -= 1;
      }

      if(ch == 'a'){
      points -= 1;
      cur_ang += 1;
      }
      cur_ang = (cur_ang+4) % 4;

      if (ch == 'w') {
      points -= 1;
      if(cur_ang == 0 )
      {
        // printf("cur_pos_x:%d\n",cur_pos_x+1 );
       if(cur_pos_y < 3)
         cur_pos_y += 1;
        else
          cant_move = true;
        }
      if(cur_ang == 1)
         if(cur_pos_x > 0)
            cur_pos_x -= 1;
            else
              cant_move = true;
      if(cur_ang == 2)
          if(cur_pos_y > 0)
             cur_pos_y -= 1;
             else
               cant_move = true;
      if(cur_ang == 3)
          if(cur_pos_x < 3)
             cur_pos_x += 1;
             else
               cant_move = true;
          map[cur_pos_x][cur_pos_y].visited = true;
    }

       if(ch == 'g')
      {
         points -= 1;
      if (map[cur_pos_x][cur_pos_y].gold) {
        gold_grabbed = true;
         map[cur_pos_x][cur_pos_y].gold = false;
         points += 1000;
       }
      }
      if(arrow_used < 3)
      if(ch == 's')
      { arrow_used += 1;
          x = cur_pos_x;
          y= cur_pos_y;

          if(cur_ang == 0 )
          {
            // printf("cur_pos_x:%d\n",cur_pos_x+1 );
           if(cur_pos_y < 3)
             y += 1;
            else
              cant_shoot = true;
            }
          if(cur_ang == 1)
             if(cur_pos_x > 0)
                x -= 1;
                else
                  cant_shoot = true;
          if(cur_ang == 2)
              if(cur_pos_y > 0)
                 y -= 1;
                 else
                   cant_shoot = true;
          if(cur_ang == 3)
              if(cur_pos_x < 3)
                 x += 1;
              else
              cant_shoot =true;
       if(!cant_shoot)
            points -= 10;
      if(map[x][y].whumpus)
      {
      map[cur_pos_x][cur_pos_y].scream =true;
     map[x][y].whumpus = false;
       }
      else
      map[cur_pos_x][cur_pos_y].wall = true;
     }

      if(ch == 'q')
      {
        breakz = true;
      }
      //death considering
      if(map[cur_pos_x][cur_pos_y].pit)
      { points -= 1000;

       breakz = true;
      }

      if(map[cur_pos_x][cur_pos_y].whumpus)
      { points -= 1000;

      breakz = true;
      }

      if(gold_grabbed && cur_pos_x ==3 &&cur_pos_y == 0)
      {  breakz =true;
      }
      display(cur_pos_x,cur_pos_y,cur_ang,cant_move);
      if(breakz)
        break;
  }
  gotoxy(10,35);
  printf("TOtal Steps:%d",steps);
  // gotoxy(10,40)
  printf("\n         Points earned:%d", points);
  if(points < 0)
  {
    printf("\n         You loose");
  }
  else
   printf("\n         You WIN");
  printf("\n");
    return 0;
}
