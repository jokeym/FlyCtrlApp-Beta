#ifndef MAG_H
#define MAG_H

#include <QWidget>

namespace Ui {
class Mag;
}

class Mag : public QWidget
{
    Q_OBJECT

public:
    explicit Mag(QWidget *parent = 0);
    ~Mag();

private:
    Ui::Mag *ui;
};

#endif // MAG_H
