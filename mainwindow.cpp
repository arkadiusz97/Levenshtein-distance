#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewMatrix->setModel(&model);
    ui->tableViewMatrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->pushButtonNext, &QPushButton::clicked, &loop, &QEventLoop::quit);
    connect(this, &MainWindow::elementChanged, this, &MainWindow::insertField);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::distanceLevenshtein(QString a, QString b)
{
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonNext->setEnabled(true);
    for(int i = 0; i <= a.length(); ++i)
        matrix.push_back(QVector<int>(b.length() + 1));
    for(int i = 0; i <= a.length(); i++)
    {
        matrix[i][0] = i;
        emit elementChanged(i, 0, i);
        loop.exec();
    }
    for(int j = 1; j <= b.length(); j++)
    {
        matrix[0][j] = j;
        emit elementChanged(0, j, j);
        loop.exec();
    }
    for(int i = 1; i <= a.length(); i++)
    {
        for(int j = 1; j <= b.length(); j++)
        {
            int cost = matrix[i - 1][j - 1];
            if(b[j - 1] !=  a[i - 1] ) cost++;
            matrix[i][j] = qMin( qMin( matrix[i - 1][j] + 1, matrix[i][j - 1] + 1), cost);
            emit elementChanged(i, j, matrix[i][j]);
            loop.exec();
        }
    }
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonNext->setEnabled(false);
    ui->labelDistanceLevenshtein->setText("Distance: " + QString::number(matrix[a.length()][b.length()]));
    ui->labelSimilarity->setText("Similarity: " + QString::number(1.0/(1.0 + matrix[a.length()][b.length()])));
}

void MainWindow::on_pushButtonStart_clicked()
{
    start();
    distanceLevenshtein(ui->lineEditString1->text(), ui->lineEditString2->text());
}

void MainWindow::start()
{
    ui->labelDistanceLevenshtein->clear();
    ui->labelSimilarity->clear();
    matrix.clear();
    model.clear();
    model.setRowCount(ui->lineEditString1->text().length());
    model.setColumnCount(ui->lineEditString2->text().length());
    labels.clear();
    labels.push_back("");
    for(auto i : ui->lineEditString1->text())
        labels.push_back(i);
    model.setVerticalHeaderLabels(labels);
    labels2.clear();
    labels2.push_back("");
    for(auto i : ui->lineEditString2->text())
        labels2.push_back(i);
    model.setHorizontalHeaderLabels(labels2);
}

void MainWindow::insertField(int i, int j, int val)
{
    QStandardItem *item = new QStandardItem;
    item->setText(QString::number(val));
    model.setItem(i, j, item);
}
