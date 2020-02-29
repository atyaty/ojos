#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <QtGui>

class CQtOpenCVViewerGl : public QWidget
{
private:
   bool mSceneChanged; /// Indicates when OpenGL view is to be redrawn

   QImage mRenderQtImg; /// Qt image to be rendered
   cv::Mat mOrigImage; /// original OpenCV image to be shown

   QColor mBgColor; /// Background color

   int mOutH; /// Resized Image height
   int mOutW; /// Resized Image width
   float mImgratio; /// height/width ratio

   int mPosX; /// Top left X position to render image in the center of widget
   int mPosY; /// Top left Y position to render image in the center of widget

public:
    CQtOpenCVViewerGl();

    void imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

    void initializeGL(); /// OpenGL initialization
    void paintGL(); /// OpenGL Rendering
    void resizeGL(int width, int height); /// Widget Resize Event

    void updateScene(); /// Forces a scene update
    void renderImage(); /// Render image on openGL frame

 public slots:
    bool showImage( cv::Mat image ); /// Used to set the image to be viewed

};

#endif // CQTOPENCVVIEWERGL_H
