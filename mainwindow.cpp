#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    colorMin(QColor(0, 255, 0)), colorNotMin(QColor(255, 0, 0)), colorCurrentCell(QColor(255, 255, 0))
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
        clearColors();
        setColor(i, 0, colorCurrentCell);
        if(!ui->checkBoxCalculateEverything->isChecked())
            loop.exec();
    }
    for(int j = 1; j <= b.length(); j++)
    {
        matrix[0][j] = j;
        emit elementChanged(0, j, j);
        clearColors();
        setColor(0, j, colorCurrentCell);
        if(!ui->checkBoxCalculateEverything->isChecked())
            loop.exec();
    }
    for(int i = 1; i <= a.length(); i++)
    {
        for(int j = 1; j <= b.length(); j++)
        {
            int cost = matrix[i - 1][j - 1];
            if(b[j - 1] !=  a[i - 1])
            {
                cost++;
                ui->labelCost->setText("Cost: 1");
            }
            else
                ui->labelCost->setText("Cost: 0");
            matrix[i][j] = qMin( qMin( matrix[i - 1][j] + 1, matrix[i][j - 1] + 1), cost);
            emit elementChanged(i, j, matrix[i][j]);
            clearColors();
            setColor(i, j, colorCurrentCell);
            if(matrix[i - 1][j] + 1 < matrix[i][j - 1] + 1 && matrix[i - 1][j] + 1 < cost)//One minimum.
            {
                setColor(i - 1, j, colorMin);
                setColor(i, j - 1, colorNotMin);
                setColor(i - 1, j - 1, colorNotMin);
            }
            else if(matrix[i][j - 1] + 1 < matrix[i - 1][j] + 1 && matrix[i][j - 1] + 1 < cost)//One minimum.
            {
                setColor(i - 1, j, colorNotMin);
                setColor(i, j - 1, colorMin);
                setColor(i - 1, j - 1, colorNotMin);
            }
            else if(cost < matrix[i][j - 1] + 1 && cost < matrix[i - 1][j] + 1)//One minimum.
            {
                setColor(i - 1, j, colorNotMin);
                setColor(i, j - 1, colorNotMin);
                setColor(i - 1, j - 1, colorMin);
            }
            else if(cost == matrix[i][j - 1] + 1 &&
                    cost == matrix[i - 1][j] + 1 &&
                    matrix[i - 1][j] + 1 == matrix[i][j - 1] + 1)//Three minimums.
            {
                setColor(i - 1, j, colorMin);
                setColor(i, j - 1, colorMin);
                setColor(i - 1, j - 1, colorMin);
            }
            else if(cost == matrix[i][j - 1] + 1 && cost < matrix[i - 1][j] + 1)//Two minimums.
            {
                setColor(i - 1, j, colorNotMin);
                setColor(i, j - 1, colorMin);
                setColor(i - 1, j - 1, colorMin);
            }
            else if(cost == matrix[i - 1][j] + 1 && cost < matrix[i][j - 1] + 1)//Two minimums.
            {
                setColor(i - 1, j, colorMin);
                setColor(i, j - 1, colorNotMin);
                setColor(i - 1, j - 1, colorMin);
            }
            else if(matrix[i][j - 1] + 1 == matrix[i - 1][j] + 1 && matrix[i][j - 1] + 1 < cost)//Two minimums.
            {
                setColor(i - 1, j, colorMin);
                setColor(i, j - 1, colorMin);
                setColor(i - 1, j - 1, colorNotMin);
            }
            if(!ui->checkBoxCalculateEverything->isChecked())
                loop.exec();
        }
    }
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonNext->setEnabled(false);
    clearColors();
    ui->labelDistanceLevenshtein->setText("Distance: " + QString::number(matrix[a.length()][b.length()]));
    ui->labelSimilarity->setText("Similarity: " + QString::number(1.0/(1.0 + matrix[a.length()][b.length()])));
    ui->labelCost->setText("");
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

void MainWindow::setColor(int i, int j, QBrush color)
{
    model.item(i, j)->setBackground(color);
}

void MainWindow::clearColors()
{
    for(int k = 0; k < model.rowCount(); ++k)
        for(int l = 0; l < model.columnCount(); ++l)
            if(model.item(k, l) != nullptr)
                model.item(k, l)->setBackground(QBrush(QColor(255, 255, 255)));
}
