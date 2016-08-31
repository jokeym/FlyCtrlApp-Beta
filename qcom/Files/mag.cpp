#include "mag.h"
#include "ui_mag.h"

#include "Files/ConfigData.h"


extern U30_BARO U30_Baro;


Mag::Mag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mag)
{
    ui->setupUi(this);
//    this->show();
    this->setWindowFlags(Qt::SplashScreen);

}

Mag::~Mag()
{
    delete ui;
}
