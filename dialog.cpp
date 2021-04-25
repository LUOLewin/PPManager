#include "dialog.h"
#include "ui_dialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QVector<QString> string;
    string.append(ui->lineEdit->text());
    string.append(ui->lineEdit_2->text());
    string.append(ui->lineEdit_3->text());

    QSqlDatabase db = QSqlDatabase::database("myDatabase.db");
    QSqlQuery query(db);

    query.exec("select * from myAccount");
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    int nRecordCount = queryModel->rowCount();

    query.prepare(QString("insert into myAccount values(?, ?, ?, ?)"));
    query.bindValue(0, nRecordCount);
    query.bindValue(1, string[0]);
    query.bindValue(2, string[1]);
    query.bindValue(3, string[2]);
    query.exec();

    Dialog::close(); // 窗口关闭
}
