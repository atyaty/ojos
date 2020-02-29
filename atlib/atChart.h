#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <iostream>
#include <gtkmm.h>
#include <vector>
#include <stdio.h>

#define DRAWAREA_AXIXS_L        0
#define DRAWAREA_AXIXS_R        1
#define DRAWAREA_AXIXS_T        2
#define DRAWAREA_AXIXS_B        3

#define PUNKT_SYMBOL_BRAK       0
#define PUNKT_SYMBOL_JEDEN      1
#define PUNKT_SYMBOL_DWA_POZ    2
#define PUNKT_SYMBOL_DWA_PION   3
#define PUNKT_SYMBOL_CZTERY     4
#define PUNKT_SYMBOL_PIEC       5
#define PUNKT_SYMBOL_SZESC_POZ  6
#define PUNKT_SYMBOL_SZESC_PION 7

using namespace std;
//------------------------------------------
class atAxis{
public:
    atAxis(){visible=false;auto_ax=true; color_r=0; color_g=0; color_b=0; line_width=1;formatT="%5.2f";ile=0;}
    bool auto_ax;
    bool visible;
    double min, max, size;                                //minimalna wartość na osi, max, długość osi
    unsigned ile;                                         //ilośc punktów na osi
    double podzialka_min, podzialka, wys,wysT;            //podziałka: początek, odstęp, wysokośc kreski-ułamek, wysokość tekstu
    bool odwrocenie;
    bool grid;
    double poz(double p);                                 //pozycja jako ułamek ekranowej długości osi
    double line_width;
    double color_r,color_g,color_b;
    string formatT;
    void podzialka_auto();
    string to_string(double p);
    void set(double m,double s, double p, double w, double t){podzialka_min=m;size=s;podzialka=p;wys=w;wysT=t;visible=true;auto_ax=false;odwrocenie=false;grid=false;}
    void set(double w, double t){wys=w;wysT=t;visible=true;auto_ax=true;odwrocenie=false;grid=false;}
    void setLine(double l,double r, double g, double b){line_width=l;color_r=r;color_g=g;color_b=b;}
    void setMax(double x);
    void clear(){ile=0;}
};

//-------------------------------------------
class atSeria{
public:
    atSeria(){punkt=0;symbol=0;line_width=0;}
    vector<double> X;
    vector<double> Y;
    void clear(){X.clear();Y.clear();punkt=0;}
    unsigned long size(){return X.size();}
    unsigned long punkt;                     //wskaźnik punktu
    void reset(){punkt=0;}
    void next(){punkt++;}
    bool eof(){return punkt>=X.size();}
    atAxis *xax,*yax;
    double line_width;
    double color_r,color_g,color_b;
    double colorp_r,colorp_g,colorp_b;
    unsigned symbol;
    void setLine(double l,double r, double g, double b){line_width=l;color_r=r;color_g=g;color_b=b;}
    void setPunkt(unsigned s,double r, double g, double b){symbol=s;colorp_r=r;colorp_g=g;colorp_b=b;}
    void add(double x,double y);
    double pozX(){return xax->poz(X.at(punkt));}
    double pozY(){return yax->poz(Y.at(punkt));}
    double pozX(double w){return xax->poz(w);}
    double pozY(double w){return yax->poz(w);}
};

//----------------------------------------------
class atChart: public Gtk::DrawingArea {
public:
  atChart();
  atAxis xb_ax,yl_ax,xt_ax,yr_ax;
  vector<atSeria*> serie;
  int width,height;                      //szerokośc/wysokość pola rysowania
  int widthL,widthR,heightB,heightT;     //szerokośc/wysokość pól osi
  int widthLt,widthRt,heightBt,heightTt; //szerokośc/wysokość pól tekstów
  void force_redraw();

  void setXBax( double w, double t){ xb_ax.set(w,t); }
  void setYLax( double w, double t){ yl_ax.set(w,t); yl_ax.odwrocenie=true;}
  void setXTax( double w, double t){ xt_ax.set(w,t); }
  void setYRax( double w, double t){ yr_ax.set(w,t); yr_ax.odwrocenie=true;}
  void setXBax(double m,double s, double p, double w, double t){ xb_ax.set(m,s,p,w,t); }
  void setYLax(double m,double s, double p, double w, double t){ yl_ax.set(m,s,p,w,t); yl_ax.odwrocenie=true;}
  void setXTax(double m,double s, double p, double w, double t){ xt_ax.set(m,s,p,w,t); }
  void setYRax(double m,double s, double p, double w, double t){ yr_ax.set(m,s,p,w,t); yr_ax.odwrocenie=true;}
  void setXBline(double l,double r, double g, double b){xb_ax.setLine(l,r,g,b);}
  void setYLline(double l,double r, double g, double b){yl_ax.setLine(l,r,g,b);}
  void setXTline(double l,double r, double g, double b){xt_ax.setLine(l,r,g,b);}
  void setYRline(double l,double r, double g, double b){yr_ax.setLine(l,r,g,b);}
  void setSeria(atSeria *s,unsigned xax,unsigned yax);
  void setXB(bool g){xb_ax.grid=g;}
  void setXT(bool g){xt_ax.grid=g;}
  void setYL(bool g){yl_ax.grid=g;}
  void setYR(bool g){yr_ax.grid=g;}
  void clear(){xb_ax.clear();xt_ax.clear();yl_ax.clear();yr_ax.clear();for(unsigned i=0;i<serie.size();i++){serie[i]->clear();}}

protected:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  void rysuj_linie(atSeria* s,const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_punkty(atSeria* s,const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_osXB(const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_osXT(const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_osYL(const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_osYR(const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_punkt(atSeria* s,const Cairo::RefPtr<Cairo::Context>& cr);
  void rysuj_gridX(const Cairo::RefPtr<Cairo::Context>& cr,int i);
  void rysuj_gridY(const Cairo::RefPtr<Cairo::Context>& cr,int i);

};

#endif // DRAWAREA_H
