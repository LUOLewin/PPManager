#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "dialog_2.h"

#include <QDebug>
#include <QTextFrame>
#include <QVector>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

QSortFilterProxyModel* m_proxyModel;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    updateMainWindow();
}


MainWindow::~MainWindow()
{
    delete ui;
}


/*
 * 初始化函数
 */
void MainWindow::init()
{
    //数据库初始化
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myDatabase.db");
    if(!db.open())
    {
        QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
    }

    QStringList strList; // listView名称列表
    QSqlQuery query;
    query.exec("create table myAccount (id int primary key,"
               "company varchar(20),"
               "user varchar(20),"
               "password varchar(20))");
}


/*
 * 列表点击事件
 */
void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    ui->textEdit->clear();

    QSqlDatabase db = QSqlDatabase::database("myDatabase.db");
    QSqlQuery query(db);

    query.prepare("select * from myAccount where company = ?"); // 注意这个带条件的select
    query.bindValue(0, m_proxyModel->itemData(ui->listView->selectionModel()->selectedIndexes()[0])[0].toString());
    query.exec();
    query.next();

    // 使用html格式显示详细账户信息
    // 注意观察如何在QString的html中插入变量
    QString str = "<font size='4' face='Verdana'"
            "<table border='5' cellpadding='30'>"
            "<li>"
            "<span>user:</span>"
            "<span style='font-family:arial;color:red;font-size:20px'>"+query.value(2).toString()+"</span>"
            "</li>"
            "<li>"
            "<span>password:"
            "<span style='font-family:arial;color:red;font-size:20px'>"+query.value(3).toString()+"</span>"
            "</li>";

    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertHtml(str);
}


/*
 * 搜索栏Enter按下
 */
void MainWindow::on_lineEdit_returnPressed()
{
    m_proxyModel->setFilterFixedString(ui->lineEdit->text()); //根据字符串过滤
}


/*
 * 菜单栏:信息插入
 */
void MainWindow::on_action_triggered()
{
    Dialog *messageWindow = new Dialog(this);
    connect(messageWindow, &Dialog::closeEvent, this, &MainWindow::updateMainWindow); // 连接子界面关闭信号和主界面更新槽
    messageWindow->exec();
}


/*
 * 主界面更新槽
 */
void MainWindow::updateMainWindow()
{
    QSqlDatabase db = QSqlDatabase::database("myDatabase.db");
    QSqlQuery query(db);
    QStringList strList; // listView名称列表

    query.exec("select * from myAccount");
    while(query.next())
    {
        strList.append(query.value(1).toString());
    }

    ItemModel = new QStandardItemModel(this);
    strList.sort(); // 升序排列listView名称列表
    int nCount = strList.size();

    for(int i=0; i<nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        item->setBackground(QBrush(QColor(224,240,233))); // listView中item颜色设置
        ItemModel->appendRow(item);
    }

    m_proxyModel = new QSortFilterProxyModel(ui->listView);
    m_proxyModel->setSourceModel(ItemModel);
    m_proxyModel->setFilterRole(Qt::DisplayRole); //设置控件过滤对象
    m_proxyModel->setDynamicSortFilter(true);

    ui->listView->setModel(m_proxyModel);           //为委托设置模型
    ui->listView->setSpacing(5);                   //为视图设置控件间距
    ui->listView->setDragEnabled(false);            //控件不允许拖动
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止修改listview子项名称
    ui->listView->setProperty("contextMenuPolicy", Qt::CustomContextMenu);
}


/*
 * listView 菜单请求
 */
void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);

    if(!((ui->listView->selectionModel()->selectedIndexes()).empty()))
    {
        QMenu *menu = new QMenu(ui->listView);
        QAction *delAction = new QAction("删除", this);
        QAction *updateAction = new QAction("修改", this);

        menu->addAction(delAction);
        menu->addAction(updateAction);
        connect(delAction, &QAction::triggered, this, &MainWindow::menuDelete);
        connect(updateAction, &QAction::triggered, this, &MainWindow::menuUpdate);
        menu->exec(QCursor::pos());
    }
}


/*
 * listView 右键菜单删除
 */
void MainWindow::menuDelete()
{
    QSqlDatabase db = QSqlDatabase::database("myDatabase.db");
    QSqlQuery query(db);

    query.prepare("delete from myAccount where company = ?");
    query.bindValue(0, m_proxyModel->itemData(ui->listView->selectionModel()->selectedIndexes()[0])[0].toString()); // 获取删除的文本信息，注意如何获取listview的文本信息
    query.exec();

    updateMainWindow();
}


/*
 * listView 右键菜单修改
 */
void MainWindow::menuUpdate()
{
    Dialog_2 *messageWindow = new Dialog_2(this);
    QLineEdit *lineEdit = messageWindow->findChild<QLineEdit *>("lineEdit"); // 获取子窗口的部件
    QLineEdit *lineEdit_2 = messageWindow->findChild<QLineEdit *>("lineEdit_2");
    QLineEdit *lineEdit_3 = messageWindow->findChild<QLineEdit *>("lineEdit_3");

    QSqlDatabase db = QSqlDatabase::database("myDatabase.db");
    QSqlQuery query(db);

    // 通过company查找相关信息显示在lineEdit上
    query.prepare("select * from myAccount where company = ?"); // 注意这个带条件的select
    query.bindValue(0, m_proxyModel->itemData(ui->listView->selectionModel()->selectedIndexes()[0])[0].toString());
    query.exec();
    query.next();
    messageWindow->id = query.value(0).toString();
    lineEdit->setText(query.value(1).toString());
    lineEdit_2->setText(query.value(2).toString());
    lineEdit_3->setText(query.value(3).toString());

    connect(messageWindow, &Dialog_2::closeEvent, this, &MainWindow::updateMainWindow); // 连接子界面关闭信号和主界面更新槽
    messageWindow->exec();
}
