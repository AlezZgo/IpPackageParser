#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFileDialog>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_chooseButton_clicked();
    void readSigFile();
    void initContainersOfKeysAndValues();
    void initIpAddressStats();
    bool isIP(int strPos);
    QVector<quint8> getAddress(int str);

    bool inDictionary(QVector<quint8> arr );
    bool dictElemIsEqualToSupposed(QVector<quint8> dictElement, QVector<quint8> supposed);
    void setResultToUI();
    QString getString(QVector<quint8> vec);
    void saveByteArraysToFiles();
    void saveToFile(QByteArray arr, QString fileName);
    QByteArray getByteArrayByPackageIndexes(QVector<int> indexes);
    QByteArray getByteArrayByPackageIndex(int index);
    QByteArray correctFirstAndSecondBytes(QByteArray arr);


private:
    Ui::MainWindow *ui;
    QByteArray massIn;

    QHash<QVector<quint8>,QVector<int>> dictAddressToIndexes;

    QMap<float,float> mapKeys;
    QMap<float,float> mapSizes;
    QMap<float,float> map;
};
#endif // MAINWINDOW_H
