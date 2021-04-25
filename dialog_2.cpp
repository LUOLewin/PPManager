#include "dialog_2.h"
#include "ui_dialog_2.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>

Dialog_2::Dialog_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_2)
{
    ui->setupUi(this);
}

Dialog_2::~Dialog_2()
{
    delete ui;
}

void Dialog_2::on_pushButton_clicked()
{
    QVector<QString> string;
    string.append(ui->lineEdit->text());
    string.append(ui->lineEdit_2->text());
    string.append(ui->lineEdit_3->text());

    QSqlDatabase db = QSqlDatabase::database("myDatabase.db");
    QSqlQuery query(db);

    query.prepare("update myAccount set company = ?, user = ?, password = ? where id = ?");
    query.bindValue(0, string[0]);
    query.bindValue(1, string[1]);
    query.bindValue(2, string[2]);
    query.bindValue(3, id);
    query.exec();

    Dialog_2::close(); // 窗口关闭
}
