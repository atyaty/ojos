#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QMainWindow>
#include <QListView>
#include <QStandardItemModel>

//#include "opencv/highgui.h"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2\core\core.hpp"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"




#include "at_oczy.h"
#include "at_majak.h"
#include "at_omamy.h"
#include "atFile.h"

using namespace cv;
using namespace std;

namespace Ui {    class MainWindow;     }


//------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void DisplayImage();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage imdisplay,imgdisplay1;
    QTimer* Timer;

private:
    Ui::MainWindow *ui;
    at_kamera *cam0;
    at_kamera *cam1;
    at_oczy* oczy;
    at_omamy* omamy;
    atFile *rap;
    QStandardItemModel *model;

    void RysujMajaki( Mat* img, QVector<at_majak>* m0);
    void RysujOmamy(Mat* img0,Mat* img1);
    void DetectMajaki(  Mat* img,QVector<at_majak>* m0, at_kamera* cam0);

};

#endif // MAINWINDOW_H
