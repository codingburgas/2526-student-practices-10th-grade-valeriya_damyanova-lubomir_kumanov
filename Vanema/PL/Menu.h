#pragma once
#include <QMainWindow>
#include "ui_Menu.h" 

namespace Ui {
    class MenuClass;
}
class Menu : public QMainWindow
{
    Q_OBJECT

public:
    Menu(QWidget* parent = nullptr);
    ~Menu();
private:
    Ui::MenuClass* ui;
private slots:
    void on_btnTest_clicked(); 
};