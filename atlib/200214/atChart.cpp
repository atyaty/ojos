#include "atChart.h"
//---------------------------------------------
atChart::atChart(){
    set_size_request (800, 400);

}
//------------------------------------------------------------------------
bool atChart::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {

     Gtk::Allocation allocation = get_allocation();
     widthLt = allocation.get_width()*yl_ax.wysT;
     widthRt = allocation.get_width()*yr_ax.wysT;
     heightTt= allocation.get_height()*xt_ax.wysT;
     heightBt= allocation.get_height()*xb_ax.wysT;
     widthL  = allocation.get_width()*yl_ax.wys+widthLt;
     widthR  = allocation.get_width()*yr_ax.wys+widthRt;
     heightT = allocation.get_height()*xt_ax.wys+heightTt;
     heightB = allocation.get_height()*xb_ax.wys+heightBt;
     width   = allocation.get_width()-widthL-widthR;
     height  = allocation.get_height()-heightB-heightT;

     //------------------------------------------------------------------------
     rysuj_osXB(cr);
     rysuj_osXT(cr);
     rysuj_osYL(cr);
     rysuj_osYR(cr);

     //-----------------------------------------------------------------------
     for(unsigned j=0; j<serie.size();j++){
         rysuj_linie( serie.at(j),cr );
         rysuj_punkty( serie.at(j),cr );
     }


     return true;
}
//-------------------------------------------------
void atChart::rysuj_linie(atSeria* s,const Cairo::RefPtr<Cairo::Context>& cr){
    if(s->line_width==0.) return;
    cr->set_line_width(s->line_width);
    cr->set_source_rgb(s->color_r,s->color_g,s->color_b);
    int posx=width*s->xax->poz(s->X.at(0));
    int posy=height*s->yax->poz(s->Y.at(0));
    cr->move_to(widthL+posx, heightT+posy );
    for(unsigned j=1; j<s->size(); j++){
        posx=width*s->xax->poz(s->X.at(j));
        posy=height*s->yax->poz(s->Y.at(j));
        cr->line_to( widthL+posx, heightT+posy );
    }
    cr->stroke();

}
//-------------------------------------------------
void atChart::rysuj_punkty(atSeria* s,const Cairo::RefPtr<Cairo::Context>& cr){
    if(s->symbol==PUNKT_SYMBOL_BRAK) return;
    cr->set_line_width(1);
    cr->set_source_rgb(s->colorp_r,s->colorp_g,s->colorp_b);
    s->punkt=0;
    while(!s->eof()){ rysuj_punkt(s,cr); }
    cr->stroke();
}
//-------------------------------------------------
void atChart::rysuj_osXB(const Cairo::RefPtr<Cairo::Context>& cr){

    if(xb_ax.auto_ax) xb_ax.podzialka_auto();
    cr->set_line_width(xb_ax.line_width);
    cr->set_source_rgb(xb_ax.color_r, xb_ax.color_g,xb_ax.color_b);
    cr->move_to( widthL, heightT+height);
    cr->line_to( widthL+width, heightT+height );
    if(xb_ax.ile<=0) return;
    int i=0,pos= width * xb_ax.poz(xb_ax.podzialka_min);
    double p=xb_ax.podzialka_min;
    while( pos<width ){
       cr->move_to(widthL + pos, height+heightT+heightBt);
       cr->line_to(widthL + pos, height+heightT);
       if(xb_ax.grid) rysuj_gridX(cr,pos);
       cr->move_to(widthL + pos, height+heightT+heightB);
       cr->show_text( xb_ax.to_string(p));

       i+=1;
       p =xb_ax.podzialka_min+i*xb_ax.podzialka;
       pos = width * xb_ax.poz(p);

    }
    cr->stroke();
}
//-------------------------------------------------
void atChart::rysuj_osXT(const Cairo::RefPtr<Cairo::Context>& cr){
    if(xt_ax.auto_ax) xt_ax.podzialka_auto();
    cr->set_line_width(xt_ax.line_width);
    cr->set_source_rgb(xt_ax.color_r, xt_ax.color_g,xt_ax.color_b);
    cr->move_to( widthL, heightT);
    cr->line_to( widthL+width, heightT );
    if(xt_ax.ile<=0) return;
    int i=0,pos=  width * xt_ax.poz( xt_ax.podzialka_min);
    double p=xt_ax.podzialka_min;
    while( pos<width ){
       cr->move_to( widthL + pos, heightTt);
       cr->line_to( widthL + pos, heightT );
       if(xt_ax.grid) rysuj_gridX(cr,pos);
       cr->move_to( widthL + pos, heightTt);
       cr->show_text( xt_ax.to_string(p));
       i+=1;
       p = xt_ax.podzialka_min+i*xt_ax.podzialka;
       pos = width * xt_ax.poz(p);
    }
    cr->stroke();
}
//-------------------------------------------------
void atChart::rysuj_osYL(const Cairo::RefPtr<Cairo::Context>& cr){
    if(yl_ax.auto_ax) yl_ax.podzialka_auto();
    cr->set_line_width(yl_ax.line_width);
    cr->set_source_rgb(yl_ax.color_r, yl_ax.color_g,yl_ax.color_b);
    cr->move_to( widthL, heightT);
    cr->line_to( widthL, heightT+height );
    if(yl_ax.ile<=0) return;
    int i=0,pos=height * yl_ax.poz(yl_ax.podzialka_min);
    double p=yl_ax.podzialka_min;
    while( pos>0 ){
       cr->move_to(widthLt,heightT + pos);
       cr->line_to(widthL ,heightT + pos);
       if(yl_ax.grid) rysuj_gridY(cr,pos);
       cr->move_to(0      ,heightT + pos);
       cr->show_text( yl_ax.to_string(p));

       i+=1;
       p =yl_ax.podzialka_min+i*yl_ax.podzialka;
       pos = height * yl_ax.poz(p);
    }
    cr->stroke();
}
//-------------------------------------------------
void atChart::rysuj_osYR(const Cairo::RefPtr<Cairo::Context>& cr){
    if(yr_ax.auto_ax) yr_ax.podzialka_auto();
    cr->set_line_width(yr_ax.line_width);
    cr->set_source_rgb(yr_ax.color_r, yr_ax.color_g,yr_ax.color_b);
    cr->move_to( widthL+width, heightT);
    cr->line_to( widthL+width, heightT+height );
    if(yr_ax.ile<=0) return;
    int i=0,pos= height * yr_ax.poz(yr_ax.podzialka_min);
    double p=yr_ax.podzialka_min;
    while( pos>0 ){
       cr->move_to(width+widthL               , heightT + pos);
       cr->line_to(width+widthL+widthR-widthRt, heightT + pos);
       if(yr_ax.grid) rysuj_gridY(cr,pos);
       cr->move_to(width+widthL+widthR-widthRt, heightT + pos);
       cr->show_text( yr_ax.to_string(p));
       i+=1;
       p = yr_ax.podzialka_min+i*yr_ax.podzialka;
       pos = height * yr_ax.poz(p);
    }
    cr->stroke();
}
//-------------------------------------------------
void atChart::rysuj_punkt(atSeria* s,const Cairo::RefPtr<Cairo::Context>& cr){
    if(s->symbol==PUNKT_SYMBOL_BRAK) return;
    int pozy = height * s->pozY();
    int pozx = width * s->pozX();
    cr->move_to(widthL + pozx,heightT + pozy);

    if(s->symbol==PUNKT_SYMBOL_JEDEN){ cr->close_path();  }
    else if(s->symbol==PUNKT_SYMBOL_DWA_POZ){ cr->rel_line_to(0,1);  }
    else if(s->symbol==PUNKT_SYMBOL_DWA_PION){ cr->rel_line_to(1,0);  }
    else if(s->symbol==PUNKT_SYMBOL_CZTERY){ cr->rel_line_to(1,0);cr->rel_line_to(0,1);cr->rel_line_to(-1,0);cr->rel_line_to(0,-1);  }
    else if(s->symbol==PUNKT_SYMBOL_PIEC){ cr->rel_line_to(1,0);cr->rel_line_to(-1,1);cr->rel_line_to(-1,-1);cr->rel_line_to(1,-1);cr->rel_line_to(1,1);  }
    else if(s->symbol==PUNKT_SYMBOL_SZESC_POZ){ cr->rel_line_to(1,0);cr->rel_line_to(0,1);cr->rel_line_to(-2,0);cr->rel_line_to(0,-1);cr->rel_line_to(1,0);  }
    else if(s->symbol==PUNKT_SYMBOL_SZESC_PION){ cr->rel_line_to(0,1);cr->rel_line_to(1,0);cr->rel_line_to(0,-2);cr->rel_line_to(-1,0);cr->rel_line_to(0,1);  }

    cr->close_path();
    cr->stroke();
    s->punkt++;
    return;
}
//--------------------------------------------------
void atChart::setSeria(atSeria *s,unsigned xax,unsigned yax){
    serie.push_back(s);
    if(xax==DRAWAREA_AXIXS_T){ s->xax=&xt_ax; } else{ s->xax=&xb_ax; }
    if(yax==DRAWAREA_AXIXS_R){ s->yax=&yr_ax; } else{ s->yax=&yl_ax; }
}
//-------------------------------------------------
void atChart::rysuj_gridX(const Cairo::RefPtr<Cairo::Context> &cr, int pos){
    const std::valarray<double>& dashes={10,5};
    cr->move_to(widthL + pos, height+heightT);
    //cr->set_line_width(yr_ax.line_width);
    cr->set_dash (dashes, 10);
    cr->line_to(widthL + pos, heightT);
    cr->unset_dash();
    return;
}
//-------------------------------------------------
void atChart::rysuj_gridY(const Cairo::RefPtr<Cairo::Context> &cr, int pos){
    const std::valarray<double>& dashes={10,5};
    cr->move_to(widthL ,heightT + pos);
    cr->set_dash (dashes, 10);
    cr->line_to(widthL+width ,heightT + pos);
    cr->unset_dash();
    return;
}
//-------------------------------------------------
void atChart::force_redraw(){
  auto win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
}



//-------------------------------------------------------------------
//   SERIA
//-------------------------------------------------------------------
void atSeria::add(double x,double y){
    xax->setMax(x);
    yax->setMax(y);
    X.push_back(x);
    Y.push_back(y);
}


//-------------------------------------------------------------------
//   AXIS
//-------------------------------------------------------------------
void atAxis::podzialka_auto(){
    if(!auto_ax ) return;
    size=max-min;
    double d0=1,d1=1;
    while (d0>size){ d1=d0; d0*=0.1;}
    while (d1<size){ d0=d1; d1*=10; }
    d1=d0*2;
    while (d1<size){ d1+=d0; }
    d0=d1-d0;
    podzialka=d0/10;
    //--------------------
    int i=min/podzialka;
    podzialka_min= i*podzialka;
    if(podzialka_min>min) podzialka_min-=podzialka;
}
//---------------------------
double atAxis::poz(double p){
    if(odwrocenie) return (1-p/size+podzialka_min/size);
    return p/size-podzialka_min/size;
}
//---------------------------
string atAxis::to_string(double p){
    char ret[50];
    sprintf(ret,formatT.c_str(),p);
    return static_cast<string>(ret);
}
//---------------------------
void atAxis::setMax(double x){
    if(ile<=0){ min=x; max=x; }
    else{
        if(x<min) min=x;
        if(x>max) max=x;
    }
    ile++;
}
