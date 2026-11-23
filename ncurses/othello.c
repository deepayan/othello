#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UP    KEY_UP
#define DOWN  KEY_DOWN
#define LEFT  KEY_LEFT
#define RIGHT KEY_RIGHT

#define F20   'q'

#define PAIR_WHITE  1
#define PAIR_RED    2
#define PAIR_GRID   3
#define PAIR_CURSOR 4
#define PAIR_TITLE  5

struct table { char x[10][10]; };

/* Forward declarations for core functions defined later in the file */
int rValid(int m, int n, struct table d);
int wValid(int m, int n, struct table d);
void whitePlay(void);
void blackPlay(void);
void computerPlay(void);

char a[10][10];
char name1[20], name2[20];
int step, level, i, j, vi, mm[10][10];
WINDOW *win, *w1, *w2;

static int curses_active = 0;

int getche(void)
{
  if (curses_active)
    return wgetch(win ? win : stdscr);
  int c = getchar();
  if (c != '\n' && c != EOF)
    {
      int ch;
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    }
  return c;
}

static void draw_board_grid(void)
{
  int row, col;

  if (has_colors())
    wattron(win, COLOR_PAIR(PAIR_GRID));

  /* Top border (row 3) */
  mvwaddch(win, 3, 3, ACS_ULCORNER);
  for (col = 0; col < 8; col++)
    {
      waddch(win, ACS_HLINE);
      waddch(win, ACS_HLINE);
      waddch(win, ACS_HLINE);
      waddch(win, col < 7 ? ACS_TTEE : ACS_URCORNER);
    }

  /* 8 board rows */
  for (row = 0; row < 8; row++)
    {
      int y = 4 + 2 * row;
      mvwaddch(win, y, 3, ACS_VLINE);
      for (col = 0; col < 8; col++)
        {
          waddstr(win, "   ");
          waddch(win, ACS_VLINE);
        }

      if (row < 7)
        {
          mvwaddch(win, y + 1, 3, ACS_LTEE);
          for (col = 0; col < 8; col++)
            {
              waddch(win, ACS_HLINE);
              waddch(win, ACS_HLINE);
              waddch(win, ACS_HLINE);
              waddch(win, col < 7 ? ACS_PLUS : ACS_RTEE);
            }
        }
    }

  /* Bottom border (row 19) */
  mvwaddch(win, 19, 3, ACS_LLCORNER);
  for (col = 0; col < 8; col++)
    {
      waddch(win, ACS_HLINE);
      waddch(win, ACS_HLINE);
      waddch(win, ACS_HLINE);
      waddch(win, col < 7 ? ACS_BTEE : ACS_LRCORNER);
    }

  if (has_colors())
    wattroff(win, COLOR_PAIR(PAIR_GRID));
}

void show(void)
{
  int ii, jj;

  for (ii = 1; ii <= 8; ii++)
    {
      for (jj = 1; jj <= 8; jj++)
        {
          int y = 2 * ii + 2;
          int x = 1 + 4 * jj;
          if (a[ii][jj] == 'b')
            mvwaddch(win, y, x, ' ');
          if (a[ii][jj] == 'r')
            {
              if (has_colors())
                wattron(win, COLOR_PAIR(PAIR_RED) | A_BOLD);
              mvwaddch(win, y, x, '@');
              if (has_colors())
                wattroff(win, COLOR_PAIR(PAIR_RED) | A_BOLD);
            }
          if (a[ii][jj] == 'w')
            {
              if (has_colors())
                wattron(win, COLOR_PAIR(PAIR_WHITE) | A_BOLD);
              mvwaddch(win, y, x, 'O');
              if (has_colors())
                wattroff(win, COLOR_PAIR(PAIR_WHITE) | A_BOLD);
            }
          if (a[ii][jj] == '*')
            mvwaddch(win, y, x, '*');
          if (a[ii][jj] == '|')
            {
              const char frames[] = { '|', '/', '-', '\\', '|' };
              int f;
              for (f = 0; f < 5; f++)
                {
                  mvwaddch(win, y, x, frames[f]);
                  wrefresh(win);
                  napms(20);
                }
            }
        }
    }
  wrefresh(win);
}

void initialise(void)
{
  int ii, jj;

  if (has_colors())
    {
      start_color();
      use_default_colors();
      init_pair(PAIR_WHITE,  COLOR_WHITE,  -1);
      init_pair(PAIR_RED,    COLOR_RED,    -1);
      init_pair(PAIR_GRID,   COLOR_GREEN,  -1);
      init_pair(PAIR_CURSOR, COLOR_YELLOW, -1);
      init_pair(PAIR_TITLE,  COLOR_CYAN,   -1);
    }

  /* Draw background title and decorative panel on stdscr first */
  clear();
  if (has_colors())
    attron(COLOR_PAIR(PAIR_TITLE) | A_BOLD);
  mvprintw(1, 12, "O T H E L L O");
  if (has_colors())
    attroff(COLOR_PAIR(PAIR_TITLE) | A_BOLD);
  mvprintw(3, 41, "Controls:");
  mvprintw(4, 41, "  Arrows or u/d/l/r : Move");
  mvprintw(5, 41, "  Enter             : Place piece");
  mvprintw(6, 41, "  q                 : Quit");
  mvprintw(8, 41, "%s : O", name1);
  mvprintw(9, 41, "%s : @", name2);
  refresh();

  win = newwin(21, 38, 2, 1);
  w1  = newwin(1, 38, 13, 41);
  w2  = newwin(1, 38, 18, 41);

  keypad(win, TRUE);
  wattron(w1, A_BOLD);
  wattron(w2, A_BOLD);

  wclear(win);
  draw_board_grid();

  for (ii = 0; ii < 10; ii++)
    for (jj = 0; jj < 10; jj++)
      a[ii][jj] = 'b';

  a[4][4] = a[5][5] = 'w';
  a[4][5] = a[5][4] = 'r';
  show();
}

void end(void)
{
  WINDOW *ww;
  int cc, cw, cr, ii, jj;

  beep();

  delwin(w1);
  delwin(w2);

  cw = cr = 0;
  for (ii = 1; ii <= 8; ii++)
    for (jj = 1; jj <= 8; jj++)
      {
        if (a[ii][jj] == 'w') cw++;
        if (a[ii][jj] == 'r') cr++;
        a[ii][jj] = 'b';
        show();
        napms(10);
      }

  cc = 1;
  for (ii = 1; ii <= 8; ii++)
    for (jj = 1; jj <= 8; jj++)
      {
        if (cc <= cw) a[ii][jj] = 'w';
        if (cc > 64 - cr) a[ii][jj] = 'r';
        show();
        napms(10);
        cc++;
      }

  ww = newwin(10, 32, 11, 41);

  wprintw(ww, "\n\n %s scores %d", name1, cw);
  wprintw(ww, "\n %s scores %d", name2, cr);

  if (cw < cr)  wprintw(ww, "\n\n %s wins !", name2);
  if (cw > cr)  wprintw(ww, "\n\n %s wins !", name1);
  if (cw == cr) wprintw(ww, "\n\n  Draw  !!");

  wprintw(ww, "\n\n Press any key to exit...");
  flushinp();
  wrefresh(ww);

  getche();
  delwin(ww);
  delwin(win);
  endwin();
  curses_active = 0;
  exit(0);
}

void input(void)
{
  int c;

  if (i < 1 || i > 8) i = 4;
  if (j < 1 || j > 8) j = 4;

  flushinp();
  wclear(w1);
  wprintw(w1, "Choose point & press Return. ");
  wrefresh(w1);

  for (;;)
    {
      show();
      if (has_colors())
        wattron(win, COLOR_PAIR(PAIR_CURSOR) | A_BOLD);
      mvwaddch(win, 2 * i + 2, 2 + 4 * j, '<');
      if (has_colors())
        wattroff(win, COLOR_PAIR(PAIR_CURSOR) | A_BOLD);
      wmove(win, 2 * i + 2, 2 + 4 * j);
      wrefresh(win);

      c = getche();
      mvwaddch(win, 2 * i + 2, 2 + 4 * j, ' ');

      if ((c == '\n') || (c == '\r') || (c == KEY_ENTER))
        break;
      if ((c == F20) || (c == 'Q') || (c == KEY_F(20)))
        end();

      if ((c == UP)    || (c == 'u')) i--;
      if ((c == DOWN)  || (c == 'd')) i++;
      if ((c == LEFT)  || (c == 'l')) j--;
      if ((c == RIGHT) || (c == 'r')) j++;
      if (i < 1) i = 1;
      if (j < 1) j = 1;
      if (i > 8) i = 8;
      if (j > 8) j = 8;
    }
}

void wleft()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii][jj-1]!='r') return;

  kk=jj-1;
  while(a[ii][kk]=='r')kk--;

  if (a[ii][kk]=='b') return;

  while(jj>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      jj--;
    }

  vi=1;
  return;
}







void wright()
{
  int ii,jj,kk;

  ii=i; jj=j;
  if (a[ii][jj+1]!='r') return;

  kk=jj+1;
  while(a[ii][kk]=='r')kk++;

  if (a[ii][kk]=='b') return;

  while(jj<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      jj++;
    }

  vi=1;
  return;
}







void wup()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii-1][jj]!='r') return;

  kk=ii-1;
  while(a[kk][jj]=='r')kk--;

  if (a[kk][jj]=='b') return;

  while(ii>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      ii--;
    }

  vi=1;
  return;
}






void wdown()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii+1][jj]!='r') return;

  kk=ii+1;
  while(a[kk][jj]=='r')kk++;

  if (a[kk][jj]=='b') return;

  while(ii<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      ii++;
    }

  vi=1;
  return;
}






void wLD()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii+1][jj-1]!='r') return;

  kk=jj-1;ll=ii+1;
  while(a[ll][kk]=='r'){kk--;ll++;}

  if (a[ll][kk]=='b') return;

  while(jj>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      jj--;
      ii++;
    }

  vi=1;
  return;
}









void wLU()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii-1][jj-1]!='r') return;

  kk=jj-1;ll=ii-1;
  while(a[ll][kk]=='r'){kk--;ll--;}

  if (a[ll][kk]=='b') return;

  while(jj>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      jj--;
      ii--;
    }

  vi=1;
  return;
}





void wRU()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii-1][jj+1]!='r') return;

  kk=jj+1;ll=ii-1;
  while(a[ll][kk]=='r'){kk++;ll--;}

  if (a[ll][kk]=='b') return;

  while(jj<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      jj++;
      ii--;
    }

  vi=1;
  return;
}







void wRD()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii+1][jj+1]!='r') return;

  kk=jj+1;ll=ii+1;
  while(a[ll][kk]=='r'){kk++;ll++;}

  if (a[ll][kk]=='b') return;

  while(jj<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='w';
      show();
      jj++;
      ii++;
    }

  vi=1;
  return;
}







void rleft()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii][jj-1]!='w') return;

  kk=jj-1;
  while(a[ii][kk]=='w')kk--;

  if (a[ii][kk]=='b') return;

  while(jj>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      jj--;
    }

  vi=1;
  return;
}







void rright()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii][jj+1]!='w') return;

  kk=jj+1;
  while(a[ii][kk]=='w')kk++;

  if (a[ii][kk]=='b') return;

  while(jj<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      jj++;
    }

  vi=1;
  return;
}







void rup()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii-1][jj]!='w') return;

  kk=ii-1;
  while(a[kk][jj]=='w')kk--;

  if (a[kk][jj]=='b') return;

  while(ii>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      ii--;
    }

  vi=1;
  return;
}






void rdown()
{
  int ii,jj,kk;
  ii=i; jj=j;

  if (a[ii+1][jj]!='w') return;

  kk=ii+1;
  while(a[kk][jj]=='w')kk++;

  if (a[kk][jj]=='b') return;

  while(ii<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      ii++;
    }

  vi=1;
  return;
}















void rLD()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii+1][jj-1]!='w') return;

  kk=jj-1;ll=ii+1;
  while(a[ll][kk]=='w'){kk--;ll++;}

  if (a[ll][kk]=='b') return;

  while(jj>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      jj--;
      ii++;
    }

  vi=1;
  return;
}









void rLU()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii-1][jj-1]!='w') return;

  kk=jj-1;ll=ii-1;
  while(a[ll][kk]=='w'){kk--;ll--;}

  if (a[ll][kk]=='b') return;

  while(jj>=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      jj--;
      ii--;
    }

  vi=1;
  return;
}





void rRU()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii-1][jj+1]!='w') return;

  kk=jj+1;ll=ii-1;
  while(a[ll][kk]=='w'){kk++;ll--;}

  if (a[ll][kk]=='b') return;

  while(jj<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      jj++;
      ii--;
    }

  vi=1;
  return;
}







void rRD()
{
  int ii,jj,kk,ll;
  ii=i; jj=j;

  if (a[ii+1][jj+1]!='w') return;

  kk=jj+1;ll=ii+1;
  while(a[ll][kk]=='w'){kk++;ll++;}

  if (a[ll][kk]=='b') return;

  while(jj<=kk)
    {
      a[ii][jj]='|';
      show();
      a[ii][jj]='r';
      show();
      jj++;
      ii++;
    }

  vi=1;
  return;
}







void whitePlay()
{
  struct table hh = {{{0}}};
  wclear(w2);

  vi=0;
  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      hh.x[i][j]=a[i][j];

  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      if(wValid(i,j,hh)==1) vi=1;

  if (vi==0) 
    {
      wclear(w1);
      wprintw(w1,"No valid move");
      wrefresh(w1);
      fflush(stdin);
      getche();
      end();
    }

  wclear(w2);
  wprintw(w2,"%s to play with O",name1);
  wrefresh(w2);
 label1:

  vi=0;
  input();
  if (a[i][j]!='b') goto label1;

  wleft();
  wright();
  wup();
  wdown();
  wRU();
  wLU();
  wLD();
  wRD();

  if (vi==0)
    {
      wclear(w1);
      wprintw(w1,"Invalid");
      wrefresh(w1);
      goto label1;
    }
  return;
}






void blackPlay()
{
  struct table hh = {{{0}}};

  wclear(w2);

  vi=0;
  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      hh.x[i][j]=a[i][j];

  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      if(rValid(i,j,hh)==1) vi=1;

  if (vi==0) 
    {
      wclear(w1);
      wprintw(w1,"No valid move");
      wrefresh(w1);
      fflush(stdin);
      getche();
      end();
    }

  wclear(w2);
  wprintw(w2,"%s to play with @",name2);
  wrefresh(w2);
 label1:
  vi=0;
  input();

  rleft();
  rright();
  rup();
  rdown();
  rRU();
  rLU();
  rLD();
  rRD();

  if (vi==0)
    {
      wclear(w1);
      wprintw(w1,"Invalid");
      wrefresh(w1);
      goto label1;
    }
  return;
}







int rValid(int m,int n,struct table d)
{
  int ii,jj;

  if (d.x[m][n]=='b')
    {
      ii=m;jj=n-1;/*left*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){jj--;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m;jj=n+1;/*right*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){jj++;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m-1;jj=n;/*up*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){ii--;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m+1;jj=n;/*down*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){ii++;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m+1;jj=n-1;/*LD*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){jj--;ii++;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m-1;jj=n-1;/*LU*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){jj--;ii--;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m+1;jj=n+1;/*RD*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){jj++;ii++;}
	  if(d.x[ii][jj]=='r')return 1;
	}
      ii=m-1;jj=n+1;/*RU*/
      if (d.x[ii][jj]=='w')
	{
	  while(d.x[ii][jj]=='w'){jj++;ii--;}
	  if(d.x[ii][jj]=='r')return 1;
	}
    }

  return 0;

}








int wValid(int m,int n,struct table d)
{
  int ii,jj;

  if (d.x[m][n]=='b')
    {
      ii=m;jj=n-1;/*left*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){jj--;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m;jj=n+1;/*right*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){jj++;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m-1;jj=n;/*up*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){ii--;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m+1;jj=n;/*down*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){ii++;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m+1;jj=n-1;/*LD*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){jj--;ii++;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m-1;jj=n-1;/*LU*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){jj--;ii--;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m+1;jj=n+1;/*RD*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){jj++;ii++;}
	  if(d.x[ii][jj]=='w')return 1;
	}
      ii=m-1;jj=n+1;/*RU*/
      if (d.x[ii][jj]=='r')
	{
	  while(d.x[ii][jj]=='r'){jj++;ii--;}
	  if(d.x[ii][jj]=='w')return 1;
	}
    }

  return 0;

}










struct table rPlay(int m,int n,struct table d)
{

  int ii,jj,kk,ll;


  ii=m; jj=n;

  if (d.x[ii][jj-1]=='w') 
    {
      kk=jj-1;
      while(d.x[ii][kk]=='w')kk--;
      if (d.x[ii][kk]=='r')
	while(jj>=kk)
	  {
	    d.x[ii][jj]='r';
	    jj--;
	  }
    }


  ii=m; jj=n;

  if (d.x[ii][jj+1]=='w')
    {
      kk=jj+1;
      while(d.x[ii][kk]=='w')kk++;
      if (d.x[ii][kk]=='r')
	while(jj<=kk)
	  {
	    d.x[ii][jj]='r';
	    jj++;
	  }
    }


  ii=m; jj=n;

  if (d.x[ii-1][jj]=='w')
    {
      kk=ii-1;
      while(d.x[kk][jj]=='w')kk--;
      if (d.x[kk][jj]=='r')
	while(ii>=kk)
	  {
	    d.x[ii][jj]='r';
	    ii--;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii+1][jj]=='w')
    {
      kk=ii+1;
      while(d.x[kk][jj]=='w')kk++;
      if (d.x[kk][jj]=='r')
	while(ii<=kk)
	  {
	    d.x[ii][jj]='r';
	    ii++;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii+1][jj-1]=='w')
    {
      kk=jj-1;ll=ii+1;
      while(d.x[ll][kk]=='w'){kk--;ll++;}
      if (d.x[ll][kk]=='r')
	while(jj>=kk)
	  {
	    d.x[ii][jj]='r';
	    jj--;
	    ii++;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii-1][jj-1]=='w') 
    {
      kk=jj-1;ll=ii-1;
      while(d.x[ll][kk]=='w'){kk--;ll--;}
      if (d.x[ll][kk]=='r') 
	while(jj>=kk)
	  {
	    d.x[ii][jj]='r';
	    jj--;
	    ii--;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii-1][jj+1]=='w') 
    {
      kk=jj+1;ll=ii-1;
      while(d.x[ll][kk]=='w'){kk++;ll--;}
      if (d.x[ll][kk]=='r')
	while(jj<=kk)
	  {
	    d.x[ii][jj]='r';
	    jj++;
	    ii--;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii+1][jj+1]=='w') 
    {
      kk=jj+1;ll=ii+1;
      while(d.x[ll][kk]=='w'){kk++;ll++;}
      if (d.x[ll][kk]=='r') 
	while(jj<=kk)
	  {
	    d.x[ii][jj]='r';
	    jj++;
	    ii++;
	  }
    }


  return d;
}








struct table wPlay(int m,int n,struct table d)
{

  int ii,jj,kk,ll;


  ii=m; jj=n;

  if (d.x[ii][jj-1]=='r') 
    {
      kk=jj-1;
      while(d.x[ii][kk]=='r')kk--;
      if (d.x[ii][kk]=='w')
	while(jj>=kk)
	  {
	    d.x[ii][jj]='w';
	    jj--;
	  }
    }


  ii=m; jj=n;

  if (d.x[ii][jj+1]=='r')
    {
      kk=jj+1;
      while(d.x[ii][kk]=='r')kk++;
      if (d.x[ii][kk]=='w')
	while(jj<=kk)
	  {
	    d.x[ii][jj]='w';
	    jj++;
	  }
    }



  ii=m; jj=n;

  if (d.x[ii-1][jj]=='r')
    {
      kk=ii-1;
      while(d.x[kk][jj]=='r')kk--;
      if (d.x[kk][jj]=='w')
	while(ii>=kk)
	  {
	    d.x[ii][jj]='w';
	    ii--;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii+1][jj]=='r')
    {
      kk=ii+1;
      while(d.x[kk][jj]=='r')kk++;
      if (d.x[kk][jj]=='w')
	while(ii<=kk)
	  {
	    d.x[ii][jj]='w';
	    ii++;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii+1][jj-1]=='r')
    {
      kk=jj-1;ll=ii+1;
      while(d.x[ll][kk]=='r'){kk--;ll++;}
      if (d.x[ll][kk]=='w')
	while(jj>=kk)
	  {
	    d.x[ii][jj]='w';
	    jj--;
	    ii++;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii-1][jj-1]=='r') 
    {
      kk=jj-1;ll=ii-1;
      while(d.x[ll][kk]=='r'){kk--;ll--;}
      if (d.x[ll][kk]=='w') 
	while(jj>=kk)
	  {
	    d.x[ii][jj]='w';
	    jj--;
	    ii--;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii-1][jj+1]=='r') 
    {
      kk=jj+1;ll=ii-1;
      while(d.x[ll][kk]=='r'){kk++;ll--;}
      if (d.x[ll][kk]=='w')
	while(jj<=kk)
	  {
	    d.x[ii][jj]='w';
	    jj++;
	    ii--;
	  }
    }

  ii=m; jj=n;

  if (d.x[ii+1][jj+1]=='r') 
    {
      kk=jj+1;ll=ii+1;
      while(d.x[ll][kk]=='r'){kk++;ll++;}
      if (d.x[ll][kk]=='w') 
	while(jj<=kk)
	  {
	    d.x[ii][jj]='w';
	    jj++;
	    ii++;
	  }
    }


  return d;
}



int countDiff(struct table d)
{
  int ii,jj,rc,wc;
  rc=wc=0;

  for (ii=1;ii<=8;ii++)
    for (jj=1;jj<=8;jj++)
      {
   	if (d.x[ii][jj]=='w')wc++;
   	if (d.x[ii][jj]=='r')rc++;
      }
  return (rc-wc);
}





int wEvaluate(int tt,int m,int n,struct table d)
{
  int count,ii,jj,sc,maxsc= -100;

  int rEvaluate(int, int ,int ,struct table);

  d=wPlay(m,n,d);

  if( (rValid(1,1,d)==1)||(rValid(1,8,d)==1)||(rValid(8,8,d)==1)||(rValid(8,1,d)==1)) return 30;

  if(tt==0) return countDiff(d);

  count=0;

  for (ii=1;ii<=8;ii++)
    for (jj=1;jj<=8;jj++)
      if(rValid(ii,jj,d)==1)
	{
	  count++;
	  sc=rEvaluate(tt-1,ii,jj,d);
	  if(sc>maxsc)maxsc=sc;
	}

  if  (count==0) return countDiff(d);
  else return maxsc;
}





int rEvaluate(int tt,int m,int n,struct table d)
{
  int count,ii,jj,sc,minsc= 100;

  int wEvaluate(int, int ,int ,struct table);


  d=rPlay(m,n,d);

  if( (wValid(1,1,d)==1)||(wValid(1,8,d)==1)||(wValid(8,8,d)==1)||(wValid(8,1,d)==1)) return -30;

  if(tt==0) return countDiff(d);

  count=0;

  for (ii=1;ii<=8;ii++)
    for (jj=1;jj<=8;jj++)
      if(wValid(ii,jj,d)==1)
	{
	  count++;
	  sc=wEvaluate(tt-1,ii,jj,d);
	  if (sc<minsc) minsc=sc;
	}

  if  (count==0)return countDiff(d);
  else return minsc;
}





void oldinput()
{
  int maxi,maxj,t,ii,jj,max,ct=0;

  maxi=maxj=0;
  max=0;

  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      if (a[i][j]=='b')
	{
	  ct=0;
	  t=1;
	  ii=i;jj=j-1;/*left*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){jj--;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }


	  t=1;
	  ii=i;jj=j+1;/*right*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){jj++;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }


	  t=1;
	  ii=i-1;jj=j;/*up*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){ii--;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }


	  t=1;
	  ii=i+1;jj=j;/*down*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){ii++;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }


	  t=1;
	  ii=i+1;jj=j-1;/*LD*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){jj--;ii++;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }



	  t=1;
	  ii=i-1;jj=j-1;/*LU*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){jj--;ii--;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }



	  t=1;
	  ii=i+1;jj=j+1;/*RD*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){jj++;ii++;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }



	  t=1;
	  ii=i-1;jj=j+1;/*RU*/
	  if (a[ii][jj]=='w')
	    {
	      while(a[ii][jj]=='w'){jj++;ii--;t++;}
	      if(a[ii][jj]=='r')ct+=t;
	    }


	  if (ct>0) { if( ((i==1)||(i==8))&&((j==1)||(j==8)) ) return;
	    if(ct>max) {max=ct;maxi=i;maxj=j;} }

	}

  if (max==0) 
    {
      wclear(w1);
      wprintw(w1,"No valid move");
      wrefresh(w1);
      fflush(stdin);
      getche();
      end();
    }


  i=maxi;j=maxj;
  return;

}














void cmpInput()
{
  int ii,jj,maxi= -100;
  struct table d = {{{0}}};

  /*  if(step<25){step++;oldinput();return;}   */


  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      d.x[i][j]=a[i][j];

  if(rValid(1,1,d)==1){i=1;j=1;return;}
  if(rValid(8,8,d)==1){i=8;j=8;return;}
  if(rValid(1,8,d)==1){i=1;j=8;return;}
  if(rValid(8,1,d)==1){i=8;j=1;return;}

  vi=0;
  for (i=1;i<=8;i++)
    for (j=1;j<=8;j++)
      {
	show();
	mm[i][j]=100;
	if (rValid(i,j,d)==1)
	  {
	    vi=1;
	    a[i][j]='*';
	    show();
	    mm[i][j]=rEvaluate(level,i,j,d);
	    a[i][j]='b';
	  }
	if (mm[i][j]!=100)
	  if (mm[i][j]>maxi) maxi=mm[i][j];
      }

  if (vi==0) 
    {
      wclear(w1);
      wprintw(w1,"No valid move");
      wrefresh(w1);
      fflush(stdin);
      getche();
      end();
    }


  for (ii=1;ii<=8;ii++)
    for (jj=1;jj<=8;jj++)
      if (mm[ii][jj]==maxi) 
	{
	  i=ii;j=jj;
	}

  return;
}





void computerPlay()
{
  wclear(w2);
  wprintw(w2,"%s to play with @",name2);
  wrefresh(w2);
  cmpInput();

  /*   fflush(stdin);getche();  */

  rleft();
  rright();
  rup();
  rdown();
  rRU();
  rLU();
  rLD();
  rRD();

  putchar(7);

  return;
}


int main(void)
{
  int e;

  printf("\nDo you want to play with the computer ? [N] ");
  e = getche();
  if ((e == 'y') || (e == 'Y'))
    {
      int ee;
      printf("\nEnter the name of the player : ");
      if (scanf("%19s", name1) != 1) return 1;
      printf("\nLevel : [1-4] ");
      if (scanf("%d", &level) != 1) level = 2;
      if (level < 1) level = 1;
      if (level > 4) level = 4;
      {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
          ;
      }
      strcpy(name2, "Computer");
      printf("\n%s will play with O\n", name1);
      printf("%s will play with @\n", name2);

      printf("\nDo you want to open ? [Y] ");
      ee = getche();

      sleep(1);

      initscr();
      cbreak();
      noecho();
      curses_active = 1;

      initialise();

      if ((ee == 'n') || (ee == 'N'))
        computerPlay();
    l1:
      whitePlay();
      computerPlay();
      goto l1;
    }

  printf("\nEnter the name of the first  player : ");
  if (scanf("%19s", name1) != 1) return 1;
  printf("\nEnter the name of the second player : ");
  if (scanf("%19s", name2) != 1) return 1;

  printf("\n%s will play with O\n", name1);
  printf("%s will play with @\n", name2);

  sleep(2);

  initscr();
  cbreak();
  noecho();
  curses_active = 1;

  initialise();

 l:
  whitePlay();
  blackPlay();
  goto l;

  return 0;
}
