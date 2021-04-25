#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked(); // 插入子界面确定按钮点击

private:
    Ui::Dialog *ui;

signals:
    void closeEvent(QCloseEvent *event); // 插入子界面关闭事件

};

#endif // DIALOG_H
