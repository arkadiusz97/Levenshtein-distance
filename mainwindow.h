#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QEventLoop>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void distanceLevenshtein(QString a, QString b);
signals:
    void elementChanged(int i, int j, int val);
private slots:
    void on_pushButtonStart_clicked();
    void insertField(int i, int j, int val);
    void start();
    void setColor(int i, int j, QBrush color);
    void clearColors();
private:
    Ui::MainWindow *ui;
    QStandardItemModel model;
    QEventLoop loop;
    QVector<QVector<int> >matrix;
    QStringList labels, labels2;
    QBrush colorMin, colorNotMin, colorCurrentCell;
};

#endif // MAINWINDOW_H
