#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
cv::VideoCapture mCapture;
cv::VideoCapture mCapture1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(DisplayImage()));
    Timer->start(50); //Will start the timer


    //-----------------------------kamery, oczy
    //kamera 0----------------------------------
    cam0=new at_kamera(512,384,25./25.,25./25.);
    cam0->BlurXs=9;
    cam0->BlurZs=9;
    cam0->BlurXdev=2;
    cam0->BlurZdev=2;
    cam0->CircleInversRat=1;
    cam0->CircleminDist=8; //dzielnik
    cam0->CircleParam1=60;
    cam0->CircleParam2=40;
    cam0->CircleminR=0;
    cam0->CirclemaxR=0;
    cam0->Cannythres1=50;
    cam0->Cannythres2=200;
    cam0->Cannyaperture=3;
    cam0->Linerho=1;
    cam0->Linetheta=180; //dzielnik liczby pi
    cam0->Linethres3=80;
    cam0->LineminLen=30;
    cam0->LineminGap=10;
    //kamera 1----------------------------------
    cam1=new at_kamera(512,384,25./27.,25./27.);
    cam1->BlurXs=9;
    cam1->BlurZs=9;
    cam1->BlurXdev=1.5;
    cam1->BlurZdev=1.5;
    cam1->CircleInversRat=1;
    cam1->CircleminDist=8; //dzielnik
    cam1->CircleParam1=60;
    cam1->CircleParam2=40;
    cam1->CircleminR=0;
    cam1->CirclemaxR=0;
    cam1->Cannythres1=50;
    cam1->Cannythres2=200;
    cam1->Cannyaperture=3;
    cam1->Linerho=1;
    cam1->Linetheta=180; //dzielnik liczby pi
    cam1->Linethres3=80;
    cam1->LineminLen=30;
    cam1->LineminGap=10;
    //------------------------------------------
    oczy=new at_oczy(cam0,cam1,19.);

    omamy=new at_omamy(10);

    rap = new atFile("../log","logfile",".csv" );
    rap->AppendLn("id;T;X;Y;Z;sX;sY;sZ;vX;vY;vZ;m0X;m1X;m0sX;m1sX;m0Xtg;m1Xtg;");

    if( mCapture.isOpened() || mCapture.open( 0 ) )   {cam0->setOpen();}
    if( mCapture1.isOpened() || mCapture1.open( 1 ) ) {cam1->setOpen();}
    //-----------------------------

    //lista do wyświetlenia omamów
    model = new QStandardItemModel( 0, 1, this );
    ui->listView->setModel(model);




}

//-------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
    delete cam0;
    delete cam1;
    delete oczy;
    delete omamy;
    rap->ZapiszPlik();
    delete rap;
}

//-------------------------------------------------------------------------
// to jest funkcja do analizy pojedynczego stereoskopowego obrazu
// pobieramy obraz z dwóch kamer i poddajemy go analizie
//
// jeśli dana akamera jest czynna, zapisujemy obraz z niej na zmienną
// potem ten obraz skalujemy do rozdzielczości w której lubimy pracować
// zamieniamy jeszcze kolory do standardy QT żeby to wyświetlić (puki co robimy to tylko po to żeby na ekranie ładnie sie pokazywało, dla procedury nie ma to znaczenia)
// ostatnim krokiem (DetectMajaki) jest znalezienie 'majaków" czyli charakterystycznych punktów obrazu na obrazkach
//
// zmienne (tablice) m0 i m1 służą do przechowywania majaków z poszczególnych kamer (postać "wektorowa", czyli współrzędne położenia, rozmiary, kolor, itp)
// mając wektorową informację o tym co widać z dwóch punktów wyznaczamy położenie obiektów w trzech wymiarach - funkcja: omamy->Add()
//
void MainWindow::DisplayImage(){

    //---------------------------------------------------------------------------------
    // KAMERA 0
    //---------------------------------------------------------------------------------
    QVector<at_majak> m0;   // tablice majaków dla kamery 0, tu będą zapisane wektorowe wersje zidentyfikowanych elementów obrazu
    Mat img;                //zmienna na obraz oryginalnie pobrany z kamery
    if( cam0->IsOpened() ){
        //img = imread("/fat/projekty/ojos/program/openCVproj.png"); //to jest ewentualny odczyt z pliku - tylko próba
        mCapture >> img;                                                            //przechwyt z kamery 0 na zmieną img
        cv::resize(img, img, Size(cam0->getX(), cam0->getZ()), 0, 0, INTER_LINEAR); //zmieniamy rozmiar
        cv::cvtColor(img,img,CV_BGR2RGB);                                           //Qt reads in RGB whereas CV in BGR (żeby ładnie wyświetlić)
        DetectMajaki(&img,&m0,cam0);                                                //detekcja majaków w obrazie kamery
    }

    //---------------------------------------------------------------
    // KAMERA 1
    //---------------------------------------------------------------
    QVector<at_majak> m1;
    Mat img1;
    if( cam1->IsOpened() ){
        mCapture1 >> img1;
        cv::resize(img1, img1, Size(cam1->getX(), cam1->getZ()), 0, 0, INTER_LINEAR);  //zmieniamy rozmiar
        cv::cvtColor(img1,img1,CV_BGR2RGB);                                            //Qt reads in RGB whereas CV in BGR
        DetectMajaki(&img1,&m1,cam1);
    }

    //Canny( img, img, cam0->Cannythres1, cam0->Cannythres2, cam0->Cannyaperture );
    //cv::cvtColor( img, img, CV_RGB2GRAY );       // detekcje kółek robimy na szarym rastrze
    //cv::cvtColor( img, img, CV_GRAY2RGB );       // detekcje kółek robimy na szarym rastrze
    //GaussianBlur( img, img, Size(cam0->BlurXs, cam0->BlurZs), cam0->BlurXdev, cam0->BlurZdev ); // Reduce the noise so we avoid false circle detection

    if(m0.size()>0){
        at_majak mm0=m0.at(0);
        ui->lineEdit_m0X->setText(QString::number( mm0.get_X() ));
        ui->lineEdit_m0sX->setText(QString::number( mm0.getsX() ));
        ui->lineEdit_m0Z->setText(QString::number( mm0.get_Z() ));
    }
    if(m1.size()>0){
        at_majak mm1=m1.at(0);
        ui->lineEdit_m1X->setText(QString::number( mm1.get_X() ));
        ui->lineEdit_m1sX->setText(QString::number( mm1.getsX() ));
        ui->lineEdit_m1Z->setText(QString::number( mm1.get_Z() ));
    }

    RysujMajaki(&img,&m0);          //ryujemy na obrazku znalezione majaki, żeby zobaczyc co zostało wykryte - tylko pomocnicze działanie
    RysujMajaki(&img1,&m1);         //...

    omamy->Add(&m0, &m1, oczy);     //majaki zamieniamy na trójwymiarowe omamy - to jest sedno systemu - do tablicy omamów dodajemy kolejne w oparciu dwuwymiarowe objekty z dwóch kamer

    RysujOmamy(&img,&img1);        //...

    //---------------------------------------------------------------------------
    //to jest do wypisywania omamów
    model->clear();
    for(int i=0; i<omamy->getSize(); i++){
        at_omam* o_act=omamy->getOmam(i);
        QString sstr = "[ " + QString::number(i)+","+QString::number(o_act->getId())+ " ] "+QString::number(o_act->getN());
        sstr+="( X="+QString::number( o_act->getX() );
        sstr+=", Y="+QString::number( o_act->getY() );
        sstr+=", Z="+QString::number( o_act->getZ() )+" )";
        QStandardItem *item = new QStandardItem(QString("Idx ") + sstr);
        model->appendRow(item);
    }
    //-----------------------------------------------------------------------------

    //--------------------------------------------
    //a to jest tylko wyświetlenie obrazków na ekranie -pomocnicze
    QImage imdisplay((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888); //Converts the CV image into Qt standard format
    QImage imdisplay1((uchar*)img1.data, img1.cols, img1.rows, img1.step, QImage::Format_RGB888); //Converts the CV image into Qt standard format
    ui->label_img->setPixmap(QPixmap::fromImage(imdisplay));//display the image in label that is created earlier
    ui->label_img1->setPixmap(QPixmap::fromImage(imdisplay1));//display the image in label that is created earlier

}

//-------------------------------------------------------------------------
//
void MainWindow::RysujMajaki(Mat* img, QVector<at_majak>* m){

    for(int i=0; i<m->size();i++){
        at_majak mm=m->at(i);
        if(mm.IsCircle())  circle( *img, Point(cvRound(mm.getX()), cvRound(mm.getZ())), cvRound(mm.getsX()), Scalar(0,0,255), 3, 8, 0 );
        else if(mm.IsLine()) line( *img, Point(cvRound(mm.getX()), cvRound(mm.getZ())), Point( cvRound(mm.getsX()+mm.getX()),cvRound(mm.getsZ()+mm.getZ())), Scalar(255,0,0), 3, 8 );
    }
}

//-------------------------------------------------------------------------
//
void MainWindow::RysujOmamy(Mat* img0,Mat* img1){

    for(int i=0; i<omamy->getSize();i++){
        at_omam* o=omamy->getOmam(i);
        at_kamera* c0=o->getOczy()->getCam0();
        at_kamera* c1=o->getOczy()->getCam1();

        int x0 =c0->get_X()*(o->getX0tg()/c0->getXtg()+1);
        int sx0=c0->get_X()*(o->getsXtg()/c0->getXtg());
        int z0 =c0->get_Z()*(o->getZtg()/c0->getZtg()+1);

        int x1 =c1->get_X()*(o->getX1tg()/c1->getXtg()+1);
        int sx1=c1->get_X()*(o->getsXtg()/c1->getXtg());
        int z1 =c1->get_Z()*(o->getZtg()/c1->getZtg()+1);

        if(o->IsCircle()){
            circle( *img0, Point(x0, z0), sx0, Scalar(255,255,0), 3, 8, 0 );
            circle( *img1, Point(x1, z1), sx1, Scalar(255,255,0), 3, 8, 0 );
        }
//        else if(o.IsLine()) line( *img, Point(cvRound(mm.getX()), cvRound(mm.getZ())), Point( cvRound(mm.getsX()+mm.getX()),cvRound(mm.getsZ()+mm.getZ())), Scalar(255,0,0), 3, 8 );
    }
}

//-------------------------------------------------------------------------
//
void MainWindow::DetectMajaki(Mat* img, QVector<at_majak>* m0, at_kamera* cam0){
    //----------------------------------------------kółka----------------------------
    Mat gray;
    cv::cvtColor( *img, gray, CV_RGB2GRAY );       // detekcje kółek robimy na szarym rastrze
    GaussianBlur( gray, gray, Size(cam0->BlurXs, cam0->BlurZs), cam0->BlurXdev, cam0->BlurZdev ); // Reduce the noise so we avoid false circle detection
    vector<Vec3f> circles;                        // kółka wektorowo - tablice do przechowywania obiektów zidentyfikowanych w rastrze - prosty format narzucony przez openCV
    HoughCircles( gray, circles, CV_HOUGH_GRADIENT, cam0->CircleInversRat, gray.rows/cam0->CircleminDist, cam0->CircleParam1, cam0->CircleParam2, cam0->CircleminR, cam0->CirclemaxR );    // Apply the Hough Transform to find the circles (progi oryginalne:  200,100)
    for( size_t i = 0; i < circles.size(); i++ ) { m0->push_back(at_majak(cam0,circles[i][0],circles[i][1],circles[i][2]));          }
    //--------------------------------------------linie------------------------------------
    /*
    Mat dst;
    Canny( *img, dst, cam0->Cannythres1, cam0->Cannythres2, cam0->Cannyaperture );
    vector<Vec4i> lines;
    HoughLinesP( dst, lines, cam0->Linerho, CV_PI/cam0->Linetheta, cam0->Linethres3, cam0->LineminLen, cam0->LineminGap );
    for( size_t i = 0; i < lines.size(); i++ ) { m0->push_back(at_majak(cam0,lines[i][0],lines[i][1],lines[i][2],lines[i][3]));        }
    */
}
