#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_Diapazon_activated(int index);

    void on_RunrasX_clicked();

    void on_RunrasS_clicked();

    void on_RunrasX_2_clicked();

    void on_RunTsux_clicked();

    void on_RejDAC_currentIndexChanged(int index);

    void on_RasDAC_clicked();

    void on_RusSuperSpeed_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
