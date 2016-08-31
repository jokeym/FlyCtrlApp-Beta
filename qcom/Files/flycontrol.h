#ifndef FLYCONTROL_H
#define FLYCONTROL_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
//#include <QtOpenGL>
#include <QtOpenGL/QtOpenGL>


#define GL_PI 3.1415926
#define GL_RADIUX  0.2f

namespace Ui {
class FlyControl;
}

class FlyControl : public QWidget
{
    Q_OBJECT
    
public:
    explicit FlyControl(QWidget *parent = 0);
    ~FlyControl();

    void LoadPicture(void);
    void updatePic(void);
    void RotatePic(void);


    
private slots:


private:
    Ui::FlyControl *ui;
};

#endif // FLYCONTROL_H
