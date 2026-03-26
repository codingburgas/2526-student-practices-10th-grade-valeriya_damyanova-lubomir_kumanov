#include "menu.h"
#include "../BLL/BLL.h" 
#include <QMessageBox>

Menu::Menu(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuClass)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Menu::on_btnTest_clicked);

}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_btnTest_clicked()
{
    BLLClass bll;
    QString result = QString::fromStdString(bll.process());
    QMessageBox::information(this, "architecture", result);
}