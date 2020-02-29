/*
by AT
*/
#include "atDevice.h"





//--------------------------------------------------------
void atDevice::SetParams(int n){
	IniWe(n);
	aParts.resize(0);
	Error=NULL;
	Part_Visible=true;
	ButtSS_Visible=false;
	EditWidth=-1;
	EditWeWidth=-1;
	EditWy_prec=-1;
	EditWy_digits=-1;
	enabled=false;
	ParentDev=NULL;
	ParentPartNo=-1;
	SetWy(0);
	EveOnUp=NULL;
	EveOnDn=NULL;
	VirtualWeChecked=false;
	sType="dev";
}

//--------------------------------------------------------
//petla po składowych
//zaczynając od ostatniej, bo pierwsza jest najblizej wyjścia
void atDevice::GoParts(){
  bool err=false;
  string s="";
  for( int i=aParts.size()-1; i>=0; i-- ){
   aParts[i]->Go();
   if(aParts[i]->GetError()!=NULL){err=true; s.append("\r\n"); s.append(aParts[i]->GetError()->Opis()); }
  }
  if(err) SetError(atDevice_ERROR_Parts_Error,NULL,s);
}
//--------------------------------------------------------
//petla po składowych dla funkcji ExtFun
//zaczynając od ostatniej, bo pierwsza jest najblizej wyjścia
void atDevice::ExtFunParts(int j){
  bool err=false;
  string s="";
  for( int i=aParts.size()-1; i>=0; i-- ){
   aParts[i]->ExtFun(j);
   if(aParts[i]->GetError()!=NULL){err=true; s.append("\r\n"); s.append(aParts[i]->GetError()->Opis()); }
  }
  if(err)  SetError(atDevice_ERROR_Parts_Error,NULL,s);
}




//--------------------------------------------------------
//zwraca numer wejścia
int atDevice::WeNo(string id){
  for( unsigned i=0; i<aIdWe.size(); i++ ) if( aIdWe.at(i)==id && aWe.at(i)!=NULL ) return i;
  return -1;
}
//--------------------------------------------------------
//podaje numer wejścia do którego podłączony jest obiekt d
//jeśli nie jest podłączone zwraca -1
int atDevice::WeNo(atDevice *d){
  for( unsigned i=0; i<aWe.size(); i++ ) if( aWe.at(i)==d ) return i;
  return -1;
}
//--------------------------------------------------------
// sprawdza czy device d jest podłączony do naszego objektu
// uwzględnia nasze wejścia bezpośrednio oraz podłączenia do naszych Partsów  (rekurencyjnie)
//uwzględnia tylko bezpośrednio wyjście objektu d
bool atDevice::IsStrictConnected(atDevice *d){
  if(WeNo(d)>=0) return true ;
  for( unsigned i=0; i<aParts.size(); i++ ) if( aParts.at(i)->IsStrictConnected(d) ) return true;
  return false;
}
//--------------------------------------------------------
// sprawdza czy device d jest podłączony do naszego objektu
// uwzględnia bezpośrednie wyjście obiektu d oraz wyjścia jego Partsów (rekurencyjnie)
bool atDevice::IsConnected(atDevice *d){
  if(IsStrictConnected(d)) 	return true ;
  for( unsigned i=0; i<d->SizeParts(); i++ ) if( IsConnected(d->Part(i)) ) 	return true;
  return false;
}
//--------------------------------------------------------
// przeszukujemy device d na okoliczność naszej do niego przynależności
// sprawdzamy najpierw wprost czy my nie jesteśmy objektem d, a nastepnie (rekurencyjnie) czy nie jesteśmy jednym z jego Partsów
bool atDevice::IsItOrPart(atDevice *d){
  if( this==d )	return true ;
  for( unsigned i=0; i<d->SizeParts(); i++ )    if( IsItOrPart(d->Part(i)) ) 	return true;
  return false;
}
//--------------------------------------------------------
// przeszukujemy wejścia wszystkich Partsów (rekurencyjnie)
// jeśli device stojący w wejściu nie pochodzi z naszego objektu (znaczy połączenie jest zewnętrzne), to dopisujemy go do naszej tablicy aVirtualWe
// w funkcji głównej jest czyszczenie tablicy wirtualnych wejść
// nie ma też przeszukiwania własnych wejść, bo one z definicji są zewnętrznie podłączone
//
// metoda jest używana do rysowania połączeń w TDevicePanel
void atDevice::findVirtualWe( ){
	aVirtualWe.clear();
	for(unsigned i=0;i<SizeParts();i++){	findVirtualWe( Part(i) );	}
	VirtualWeChecked=true;
}
//--------------------------------------------------------
// funkcja podrzędna przeszukuje partsy w objekcie d przekazanym jako parametr (rekurencyjnie)
// oraz przeszukuje wejścia przekazanego devica, sprawcza czy poszczególne wejścia (ich żródła) przynależą do naszego objektu
// znaczy czy połącznie jest wewnętrzne
// tylko zewnętrzne połączenia dodawane są do aVirtualWe
//  
void atDevice::findVirtualWe( atDevice *d ){
	for(unsigned i=0;i<d->SizeParts();i++){	findVirtualWe( d->Part(i) );	}
	for(unsigned i=0;i<d->SizeWe();i++){
		atDevice * dwej=d->We(i);
		if( dwej!=NULL && !dwej->IsItOrPart(this) && WeNo(dwej)<0){
//		if( dwej!=NULL && !dwej->IsItOrPart(this) ){
			aVirtualWe.push_back(dwej);
		}
	}
}
//--------------------------------------------------------
//zwraca objekt wejscia o identyfikatorze id
atDevice * atDevice::We(string id){ int i=WeNo(id); if(i>=0) return aWe.at(i); else return NULL; }
//--------------------------------------------------------
//sprawdza czy istnieje wejście i czy jest wolne
bool atDevice::WeEmpty(string id){
  for( unsigned i=0; i<aIdWe.size(); i++ ) if( aIdWe.at(i)==id && aWe.at(i)==NULL ) return true;
  return false;
}
//--------------------------------------------------------
//zwraca numer składowej o identyfikatorze id
int atDevice::PartNo(string id){
  for( unsigned i=0; i<aParts.size(); i++ ){
   if( aParts.at(i)!=NULL && aParts.at(i)->GetId()==id )
	 return i;
  }
  return -1;
}
//--------------------------------------------------------
//jeśli objekt d jest rodzicem lub dziadkiem naszym
//to zwracamy numer pozycji pod którą u tego dziadka występujemy (w dziadkowym aParts)
//w przeciwnym wypadku zwracamy -1
//int atDevice::getParentPartNo(atDevice *d){
//  if(ParentDev==NULL) return -1; //jeśli nie mamy rodzica to zwracamy -1
//  if(ParentDev==d) return ParentPartNo; // jeśli d jest naszym rodzicem to zwracamy ParentPartsNo
//  return ParentDev->getParentPartNo(d); //jeśli d nie jest rodzicem to sprawdzamy u naszego rodzica
//}
//--------------------------------------------------------
//zwraca składową o identyfikatorze id
atDevice * atDevice::Part(string id){
 int i=PartNo(id);
 if(i>=0)
  return aParts.at(i);
 else
  return NULL;
}
//--------------------------------------------------------
//standardowa funkcja wyjscia
//- jeśli są podzespoły to: na wyjściu jest powtórzona wartość wyjścia zerowego podzespołu
//- jeśli nie ma podzespołów to: na wyjściu jest wsawiana watość NAND z wejść [brak wejścia (czyli: aWe.at(i)==NULL)traktowany jest jak 0 na wejściu]
//- jeśli nie ma wejść to: na wyjsciu jest zero
//funkcja ta jest wirtualna i może byc podmieniona na dowolną inną
double atDevice::GoFun(){
   if( aParts.size()>0 ){
    
	GoParts();
	return aParts.at(0)->GetWy();
   }
   else if(aWe.size()>0){ for( unsigned i=0; i<aWe.size(); i++ ) if( aWe.at(i)==NULL || aWe.at(i)->GetWy()==0.0) return 1.0;   }
   return 0.0;
}
//---------------------------------------------------------
//standardowa funkcja zewnętrzna
//uruchamia funkcje zewnętrzne w podzespołach (z parametrem takim jak sama dostala)
//jednocześnie przestawia znacznik enabled w zależnosci od parametru
void atDevice::ExtFun(int j){
 ExtFunParts(j);
 SetEnabled(j>0);
}
//---------------------------------------------------------
void  atDevice::SetError(int e, string op, int ech, int buf, string he){
 if(Error!=NULL) return; // przeniosłem kontrolę tu żeby wyświetlany był pierwszy bład jaki się pojawi a nie ostatni
 string s="dev:"; s.append(Id); //s.append("="); s.append(sNazwa);
 if(he!=""){s.append(":");s.append(he);}
 Error=new atError(e,op,ech,buf,s);
 AnsiString sE ="\r\nDevice Error: "+DateTimeToStr(Now())+ " " + Error->Opis().c_str();
 Rap.Write(sE);
}

//---------------------------------------------------------
//podłączamy do wejścia "i" urządzenie "d"
bool atDevice::connect(atDevice *d, unsigned i) {
 if( i>=nWe) { SetError(atDevice_ERROR_connect_nWe,""); return false; }
 if( aWe.at(i)!=NULL) {
  string s="Wejście: "; s.append(int2str(i)); s.append(" device: "); s.append(d->sNazwa);
  SetError(atDevice_ERROR_connect_RECONNENT,NULL,s);
  return false;
 }
 aWe.at(i)=d;
 if(aConnDev.at(i)>=0)	 aParts.at(aConnDev.at(i))->connect(d,aConnWe.at(i));
 return true;
}
//---------------------------------------------------------
//podłączmy do wejścia idWe urządzenie "d"
bool atDevice::connect(atDevice *d, string idWe) {
 int nWe=WeNo(idWe);
 if(nWe>=0) return connect(d,nWe); else return false;
}
//--------------------------------------------------------
bool atDevice::connect(atDevice *d, unsigned nP,  unsigned nW){
 if( d->SizeParts()>nP ){   return connect( d->Part(nP), nW); }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
bool atDevice::connect(atDevice *d, string idP,  unsigned nW){
 atDevice *dP=d->Part(idP);
 if( dP!=NULL ){ return connect( dP, nW); }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
bool atDevice::connect(atDevice *d, string idP,  string idW){
 atDevice *dP=d->Part(idP);
 if( dP!=NULL ){ return connect( dP, idW); }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}

//--------------------------------------------------------
//metoda dodaje składowe niepodłączane urządzenia.
//wyjscie pierwszej dodanej skladowej jest jednocześnie wyjsciem urządzenia.
//pierwsza dodawana składowa musi byc niepodłączana, bo nie ma jej do czego podłączyć
bool atDevice::addPart(atDevice *d){
 aParts.push_back(d);
 d->ParentDev=this;
 d->ParentPartNo=aParts.size()-1;
 return true;
}
//--------------------------------------------------------
//metoda dodaje składowe podłączane urzadzenia
//składowa po dodaniu łączona jest z odpowiednim istniejącym juz urządzeniem
bool atDevice::addPart(atDevice *d, unsigned nDev, unsigned nWe){
 if(aParts.size()<=nDev || aParts.at(nDev)->SizeWe()<=nWe || aParts.at(nDev)->We(nWe)!=NULL){
	SetError(atDevice_ERROR_addParts,""); return false;
 }
 addPart(d);
 aParts.at(nDev)->connect(d,nWe);
 return true;
}
//--------------------------------------------------------
//metoda dodaje składowe podłączane urzadzenia
//składowa po dodaniu łączona jest z odpowiednim istniejącym juz urządzeniem
bool atDevice::addPart(atDevice *d, string idDev, string idWe){
 atDevice *d1=Part(idDev);
 if(d1!=NULL && d1->WeEmpty(idWe)){
   addPart(d);
   return d1->connect(d,idWe);
 }
 SetError(atDevice_ERROR_addParts,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje składowe podłączane urzadzenia
//składowa po dodaniu łączona jest z odpowiednim istniejącym juz urządzeniem
bool atDevice::addPart(atDevice *d, string idDev, unsigned nWe){
 int nDev=PartNo(idDev);
 if(nDev>=0) return addPart(d,nDev,nWe);
 SetError(atDevice_ERROR_addParts,"");
 return false;
}
//---------------------------------------------------------
//podłączenie wejscia (n) do wejścia któregoś z istniejacych składowych
bool atDevice::addWe(unsigned n, unsigned nD, unsigned nW) {
 if( n<nWe && aConnDev.at(n)<0 &&
	 nD<aParts.size() && nW<aParts.at(nD)->SizeWe() &&
	 aParts.at(nD)->We(nW)==NULL  ){
   aIdWe.at(n)=Part(nD)->GetId();
   aConnDev.at(n)=nD;
   aConnWe.at(n)=nW;
   return true;
 }
 SetError(atDevice_ERROR_addWe,"");
 return false;
}
//---------------------------------------------------------
//podłączenie wejscia (n) do wejścia którejś z istniejacych składowych (idD, idW)
//dodanie łączy sie z nadaniem nazy wejściu
bool atDevice::addWe(unsigned n, string idD, string idW) {
 int nD=PartNo(idD);
 if( nD>=0){
  unsigned nW=Part(idD)->WeNo(idW);
  return addWe(n,nD,nW);
 }
 SetError(atDevice_ERROR_addWe,"");
 return false;
}
//---------------------------------------------------------
//podłączenie wejscia (n) do wejścia którejś z istniejacych składowych (idD, idW)
//dodanie łączy sie z nadaniem nazy wejściu
bool atDevice::addWe(unsigned n, string idD, unsigned nW) {
 int nD=PartNo(idD);
 if( nD>=0) return addWe(n,nD,nW);
 SetError(atDevice_ERROR_addWe,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje połaczenie składowej nD0 do wejscia nW składowej nD1
bool atDevice::addConnection(unsigned nD0, unsigned nD1, unsigned nW){
 if( nD0>nD1 && nD0<aParts.size() && nD1<aParts.size()  ){
   return aParts.at(nD1)->connect( aParts.at(nD0), nW);
 }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje połaczenie składowej idD0 do wejscia idW składowej idD1
bool atDevice::addConnection(string idD0, string idD1, string idW){
 int nD0=PartNo(idD0);
 int nD1=PartNo(idD1);
 if(nD0>=0 && nD1>=0){
  int nW=Part(idD1)->WeNo(idW);
  return addConnection(nD0,nD1,nW);
 }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje połaczenie składowej idD0 do wejscia idW składowej idD1
bool atDevice::addConnection(string idD0, string idD1, unsigned nW){
 int nD0=PartNo(idD0);
 int nD1=PartNo(idD1);
 if(nD0>=0 && nD1>=0)  return addConnection(nD0,nD1,nW);
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje połaczenie pomiędzy składową nP składowej nD0
//do wejscia nW składowej nD1
//czyli podłączamy sie do jakiegoś punktu wewnatrz składowej D0
bool atDevice::addConnection(unsigned nD0, unsigned nP,  unsigned nD1, unsigned nW){
 if( nD0>nD1 && nD0<aParts.size() && nD1<aParts.size() && nP<aParts.at(nD0)->SizeParts() ){
   return aParts.at(nD1)->connect( aParts.at(nD0)->Part(nP), nW);
 }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje połaczenie pomiędzy składową nP składowej nD0
//do wejscia nW składowej nD1
//czyli podłączamy sie do jakiegoś punktu wewnatrz składowej D0
bool atDevice::addConnection(string idD0, string idP,  string idD1, string idW){
 int nD0=PartNo(idD0);
 int nD1=PartNo(idD1);
 if(nD0>=0 && nD1>=0){
  unsigned nP=Part(idD0)->PartNo(idP);
  unsigned nW=Part(idD1)->WeNo(idW);
  return addConnection(nD0,nP,nD1,nW);
 }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
//metoda dodaje połaczenie pomiędzy składową nP składowej nD0
//do wejscia nW składowej nD1
//czyli podłączamy sie do jakiegoś punktu wewnatrz składowej D0
bool atDevice::addConnection(string idD0, string idP,  string idD1, unsigned nW){
 int nD0=PartNo(idD0);
 int nD1=PartNo(idD1);
 if(nD0>=0 && nD1>=0){
  unsigned nP=Part(idD0)->PartNo(idP);
  return addConnection(nD0,nP,nD1,nW);
 }
 SetError(atDevice_ERROR_addConnection,"");
 return false;
}
//--------------------------------------------------------
//uruchamiacz od zdarzeń
//EveStatus wymusza uruchomienie tylko przy zmianie Wyjścia (przejście przez zero)
//wskaźniki *_One wymuszają skasowanie zdarzenia po jego wykonaniu
void atDevice::GoEve(){
	if(Wy>0 && !EveStatus){
		EveStatus=true;
		if (EveOnUp!=NULL){
			  EveFun ef=EveOnUp;
			  if(EveOnUp_One)EveOnUp=NULL;
			  ef();  //po to jest ten numer ze zmienna "ef" żeby ta funkcja mogła ustawić zdarzenie (to którym sama była) po raz kolejny
		}
	}
	if(Wy<=0 && EveStatus){
		EveStatus=false;
//		if( EveOnDn!=NULL){ EveOnDn(); if(EveOnDn_One)EveOnDn=NULL; } //tak było, jeśli funkcja nadpisała "swoje" zdarzenie to nic to nie dawało bo od razu było zNULLowane
		if( EveOnDn!=NULL){
			 EveFun ef=EveOnDn;
			 if(EveOnDn_One)EveOnDn=NULL;
			 ef(); //po to jest ten numer ze zmienną "ef" żeby ta funkcja mogła ustawić zdarzenie (to którym sama była) po raz kolejny
		}
	}
}





//==============================================================
//funkcje GoFun klas dziedziczonych
double atDeviceOnOff::GoFun(){ return Enabled()?1.0:0.0; }
void atDeviceOnOff::ExtFun(int i){
   if(i<=0) SetEnabled(false);
   else if(i==1) SetEnabled(true);
   else SetEnabled(!Enabled());
}

//==============================================================
double atDeviceAND::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  for( unsigned i=0; i<aWe.size(); i++ ) if( aWe.at(i)==NULL || aWe.at(i)->GetWy()==0.0) return 0.0;
  return 1.0;
}
//==============================================================
double atDeviceOR::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  for( unsigned i=0; i<aWe.size(); i++ ){
	if( aWe.at(i)!=NULL && aWe.at(i)->GetWy()!=0.0)
	  return 1.0;
  }
  return 0.0;
}
//==============================================================
double atDeviceNOR::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  for( unsigned i=0; i<aWe.size(); i++ ) if( aWe.at(i)!=NULL &&  aWe.at(i)->GetWy()!=0.0) return 0.0;
  return 1.0;
}
//==============================================================
double atDeviceNAND::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  for( unsigned i=0; i<aWe.size(); i++ ) if( aWe.at(i)==NULL || aWe.at(i)->GetWy()==0.0) return 1.0;
  return 0.0;
}
//==============================================================
double atDeviceTrue::GoFun(){ return 1.0; }
//==============================================================
double atDeviceFalse::GoFun(){ return 0.0; }
//==============================================================
double atDeviceSum::GoFun(){
  if(!Enabled()) return 0.;
  if( aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  for( unsigned i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) sum+=aWe.at(i)->GetWy();
	else SetError(atDevice_ERROR_WeNotConected,"");
  return sum;
}
//==============================================================
double atDeviceInv::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  for( unsigned i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) sum+=aWe.at(i)->GetWy();
	else SetError(atDevice_ERROR_WeNotConected,"");
  return -sum;
}
//==============================================================
double atDeviceRoz::GoFun(){
  if(aWe.size()<=1){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  if( aWe.at(0)!=NULL ) sum+=aWe.at(0)->GetWy();
  else SetError(atDevice_ERROR_WeNotConected,"");
  for( unsigned i=1; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) sum-=aWe.at(i)->GetWy();
	else SetError(atDevice_ERROR_WeNotConected,"");
  return sum;
}
//==============================================================
double atDeviceMno::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double mul=1.0;
  for( unsigned i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) mul*=aWe.at(i)->GetWy();
	else SetError(atDevice_ERROR_WeNotConected,"");
  return mul;
}
//==============================================================
double atDevicePot::GoFun(){
  if(aWe.size()<2){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double mul=1.;
//  double podst=10.;
//  if( aWe.at(0)!=NULL ) podst=aWe.at(0)->GetWy();
  if( aWe.at(0)!=NULL ) mul=aWe.at(0)->GetWy();
  if( aWe.at(1)!=NULL ) mul*=pow( 10., aWe.at(1)->GetWy() );
  return mul;
}
//==============================================================
double atDeviceOdw::GoFun(){
  if(aWe.size()<=0){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double mul=1.0;
  for( unsigned i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) mul*=aWe.at(i)->GetWy();
	else SetError(atDevice_ERROR_WeNotConected,"");
  if(mul!=0.) return 1./mul;
  else return 0.;
}
//==============================================================
double atDeviceDzi::GoFun(){
  if(aWe.size()<=1){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  if( aWe.at(0)!=NULL ) sum=aWe.at(0)->GetWy();
  else SetError(atDevice_ERROR_WeNotConected,"");
  for( unsigned i=1; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ){
	 if( aWe.at(i)->GetWy()!=0.0) sum/=aWe.at(i)->GetWy();
	 else sum=0.;
	}
	else  SetError(atDevice_ERROR_WeNotConected,"");
  return sum;
}
//==============================================================
double atDeviceAbs::GoFun(){
  if( aWe.at(0)!=NULL ) return fabs( aWe.at(0)->GetWy() );
  return 0.;
}
//==============================================================
double atDeviceSemafor::GoFun(){    //powtarza na wyjściu wartość z we0 o ile we1!=0
  if( aWe.size()<2 ){SetError(atDevice_ERROR_ZaMaloWejsc,""); return 0.;};
  if( aWe.at(0)!=NULL && aWe.at(1)!=NULL && aWe.at(1)->GetWy()!=0. )  return aWe.at(0)->GetWy();
  return 0.;
}
//==============================================================
double atDeviceSem::GoFun(){    //powtarza na wyjściu wartość z we0 o ile we1!=0
  if( aWe.at(we_nr)!=NULL ) return aWe.at(we_nr)->GetWy();
  else SetError( atDevice_ERROR_WeNotConected ,"");
  return 0.;
}
//---------------------------------------------------------------
void atDeviceSem::SetFun(int j){
 if( j<0 || aWe.size()<=(unsigned)j ){ SetError(atDevice_ERROR_ZaMaloWejsc,""); }
 else { we_nr=j; Go();}
}
//---------------------------------------------------------------
string  atDeviceSem::GetOpis(){
 AnsiString s="N="+IntToStr(we_nr);
 return s.c_str();
}

//==============================================================
//funkcje GoFun klas dziedziczonych DI
void atDeviceDI::SetBuf(PCBuffer *buf,unsigned bn){
 if(buf!=NULL && buf->GetType()==PCBUFF_TYPE_DI ){  PCBuf=buf;  BN=bn; PCBuf->AddDevice(this,BN); Go(); }
 else{ PCBuf=NULL; BN=0; SetError(atDevice_ERROR_BadBuffor,""); }
}
//---------------------------------------------------------------
double atDeviceDI::GoFun(){
 double r=0.0;
 if (PCBuf!=NULL){
  r=PCBuf->GetBitValue(BN);
  SetError( PCBuf->GetError() );
 }
 return r;
}
//---------------------------------------------------------------
string  atDeviceDI::GetOpis(){
 if(PCBuf==NULL) return "DI-NULL" ;
 AnsiString s=PCBuf->GetId()+", DI-"+PCBuf->BufferNo()+" ("+IntToStr(BN)+")";
 return s.c_str();
}
//---------------------------------------------------------------
TColor  atDeviceDI::GetColor(){  if(PCBuf==NULL) return NULL ; return PCBuf->GetHardwareColor(); }
//---------------------------------------------------------------
//funkcje klas dziedziczonych DI - Blok
atDeviceDIBlock::atDeviceDIBlock(PCBuffer *buf){
 PCBuf=buf;
 AnsiString s="DI-"+buf->BufferNo();
 SetNazwa(s.c_str());
 for(unsigned i=0; i<buf->GetLen(); i++)
   addPart(new atDeviceDI(buf,i,int2str(i)));
 IniWe(0);
 Go();
}
//---------------------------------------------------------------
atDeviceDIBlock::~atDeviceDIBlock(){
 for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
}

//==============================================================
//funkcje GoFun klas dziedziczonych DO
//---------------------------------------------------------------
void atDeviceDO::SetBuf(PCBuffer *buf,unsigned bn){
 if(buf!=NULL && buf->GetType()==PCBUFF_TYPE_DO ){  PCBuf=buf;  BN=bn; PCBuf->AddDevice(this,BN); }
 else{ PCBuf=NULL; BN=0; SetError(atDevice_ERROR_BadBuffor,""); }
}
//---------------------------------------------------------------
double atDeviceDO::GoFun(){
 if(PCBuf!=NULL){
	if(aWe.at(0)!=NULL ){
		 PCBuf->SetBitValue(BN,aWe.at(0)->GetWy()==0.0? 0:1);
		 SetError(PCBuf->GetError());
	}
	return PCBuf->GetBitValue(BN)?1.0:0.0;
 }
 return 0.0;
}
//---------------------------------------------------------------
string  atDeviceDO::GetOpis(){
 if(PCBuf==NULL) return "DO-NULL" ;
 AnsiString s=PCBuf->GetId()+", DO-"+PCBuf->BufferNo()+" ("+IntToStr(BN)+")";
 return s.c_str();
}
//---------------------------------------------------------------
TColor  atDeviceDO::GetColor(){  if(PCBuf==NULL) return NULL ; return PCBuf->GetHardwareColor(); }

//---------------------------------------------------------------
//funkcje klas dziedziczonych DO - Blok
atDeviceDOBlock::atDeviceDOBlock(PCBuffer *buf){
 PCBuf=buf;
 AnsiString s="DO-"+buf->BufferNo();
 SetNazwa(s.c_str());
 IniWe(buf->GetLen());
 for(unsigned i=0; i<buf->GetLen(); i++){
  addPart(new atDeviceDO(buf,i,int2str(i)));
  addWe(i,i,0);
 }
}
//---------------------------------------------------------------
atDeviceDOBlock::~atDeviceDOBlock(){
 for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
}

//==============================================================
//Ustawia Wyjście w zależności od funkcji zewnetrznej
//funkcje GoFun klas dziedziczonych DO
//double atDeviceDOSwitch::GoFun(){ PCBuf->SetBitValue(BN,on?1:0); return PCBuf->GetBitValue(BN)?1.0:0.0; }
//void atDeviceDOSwitch::ExtFun(int i){on=!on; Go();}
//==============================================================
//Ustawia Wyjście w zależności od funkcji zewnetrznej
//funkcje GoFun klas dziedziczonych DO
//double atDeviceDOOnOff::GoFun(){ PCBuf->SetBit(BN,on?1:0); return PCBuf->GetBit(BN)?1.0:0.0; }
//void atDeviceDOOnOff::ExtFun(int i){ on=(i!=0?true:false); Go();}

//---------------------------------------------------------------
//funkcje klas dziedziczonych DO - Blok
//atDeviceDOSwitchBlock::atDeviceDOSwitchBlock(PCBuffer *buf){
 //PCBuf=buf;
// AnsiString s="DO-"+buf->BufferNo()+" Sw";
// SetNazwa(s.c_str());
// for(unsigned i=0; i<buf->GetLen(); i++){
//  addPart(new atDeviceDOSwitch(buf,i,int2str(i)));
 //}
//}
//---------------------------------------------------------------
//atDeviceDOSwitchBlock::~atDeviceDOSwitchBlock(){
// for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
//}



//==============================================================
//funkcje GoFun klas dziedziczonych AD
void atDeviceAD::SetBuf(PCBuffer *buf){
 if(buf!=NULL && buf->GetType()==PCBUFF_TYPE_AI ){  PCBuf=buf;  PCBuf->AddDevice(this); }
 else{ PCBuf=NULL; SetError(atDevice_ERROR_BadBuffor,""); }
}
//---------------------------------------------------------------------------
double atDeviceAD::GoFun(){
 if(PCBuf==NULL){ SetError(atDevice_ERROR_AIBuff_NULL,""); return 0; }
 SetError(PCBuf->GetError());
 return PCBuf->GetDoubleValue();
}
//---------------------------------------------------------------
string  atDeviceAD::GetOpis(){
 if(PCBuf==NULL) return "AI-NULL" ;
 AnsiString s=PCBuf->GetId()+", AI-"+PCBuf->BufferNo();
 return s.c_str();
}
//---------------------------------------------------------------
TColor  atDeviceAD::GetColor(){  if(PCBuf==NULL) return NULL ; return PCBuf->GetHardwareColor(); }


//==============================================================
//
atDeviceTara::atDeviceTara(string s,string id):atDevice(s,id){ SetType("devTara");EditWidth=50; IniWe(1); tara=0.0;}
double atDeviceTara::GoFun(){ return (aWe.at(0)!=NULL) ?  aWe.at(0)->GetWy()-tara : 0.0;}
void atDeviceTara::SetFun(int i){ tara=(i==1 && aWe.at(0)!=NULL)? aWe.at(0)->GetWy() :0.0; Go();}

//==============================================================
//
atDeviceCalc::atDeviceCalc(string tr,string s,string id):atDevice(s,id){ Tr=new atTrace(tr); Tr->Wczytaj(); SetDev(); }
atDeviceCalc::atDeviceCalc(string tr,string kX,string kY,string s,string id):atDevice(s,id){ Tr=new atTrace(tr); Tr->Wczytaj(kX,kY); SetDev();}
//---------------------------------------------------------------
void atDeviceCalc::SetDev(){
 SetType("devCalc");
 EditWidth=50;
 EditWeWidth=50;
 IniWe(1);
 if(Tr->GetN()<=0){
	string s="atDeviceCalc, Empty trace: "; s.append(Tr->GetFileName()); s.append(" N="); s.append(IntToStr(Tr->GetN()).c_str());
	SetError(atDevice_ERROR_Trace_Empty,NULL,s);
 }
 Go();
}
//---------------------------------------------------------------
atDeviceCalc::~atDeviceCalc(){delete Tr;atDevice::~atDevice();}
//---------------------------------------------------------------
double atDeviceCalc::GoFun(){ return (Tr->GetN()>0 && aWe.at(0)!=NULL) ? Tr->GetY( aWe.at(0)->GetWy() ) : 0.0;}
//---------------------------------------------------------------
string  atDeviceCalc::GetOpis(){ return Tr->GetFileName(); }

/*
//==============================================================
//funkcje GoFun klas dziedziczonych ADCalc
atDeviceADCalc::atDeviceADCalc(PCAIBuffer *buf,string tr,string s,string id){
 IniPP();
 IniWe(0);
 PCBuf=buf;
 SetDev(tr);
 SetNazwa(s,id);
}
//---------------------------------------------------------------
void atDeviceADCalc::SetDev(string tr){
 Tr=new atTrace(tr);
 Tr->Wczytaj();
 if(Tr->GetN()<=0){
   string s="atDeviceADCalc, Empty trace: "; s.append(Tr->GetFileName()); s.append(" N="); s.append(IntToStr(Tr->GetN()).c_str()) ;
   SetError(atDevice_ERROR_Trace_Empty,NULL,s);
 }
 Go();
}
//---------------------------------------------------------------
atDeviceADCalc::~atDeviceADCalc(){delete Tr;atDevice::~atDevice();}
//---------------------------------------------------------------
double atDeviceADCalc::GoFun(){return (Tr->GetN()>0) ? Tr->GetY( PCBuf->GetValue() ) : 0.0;}
*/
//==============================================================
//funkcje GoFun klas dziedziczonych AD
//double atDeviceTemp::GoFun(){ return PCBuf->Get_Chann();}

//---------------------------------------------------------------
//funkcje klas dziedziczonych AD - Blok
atDeviceADBlock::atDeviceADBlock(PCCard *pcc){
 int n=pcc->GetAIChann();
 if(n>0){
   PCBuf=new PCAIBuffer * [n];

   SetNazwa("AD");
   for(int i=0; i<n; i++){
	 PCBuffer *p=pcc->GetAIChannel(i);
	 addPart(new atDeviceAD(p,p->BufferNo().c_str()));
   }
   IniWe(0);  Go();
 }
}
//---------------------------------------------------------------
atDeviceADBlock::~atDeviceADBlock(){
 for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
}



//==============================================================
double atDeviceMathSrednia::GoFun(){
 if( en && aWe.at(0)==NULL){
  sum += aWe.at(0)->GetWy();
  N++;
  if(N==Nmax){ sred=sum/N; ready=true; sum=0.; N=0;}
 }
 return sred;
}
//---------------------------------------------------------------
void atDeviceMathSrednia::ExtFun(int i){ en=(i==0?false:true); ready=false; sred=0.; sum=0.; N=0; }
//==============================================================
// znajduje maksymalną z wejścia
double atDeviceMathMax::GoFun(){
 if( Enabled() && aWe.at(0)!=NULL ){
  if(!started) {max=aWe.at(0)->GetWy(); started=true;}
  else if( aWe.at(0)->GetWy()>max ) max=aWe.at(0)->GetWy();
  return max;
 }
 return 0;
}
void atDeviceMathMax::ExtFun(int i){ atDevice::ExtFun(i); started=false; max=0; }
//==============================================================
// znajduje maksymalną z wejścia
double atDeviceMathMin::GoFun(){
 if( Enabled() && aWe.at(0)!=NULL ){
  if(!started) {min=aWe.at(0)->GetWy(); started=true;}
  else if( aWe.at(0)->GetWy()<min ) min=aWe.at(0)->GetWy();
  return min;
 }
 return 0;
}
void atDeviceMathMin::ExtFun(int i){ atDevice::ExtFun(i); started=false; min=0; }




//==============================================================
// Detektor przełamania trendu :)
double atDeviceBreakDetector::GoFun(){
 if( aWe.at(0)==NULL || stat==0 ) return 0;
 if(stat==1) { t1=t0=aWe.at(0)->GetWy(); stat=2; }
 else {
   double trend_old=t1-t0;
   double trend_new=aWe.at(0)->GetWy()-t1;
   if( trend_new!=0. ){
	t0=t1;
	t1=aWe.at(0)->GetWy();
	if(znak_ok(trend_old,trend_new)) n++;
	else if( n<3 ) n=0;
	else stat=0;
   }
 }


 return stat;
}
void atDeviceBreakDetector::ExtFun(int i){
 if(i!=0){ stat=1; t0=0.; t1=0.; n=0;} //inicjowanie pomiaru
 else {stat=0;}                        //reset pomiaru
}


//==============================================================
//funkcje GoFun klas dziedziczonych DA
//---------------------------------------------------------------
void atDeviceDA::SetBuf(PCBuffer *buf){
 EditWidth=50;
 EditWeWidth=50;
 ButtSS_Visible=true;
 if( buf!=NULL ){  PCBuf=buf;  PCBuf->AddDevice(this); }
 else{ PCBuf=NULL; SetError(atDevice_ERROR_BadDABuffor,""); }
}
//---------------------------------------------------------------
double atDeviceDA::GoFun(){
 if(aWe.at(0)!=NULL && PCBuf!=NULL){
	 PCBuf->SetDoubleValue(	   aWe.at(0)->GetWy() );
	 SetError(PCBuf->GetError());
	 return aWe.at(0)->GetWy();
 }
 return 0;
}
//---------------------------------------------------------------
string  atDeviceDA::GetOpis(){
 if(PCBuf==NULL) return "AO-NULL" ;
 AnsiString s=PCBuf->GetId()+", AO-"+PCBuf->BufferNo();
 return s.c_str();
}
//---------------------------------------------------------------
TColor  atDeviceDA::GetColor(){  if(PCBuf==NULL) return NULL ; return PCBuf->GetHardwareColor(); }


//---------------------------------------------------------------
//funkcje klas dziedziczonych DA - Blok
atDeviceDABlock::atDeviceDABlock(PCCard *pcc){
 int n=pcc->GetAOChann();
 if(n>0){
   PCBuf=new PCBuffer * [n];
   SetNazwa("DA");
   for(int i=0; i<n; i++){
	 PCBuffer *p=pcc->GetAOChannel(i);
	 addPart(new atDeviceDA(p,p->BufferNo().c_str()));
   }
   IniWe(0);  Go();
 }
}
//---------------------------------------------------------------
atDeviceDABlock::~atDeviceDABlock(){
 for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
}

//==============================================================
double atDeviceKroki::GoFun(){
 if( Enabled()){
	if( Kroki>0 ) {
		Kroki--; }
	else if( nWe>0 && aWe.at(0)!=NULL ){
		if( we_last == 0. && aWe.at(0)->GetWy()!=0. ) {  //przejście z 0 na 1 inicjuje licznik
			Kroki=K;
		}
		we_last= aWe.at(0)->GetWy();
	}
	return Kroki;
 }
 return 0;
}
void atDeviceKroki::SetFun(int i){ Kroki=i; }

