#ifndef DIALOG_2_H
#define DIALOG_2_H

#include <QDialog>

namespace Ui {
class Dialog_2;
}

class Dialog_2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_2(QWidget *parent = nullptr);
    ~Dialog_2();
    QString id;

private:
    Ui::Dialog_2 *ui;

signals:
    void closeEvent(QCloseEvent *event); // 修改子界面关闭事件

private slots:
    void on_pushButton_clicked();
};

#endif // DIALOG_2_H
