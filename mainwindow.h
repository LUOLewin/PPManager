#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //QStringListModel *Model;
    QStandardItemModel *ItemModel;
    void init();

private:
    Ui::MainWindow *ui;

private slots:
    void on_listView_clicked(const QModelIndex &index); // listview 项目点击
    void on_lineEdit_returnPressed(); // 搜索栏回车
    void on_action_triggered(); // 主界面菜单栏点击
    void updateMainWindow(); // 更新主界面
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void menuDelete(); // listView 右键菜单删除
    void menuUpdate(); // listView 右键菜单修改
};
#endif // MAINWINDOW_H
