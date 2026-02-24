







#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

static GdkPixmap *pixmap = NULL;

float *data;
int n;
float hi,lo;

static gint configure_event( GtkWidget         *widget,
                             GdkEventConfigure *event )
{
  //  static int ind=0;

  int wid,hei,t;
  float x0,y0;

  //  if(ind==1)return TRUE;
  //  ind=1;

  if (pixmap)
    gdk_pixmap_unref(pixmap);

  pixmap = gdk_pixmap_new(widget->window,
			  wid=widget->allocation.width,
			  hei=widget->allocation.height,
			  -1);

  gdk_draw_rectangle (pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);


  x0=0;
  y0=data[0]*hei/(2*hi);


  for(t=1;t<n;t++)
    gdk_draw_line(pixmap,
		  widget->style->black_gc,
		  1.0*(t-1)*wid/n,
		  hei/2.0y+data[t-1]*hei/(2*hi),
		  1.0*t*wid/n,
		  hei/2.0+data[t]*hei/(2*hi));
  

  return TRUE;
}



/* Redraw the screen from the backing pixmap */
static gint expose_event( GtkWidget      *widget,
                          GdkEventExpose *event )
{
  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		  pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return FALSE;
}

void quit ()
{
  gtk_exit (0);
}









int main( int   argc, 
          char *argv[] )
{
  GtkWidget *window;
  GtkWidget *drawing_area;
  GtkWidget *vbox;

  GtkWidget *button;

  FILE *in;
  int t;
  float dum;
  char *s=(char*)malloc(50*sizeof(char));



  hi= -10000;
  lo=10000;

  printf("\n\n_DataFile: ");
  scanf("%s",s);
  in=fopen(s,"r");

  n= -1;
  while(!feof(in))
    {
      fscanf(in,"%f",&dum); printf("\n%f ",dum);
      if(dum>hi)hi=dum; else if(dum<lo)lo=dum;
      ++n;
    }
  printf("\n\n_NoOfDataPoints: %d\n\n", n);


  data=(float*)malloc(n*sizeof(float));

  rewind(in);
  for(t=0;t<n;t++)
    {
      fscanf(in,"%f",&data[t]);
    }

  if(fabs(hi)>fabs(lo))hi=fabs(hi); else hi=fabs(lo);

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window, "Test Input");

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (quit), NULL);

  /* Create the drawing area */

  drawing_area = gtk_drawing_area_new ();
  gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 200);
  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /* Signals used to handle backing pixmap */

  gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
		      (GtkSignalFunc) expose_event, NULL);
  gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
		      (GtkSignalFunc) configure_event, NULL);


  button = gtk_button_new_with_label ("Quit");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (window));
  gtk_widget_show (button);

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}
/* example-end */




















......................................


#include <gtk/gtk.h>

struct table { char x[10][10]; };

char a[10][10];
char name1[20],name2[20];
int step,level,i,j,vi,mm[10][10];
WINDOW *win,*w1,*w2;


void show( void)
   {
   int ii,jj;

   for (ii=1;ii<=8;ii++)
	{
   for (jj=1;jj<=8;jj++)
	{
	if(a[ii][jj]=='b')mvwaddch(win,2*ii+2,1+4*jj,' ');
	if(a[ii][jj]=='r')mvwaddch(win,2*ii+2,1+4*jj,'¢');
	if(a[ii][jj]=='w')mvwaddch(win,2*ii+2,1+4*jj,'¡');
	if(a[ii][jj]=='*')mvwaddch(win,2*ii+2,1+4*jj,'*');
	if(a[ii][jj]=='|')
		{
		mvwaddch(win,2*ii+2,1+4*jj,'x');
		wrefresh(win);
		mvwaddch(win,2*ii+2,1+4*jj,'o');
		wrefresh(win);
		mvwaddch(win,2*ii+2,1+4*jj,'q');
		wrefresh(win);
		mvwaddch(win,2*ii+2,1+4*jj,'s');
		wrefresh(win);
		mvwaddch(win,2*ii+2,1+4*jj,'x');
		wrefresh(win);
		}
	}
	}
   wrefresh(win);
   }






void initialise()
   {
   int ii,jj;

   win=newwin(20,37,2,1);
   w1=newwin(1,36,13,41);
   w2=newwin(1,36,18,41);

   wprintw(win,"(0");
   wsetattr(w1,_BOLD);
   wsetattr(w2,_BLINK|_BOLD);



   wclear(win);


mvwaddstr(win,3,2," ¦£££­£££­£££­£££­£££­£££­£££­£££¨ ");
mvwaddstr(win,4,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,5,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,6,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,7,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,8,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,9,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,10,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,11,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,12,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,13,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,14,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,15,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,16,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,17,2," ª£££¥£££¥£££¥£££¥£££¥£££¥£££¥£££« ");
mvwaddstr(win,18,2," ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤   ¤ ");
mvwaddstr(win,19,2," §£££¬£££¬£££¬£££¬£££¬£££¬£££¬£££© ");



   for (ii=0;ii<10;ii++)
   for (jj=0;jj<10;jj++)
      a[ii][jj]='b';

   a[4][4]=a[5][5]='w';
   a[4][5]=a[5][4]='r';
   show();
   }






void end()
   {

WINDOW *ww;
int cc,cw,cr,cb,ii,jj;

putchar(7);

   delwin(w1);
   delwin(w2);

   cw=cr=0;
   for (ii=1;ii<=8;ii++)
   for (jj=1;jj<=8;jj++)
		{
		if (a[ii][jj]=='w')cw++;
		if (a[ii][jj]=='r')cr++;
		if (a[ii][jj]=='b')cb++;
		a[ii][jj]='b';
		show();
		}


   cc=1;
   for (ii=1;ii<=8;ii++)
   for (jj=1;jj<=8;jj++)
		{
		if (cc<=cw) a[ii][jj]='w';
		if (cc>64-cr) a[ii][jj]='r';
		show();
		cc++;
		}
   ww=newwin(10,30,10,40);

   wprintw(ww,"\n\n %s scores %d",name1,cw); 
   wprintw(ww,"\n %s scores %d",name2,cr); 

   if(cw<cr)   wprintw(ww,"\n\n %s wins !",name2); 
   if(cw>cr)   wprintw(ww,"\n\n %s wins !",name1); 
   if(cw==cr)   wprintw(ww,"\n\n  Draw  !!"); 

   wrefresh(ww);

   fflush(stdin);
   getche();
   puts("\033[2J");
   exit(0);
   }






void input()
   {
   int c;

   wclear(w1);
   wprintw(w1,"Choose point & press Return. ");
   wrefresh(w1);
   fflush(stdin);
   c=getche();
   while ((c!=13)&&(c!=F20) )
   	{
   	show();
   	if ((c==UP)||(c=='u')) i--;
   	if ((c==DOWN)||(c=='d')) i++;
   	if ((c==LEFT)||(c=='l')) j--;
   	if ((c==RIGHT)||(c=='r')) j++;
   	if (i<1)i=1;
   	if (j<1)j=1;
   	if (i>8)i=8;
   	if (j>8)j=8;
	mvwaddch(win,2*i+2,2+4*j,'¿');
	wmove(win,2*i+2,2+4*j);
	wrefresh(win);
	mvwaddch(win,2*i+2,2+4*j,' ');
	c=getche();
   	}
   if (c==F20) end();

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
   struct table hh;
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
   wprintw(w2,"%s to play with \033(0¡\033(B",name1);
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
struct table hh;

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
   wprintw(w2,"%s to play with \033(0¢\033(B",name2);
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
struct table d;

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
   wprintw(w2,"%s to play with \033(0¢\033(B",name2);
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









main() {

int e;


puts("[2J");

/*  ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º */

puts("1;1;1;15;1;2;12;0{ @");

/*
puts("TyxExEDEDEDEDyT/TnNoVsSsSsSsSv@;");
puts("TyxEx}|}|}|}|yT/TnNoVvVvVvVvVv@;");    alt  coins  */




puts("?gwSw{{{{{{{{w?/?INTNVNVNVNVNF?;");
puts("?gwSwCCCCCCCCw?/?INTNSKSKSKSKF?;");    /*  coins  */


puts("owowowowowowowo/EAEAEAEAEAEAEAE;");
puts("TiTi~??~~~~iTiT/TiTi~??~~~~iTiT;");
puts("tyty~??~~~tytyt/UiUi}??}}}UiUiU;");
puts("OgOgowowowowowo/TiTi~??}}}UiUiU;");
puts("TiTi~??~~~tytyt/DADAFAEAEAEAEAE;");
puts("owowowowowOgOgO/UiUi}??~~~TiTiT;");
puts("tyty~??~~~TiTiT/EAEAEAEBFBDADAD;");
puts("TiTi~??~~~tytyt/TiTi~??}}}UiUiU;");
puts("tyty~oo~~~dYdYd/UiUi}??~~~UhUhU;");
puts("tyty~??~~~tytyt/EAEAEAEAEAEAEAE;");
puts("owowowowowowowo/UiUi}??}}}UiUiU;");   /* grid  */



/*  alternate grid :
puts("ooooooooooooooo/@A@A@A@A@A@A@A@;");
puts("????TiT~~~?????/????TiT~~~?????;");
puts("ooootyt~~~ooooo/@A@ATiT~~~@A@A@;");
puts("????O_Ooooooooo/????TiT~~~@A@A@;");
puts("????TiT~~~ooooo/????@A@A@A@A@A@;");
puts("oooooooooo?????/@A@ATiT~~~?????;");
puts("ooootyt~~~?????/@A@A@A@A@A?????;");
puts("????TiT~~~ooooo/????TiT~~~@A@A@;");
puts("ooootyt~~~?????/@A@ATiT~~~?????;");
puts("ooootyt~~~ooooo/@A@A@A@A@A@A@A@;");
puts("ooooooooooooooo/@A@ATiT~~~@A@A@;");
*/


puts("?oGsw{KK{K{wo??/?FGVN^WW^W^NF??;");
puts("_?ogsw{ooo_????/??FGVN^WWSWK???;");
puts("CCwC{{w????????/?ONO^^N@NPN^]O?;");
puts("??_O_ooooo_????/?FGVN^YYZZHKC??;");
puts("CCwC{{w????????/??NON^^WGC?????;");
puts("??_O_ooooo_????/?FGVN^WWW^NF???;");   /* othello  */



puts("TiTiTiTiTiTiTiT/TiTiTiTiTiTiTiT;");
puts("iTiDAPiPiPADiTi/iTiO_TaTaT_OiTi;");
puts("iTiTaPa@APaPaTi/iTiTiTiO_TiTiTi;");
puts("iTA@iTiTiTA@iTi/iT_?iSiSiS_?iTi;");
puts("iTA@iPiPiPiPiTi/iT_?iCiCiCiCiTi;");
puts("iTA@iTiTiTiTiTi/iT_?iDiDiDiDiTi;");  /* background  othello  */
puts("TiTy|mTmTm|yTiT/TiTf^i\i\i^nTiT;");
puts("TiTi\m\}|m\m\iT/TiTiTiTn^iTiTiT;");
puts("Ti|}TiTiTi|}TiT/Ti^~TjTjTj^~TiT;");
puts("Ti|}TmTmTmTmTiT/Ti^~TzTzTzTzTiT;");
puts("Ti|}TiTiTiTiTiT/Ti^~TyTyTyTyTiT;");


puts("???????????????/?GKEFBFFN\Xpp_?;");  /* arrow */


 /*puts("????}{{wwOo__??/????FF?@BFM[xp`;"); alt   arrow */

puts("œ+ @|");

printf("Do you want to play with the computer ? [N] ");
fflush(stdin);
e=getche();
if ((e=='y')||(e=='Y')) 
	{
	int ee;
	printf("[2J\nEnter the name of the player : ");scanf("%s",name1);
	printf("\n\nLevel : [1-4] ");scanf("%d",&level);
	strcpy(name2,"Computer");
	printf("[2J%s  will play with \033(0¡\033(B",name1);
	printf("\n%s  will play with \033(0¢\033(B",name2);

	printf("\n\nDo you want to open ? [Y] ");
	fflush(stdin);ee=getche();

	sleep(1);

	initscr();

	initialise();

	printf("[H\n           #3 ®¯°±²²³\n");
	printf("           #4 ®¯°±²²³");

clear();
printw("\n\n\n\n\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");

refresh();

	if( (ee=='n')||(ee=='N'))computerPlay(); 
l1:
	   whitePlay();
	   computerPlay();
	  goto l1;
	}





printf("[2J");
printf("\n\nEnter the name of the first  player : ");scanf("%s",name1);
printf("\n\nEnter the name of the second player : ");scanf("%s",name2);

printf("[2J%s  will play with \033(0¡\033(B",name1);
printf("\n%s  will play with \033(0¢\033(B",name2);

sleep(2);

initscr();

   initialise();

printf("[H           #3 ®¯°±²²³\n");
printf("           #4 ®¯°±²²³");


clear();
printw("\n\n\n\n\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");
printw("\n                                      ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ´µ¶·¸¹¹µ");

refresh();


l:
   whitePlay();
   blackPlay();
  goto l;

}




