#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 

 void greetings(char*);
 void prepare_battlefield();
 void battlefield(int, char*);
 void spawn_battlefield(int, char*);
 void spawn(int);
 void action();
 int moove(int*, int*, int, int, int);
 void bullet_check(int);
 int search(int*);
 
 void gameover();
 void refresh_scr();
 
 int warfare(int);
 
 char field[24][80], score[6] = {'0','0','0','0','0','0'};
 int bullets[23];
 enum replacements {warrior, hunter, hero, left = 97, up = 119, right = 100, down = 115, aleft = 228, aup = 230, aright = 162, adown = 235, shot = 32, bullet = 42, alien_bullet = 253};
 struct ship {int x, y, lives;} HERO = {11, 0, 7}, BADASS = {0, 0};
 struct enemy_stats {int alive, count, spawntime, kind, moove_speed;} enemy = {0, 0, 0, 0, 7};
 int enemy_lives[6] = {3, 3, 3, 3, 3, 3}, shoot_rate = 7, si = 7;
 
int main()
{  
  srand(time(NULL));
      
  char name[20]; 
  
  //greetings(name);   
  prepare_battlefield();
  spawn(hero); 
  while(HERO.lives)
  {         
   battlefield(HERO.lives, score); 
   
   if(enemy.alive == 0)
    {
       enemy.spawntime++;
       if(enemy.spawntime == 10)
       {
         enemy.kind = hunter;                          
         spawn(enemy.kind);
         enemy.alive = 1;
         enemy.spawntime = 0;
       }   
    }
    else  
    warfare(enemy.kind);             
  }
  gameover();
  
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void refresh_scr()
    {
	    int i, timer;    
	    system("cls");
	    printf("%s", *field);
    
    i = clock();
    do timer = clock(); while(timer-i < 10);	 
    }
    
    
    void gameover()
    {
      FILE* p;
      char app[4];
      int out, i, count = 0, x = 7;
      
      if((p = fopen("gamedata.txt", "a+")) == NULL)
      printf("Gamedata is not found.");   
      else
      {
         do
         {
           for(i = 0; i < 4; i++)
           app[i] = 0;
           
           for(i = 0; i < 4; i++)
           {
   		       app[i] = fgetc(p);
               if(app[i] == ' ' || app[i] == EOF)
               break;
           }   
           out = atoi(app);
           count++; 
           if(out != 13)
		   field[x][30+count] = out;
		   else
		   {
		   x++;
		   count = 0;
		   }
     	   refresh_scr();    
            
         }
         while(app[i] != EOF); 
      }
      getch();
    }                                                                            

















    int warfare(int kind)
    {
            static int x, y, mi, si, search_res, stage;
            
                     if(search_res == 1)
                    {
            		  if(stage == 0)
            		  {
            			  if(si%2 == 0 && enemy_lives[si/2] != 0)
            	          {
            	              field[BADASS.x+si/2][BADASS.y-2*si/2-2] = alien_bullet;
            	                  
            	           	  if(bullets[BADASS.x+si/2]==1) bullets[BADASS.x+si/2] = 3; 
            	           	  else
            	              if(bullets[BADASS.x+si/2]==0) bullets[BADASS.x+si/2] = 2;
            	              if(si == 10) stage = 1;
            	          }  
            	          si++;
            		  }
                      else
                      {
            	          if(mi % enemy.moove_speed == 0)
            	          {
            	              y = -1;              
            	              if(mi / enemy.moove_speed == 5)
            	              {si = 0; search_res = 0; mi = enemy.moove_speed-1; stage = 0;}  
            	          }      
            	          mi++;   
            		  } 
                    }
                    else
                    search_res = search(&x);
                    
                    if(moove(&BADASS.x, &BADASS.y, x, y, enemy.kind) == 1)
                    {
                        int i;
                        enemy.alive = 0;
                        for(i = 0; i < 6; i++)
                        enemy_lives[i] = 3;
                        mi = 0, si = 0, search_res = 0, stage = 0;
                    }
                    x = y = 0;     
    }                                                                                                                                                       





    void battlefield(int lives, char* score)
    {    
    int i, timer;    
    for(i = 0; i < HERO.lives; i++)
    field[0][6+i] = 3;  
    for(; i < 62; i++)
    field[0][6+i] = ' ';
    for(i = 0; i < 6; i++)   
    field[0][i+74] = *(score+i);   
	 
    if(si != shoot_rate)
    si++;
    action();
    
    for(i = 1; i < 24; i++) 
     if(bullets[i] != 0) 
     bullet_check(i);
     
          
    system("cls");
    printf("%s", *field);
    
    i = clock();
    do timer = clock(); while(timer-i < 60);
}



void action()
{
     if (kbhit() == 1)
     {    
        int dir = getch();
        
        if(dir == left || dir == aleft)
        moove(&HERO.x, &HERO.y, 0, -1, hero);
        else
        if(dir == up || dir == aup)
        moove(&HERO.x, &HERO.y, -1, 0, hero);
        else
        if(dir == right || dir == aright)
        moove(&HERO.x, &HERO.y, 0, 1, hero);
        else
        if(dir == down|| dir == adown)
        moove(&HERO.x, &HERO.y, 1, 0, hero); 
        else
        moove(&HERO.x, &HERO.y, 0, 0, hero);
 
        if(dir == shot)
        {
          if(si == shoot_rate)
          {
          field[HERO.x+1][HERO.y+1] = bullet;
          field[HERO.x-1][HERO.y+1] = bullet;
          
          if(bullets[HERO.x+1] == 2) bullets[HERO.x+1] = 3; 
          else
          if(bullets[HERO.x+1] != 3) bullets[HERO.x+1] = 1; 
          
          if(bullets[HERO.x-1] == 2) bullets[HERO.x-1] = 3; 
          else
          if(bullets[HERO.x-1] != 3) bullets[HERO.x-1] = 1;
          
          si = 0;    
          } 
        }
     }
     else
 	 moove(&HERO.x, &HERO.y, 0, 0, hero);
}

int moove(int* X, int* Y, int x, int y, int kind)
{  
   switch (kind)
   {
    case hero:
    {  
          if(x != 0) switch(x)
          {
            case -1:  if(*X == 2) x = 0; break;
            case 1:   if(*X == 22) x = 0; break;
          }
          if(y != 0) switch(y)
          {
            case -1:  if(*Y == 0) y = 0; break;
            case 1:   if(*Y == 78) y = 0; break;
          }
          
          //Нос                 
		  if(field[*X][*Y+1] == 16)
          field[*X][*Y+1] = ' ';
		  else
		  {
          printf("\a");
		  HERO.lives--; 
          }
          //Хвост
          field[*X][*Y] = ' ';
          if(field[*X+1][*Y] == -64)
          field[*X+1][*Y] = ' ';
          else
          {
          printf("\a");
		  HERO.lives--; 
          }
		  
          if(field[*X-1][*Y] == -38)
          field[*X-1][*Y] = ' ';
          else
          {
          printf("\a");
		  HERO.lives--;
          } 
   
   
          field[*X+x][*Y+y+1] = 16;       
          field[*X+x][*Y+y] = 178;
          field[*X+x+1][*Y+y] = 192;
          field[*X+x-1][*Y+y] = 218;
   } break;
   
   case hunter:
    {     
    int i, alive = 0;
          if(*Y > 12)
          {               
            for(i = 0; i <= 5;i++)
            {
                  if(enemy_lives[i] != 0) 
                  {
                    if(field[*X+i][*Y-2*i] != -51 || field[*X+i][*Y-2*i-1] != 17) 
                    enemy_lives[i]--;
                    
                    field[*X+i][*Y-2*i-1] = ' ';
                    field[*X+i][*Y-2*i] = ' ';
                  }
            }
            
            for(i = 0; i <= 5;i++)
            {
                  if(enemy_lives[i] != 0)
                  {
                    if(field[*X+i+x][*Y-2*i+y] == bullet || field[*X+i+x][*Y-2*i-1+y] == bullet) 
                    enemy_lives[i]--;
                    
                    if(enemy_lives[i]!=0)
                    {
                    field[*X+i+x][*Y-2*i-1+y] = 17;
                    field[*X+i+x][*Y-2*i+y] = 205;
                    alive = 1;
                    }
                    else
                    {
                    field[*X+i+x][*Y-2*i-1+y] = ' ';
                    field[*X+i+x][*Y-2*i+y] = ' ';    
                    }
                  }
            }
          } 
          else
          {
            for(i = 0; i <= 5;i++)
            {
                  if(enemy_lives[i] != 0) 
                  {
                    if(field[*X+i][*Y-2*i] != -51 || field[*X+i][*Y-2*i-1] != 17) 
                    enemy_lives[i]--;
                    
                    
                    if(enemy_lives[i]!=0)
                    {
	                    if(*Y-2*i-1 >= 0)       
	                    field[*X+i][*Y-2*i-1] = ' ';
	                    field[*X+i][*Y-2*i] = ' ';
	                    if(*Y-2*i == 0)
	                    enemy_lives[i] = 0;
	                    else
	                    alive = 1;
					}
                  }
            }
            
            for(i = 0; i <= 5;i++)
            {
                  if(enemy_lives[i] != 0)
                  {
                    if(field[*X+i+x][*Y-2*i+y-1] == bullet || field[*X+i+x][*Y-2*i-1+y-1] == bullet) 
                    enemy_lives[i]--;
                    
                    if(enemy_lives[i]!=0)
                    {
                    if(*Y-2*i-1+y >= 0)
                    field[*X+i+x][*Y-2*i-1+y] = 17;
                    field[*X+i+x][*Y-2*i+y] = 205;
                    alive = 1;
                    }
                    else
                    {
                    field[*X+i+x][*Y-2*i-1+y] = ' ';
                    field[*X+i+x][*Y-2*i+y] = ' ';    
                    }
                  }
            }  
          }
              
           if(alive == 0)
            return 1;
                   
   } break; 
   case warrior:
    {     
          int i;
          
           if(BADASS.y >= HERO.y)
           if(BADASS.x+4 > HERO.x) x = -1;
           else
           if(BADASS.x+4 < HERO.x) x = 1;
        
        
                     if(x != 0) switch(x)
                                {
                                case -1:  if(*X == 2) x = 0; break;
                                case 1:   if(*X == 20) x = 0; break;
                                }
                              
                     if(y == 1) if(*Y == 79) y = 0;
          if(*Y > 2)
          {  
              for(i = 0; i <= 7; i += 7)
              {                          
                  //Хвост.Чистка
                  field[*X-2+i][*Y] = ' ';
                  field[*X-1+i][*Y] = ' ';
                  field[*X+i][*Y] = ' ';
                  field[*X+1+i][*Y] = ' ';
                  field[*X+2+i][*Y] = ' ';
                  //Центр.Чистка
                  field[*X-1+i][*Y-1] = ' ';
                  field[*X+i][*Y-1] = ' ';
                  field[*X+1+i][*Y-1] = ' ';
                  //Нос.Чистка       
                  field[*X+i][*Y-2] = ' ';
                  
                  //Хвост.Рисовка
                  field[*X-2+x+i][*Y+y] = 187;
                  field[*X-1+x+i][*Y+y] = 178;
                  field[*X+x+i][*Y+y] = 178;
                  field[*X+1+x+i][*Y+y] = 178;
                  field[*X+2+x+i][*Y+y] = 188;
                  //Центр.Рисовка
                  field[*X-1+x+i][*Y-1+y] = 196;
                  field[*X+x+i][*Y-1+y] = 178;
                  field[*X+1+x+i][*Y-1+y] = 196;
                  //Нос.Рисовка       
                  field[*X+x+i][*Y-2+y] = 17;
              } 
          }
          else
          {
              for(i = 0; i <= 7; i += 7)
              {                          
                  if(*Y == 2)
                  {
                  //Хвост.Чистка
                  field[*X-2+i][*Y] = ' ';
                  field[*X-1+i][*Y] = ' ';
                  field[*X+i][*Y] = ' ';
                  field[*X+1+i][*Y] = ' ';
                  field[*X+2+i][*Y] = ' ';
                  }
                  if(*Y == 1)
                  {
                  //Центр.Чистка
                  field[*X-1+i][*Y-1] = ' ';
                  field[*X+i][*Y-1] = ' ';
                  field[*X+1+i][*Y-1] = ' ';
                  }
                  if(*Y == 0)
                  //Нос.Чистка       
                  field[*X+i][*Y-2] = ' ';
                  
                  if(*Y == 2)
                  {
                  //Хвост.Рисовка
                  field[*X-2+x+i][*Y+y] = 187;
                  field[*X-1+x+i][*Y+y] = 178;
                  field[*X+x+i][*Y+y] = 178;
                  field[*X+1+x+i][*Y+y] = 178;
                  field[*X+2+x+i][*Y+y] = 188;
                  }
                  if(*Y == 1)
                  {
                  //Центр.Рисовка
                  field[*X-1+x+i][*Y-1+y] = 196;
                  field[*X+x+i][*Y-1+y] = 178;
                  field[*X+1+x+i][*Y-1+y] = 196;
                  }
                  if(*Y == 0)
                  //Нос.Рисовка       
                  field[*X+x+i][*Y-2+y] = 17;
              } 
          }       
             
   } break;
} 
   *X += x;   
   *Y += y;
   return 0;
}

void bullet_check(int i)
{
         int c, j;
         switch(bullets[i])
         {
case 1:{
             for(j = 0, c = 0; j < 80; j++)
             {  
                if(field[i][j] == bullet)
                {
                  field[i][j] = ' ';
                  c++; 
                  if(j != 79)
                  { 
                    j++;
                    field[i][j] = bullet;    
                  }
                } 
             } 
             
             if(c == 0)
             bullets[i] = 0;
       } break;
       
case 2:
    {     
             for(j = 79, c = 0; j >= 0 ; j--)
             {  
                if(field[i][j] == -3)
                {
                  field[i][j] = ' ';
                  c++; 
                  if(j != 0)
                  { 
                    j--;
                    field[i][j] = alien_bullet;   
                  }
                } 
             } 
             
             if(c == 0)
             bullets[i] = 0;              
    } break; 
    
case 3:
    {     
          int ac;
             for(j = 0, c = 0; j < 80; j++)
             {  
                if(field[i][j] == bullet)
                {
                  field[i][j] = ' ';
                  c++; 
                  if(j != 79)
                  { 
                    j++;
                    if(field[i][j] != -3)
                    field[i][j] = bullet;
                    else  
                    field[i][j] = ' ';  
                  }
                } 
                
                if(field[i][j] == -3)
                {
                  field[i][j] = ' ';
                  ac++; 
                  if(j != 0)
                  { 
                    j--;
                    if(field[i][j] != bullet)
                    field[i][j] = alien_bullet;
                    else  
                    field[i][j] = ' ';   
                  }
                } 
             } 
             
             if(ac == 0 && c == 0)
             bullets[i] = 0;
             else
             {
             if(c == 0)
             bullets[i] = 2;
             if(ac == 0)
             bullets[i] = 1;
             }
             
             
    } break;
             
             
         }
}

int search(int* x)
{
            if(BADASS.x >= HERO.x) *x = -1;
            else
            if(BADASS.x+5 <= HERO.x) *x = 1;
            else
            return 1;  
}





void greetings(char* name)
{
      int i = 0, c;
      char variant;
      printf("What's your name, newbie?\nMy name is ");
      gets(name);
      printf("Ok, %s. Aliens again want stole all our grandmas and only YOU can save them!\n\nWill you do it?\n1) Yes!\n2) Of course!\n", name);
      variant = getch();
      if(variant != '1' && variant != '2')
      printf("%c) I dreamed about it whole my tired life!\n", variant);
      printf("Well, good luck %s!", name);  
 c = clock();
 do
 {
 i = clock() - c;
 }/////////////////////
 while(i < 3000);
}

void spawn(int ship)
{
   int i, j, c = 0;  
switch (ship)
{
   case warrior:
   {            
                  field[7][79] = 17;
                  field[14][79] = 17;
                  action();
                  battlefield(HERO.lives, score);
                  field[7][78] = 17;
                  field[14][78] = 17;
                  
                  field[6][79] = 196;
                  field[7][79] = 178;
                  field[8][79] = 196;
                  
                  field[13][79] = 196;
                  field[14][79] = 178;
                  field[15][79] = 196;
                  action();
                  battlefield(HERO.lives, score);
                  field[7][77] = 17;
                  field[14][77] = 17;
                  
                  field[6][78] = 196;
                  field[7][78] = 178;
                  field[8][78] = 196;
                  
                  field[13][78] = 196;
                  field[14][78] = 178;
                  field[15][78] = 196;
                  
                  field[5][79] = 187;
                  field[6][79] = 178;
                  field[7][79] = 178;
                  field[8][79] = 178;
                  field[9][79] = 188;
                  
                  field[12][79] = 187;
                  field[13][79] = 178;
                  field[14][79] = 178;
                  field[15][79] = 178;
                  field[16][79] = 188;
                  action();
                  battlefield(HERO.lives, score);
                                                    
                  for(i = 0; i <= 6;i++)
                  {
                  field[7][77-i] = 17;
                  field[14][77-i] = 17;
                  
                  field[6][78-i] = 196;
                  field[7][78-i] = 178;
                  field[8][78-i] = 196;
                  
                  field[13][78-i] = 196;
                  field[14][78-i] = 178;
                  field[15][78-i] = 196;
                  
                  field[5][79-i+1] = ' ';
                  field[6][79-i+1] = ' ';
                  field[7][79-i+1] = ' ';
                  field[8][79-i+1] = ' ';
                  field[9][79-i+1] = ' ';
                  
                  field[12][79-i+1] = ' ';
                  field[13][79-i+1] = ' ';
                  field[14][79-i+1] = ' ';
                  field[15][79-i+1] = ' ';
                  field[16][79-i+1] = ' ';
                  
                  field[5][79-i] = 187;
                  field[6][79-i] = 178;
                  field[7][79-i] = 178;
                  field[8][79-i] = 178;
                  field[9][79-i] = 188;
                  
                  field[12][79-i] = 187;
                  field[13][79-i] = 178;
                  field[14][79-i] = 178;
                  field[15][79-i] = 178;
                  field[16][79-i] = 188;
                  action();
                  battlefield(HERO.lives, score);
                  }
                  BADASS.x = 7; BADASS.y = 73;
   }
   break;
   
   case hunter:
   {            
                int c;
                  for(j = 0; j < 6;j++)
                  {
                     field[9][79] = 17;
                  	 battlefield(HERO.lives, score);
                     field[9][78] = 17;
                     field[9][79] = 205;
                  	 battlefield(HERO.lives, score);
                     for(i = 0; i <= 6-j;i++)
                     {
                     field[9+i][78-2*i] = ' ';
                     field[9+i][79-2*i] = ' ';
                     field[9+i+1][76-2*i] = 17;
                     field[9+i+1][77-2*i] = 205;
                     for(c = 6; c > 6-j; c--)
                     {
                     field[9+c+1][76-2*c] = 17;
                     field[9+c+1][77-2*c] = 205;
                     }
                  	 battlefield(HERO.lives, score);
                     } 
                  }
                  BADASS.x = 11; BADASS.y = 75;
   }
   break;
   
   case hero:
   {      
                  int i;      
                  
                  field[11][0] = 16;
                  spawn_battlefield(HERO.lives, score);
                   
                  field[11][1] = 16;
                  field[10][0] = 218;
                  field[11][0] = 178;
                  field[12][0] = 192;
                  spawn_battlefield(HERO.lives, score);
                  HERO.x = 11; HERO.y = 0;
                  
                  for(i = 0; i < 5;i++)
                  {    
                  moove(&HERO.x, &HERO.y, 0, 1, hero);
                  spawn_battlefield(HERO.lives, score);     
                  } 
   }
   break;
}
}


void prepare_battlefield()
{
 int i, j;
 char l[6]="Lives:", s[6]="Score:";
 system("cls");
  
 for(i = 0; i < 24; i++) 
 for(j = 0; j < 80; j++)  
 field[i][j] = ' ';
 
  for(i = 0; i < 6; i++)
 {
  field[0][i] = l[i];
  field[0][i+68] = s[i];      
 }
 
 for(i = 0; i < 23; i++)
 bullets[i] = 0;
}

void spawn_battlefield(int lives, char* score)
{
    int i, timer;    
    for(i = 0; i < HERO.lives; i++)
    field[0][6+i] = 3;  
    for(; i < 62; i++)
    field[0][6+i] = ' ';
    for(i = 0; i < 6; i++)   
    field[0][i+74] = *(score+i);    
          
    system("cls");
    printf("%s", *field);
    i = clock();
    do timer = clock(); while(timer-i < 500);    
}



















