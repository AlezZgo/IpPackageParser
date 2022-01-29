#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QDir>
#include <QSaveFile>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_chooseButton_clicked()
{

    readSigFile();

    initContainersOfKeysAndValues();

    initIpAddressStats();

    setResultToUI();

    saveByteArraysToFiles();

}

void MainWindow::saveByteArraysToFiles(){
    QHashIterator<QVector<quint8>,QVector<int>> currAddressToIndexes(dictAddressToIndexes);
    while (currAddressToIndexes.hasNext()) {
        currAddressToIndexes.next();
        QByteArray package = getByteArrayByPackageIndexes(currAddressToIndexes.value());
        saveToFile(package, getString(currAddressToIndexes.key()) + ".sig");

    }
}

void MainWindow::saveToFile(QByteArray arr, QString fileName){

    QSaveFile *saveFile = new QSaveFile(fileName);
    saveFile->open(QIODevice::WriteOnly);
    saveFile->write(arr);
    saveFile->commit();

}

QByteArray MainWindow::getByteArrayByPackageIndexes(QVector<int> indexes){
    QByteArray result;
    foreach(int index, indexes){
        result.append(getByteArrayByPackageIndex(index-1));
    }
    return result;
}

QByteArray MainWindow::getByteArrayByPackageIndex(int index){
    QByteArray result;
    int from = mapKeys.value(index);
    int size = mapSizes.value(index)+2;

    for (int i=from;i<from+size;i++) {
        result.append((quint8)massIn[i]);
    }

    result = correctFirstAndSecondBytes(result);

    return result;
}

QByteArray MainWindow::correctFirstAndSecondBytes(QByteArray arr){
    QByteArray result;
    result = arr;
    int juniorByte = arr.size() -2;
    int seniorByte = 0;

    while(juniorByte-256>0){
        seniorByte++;
        juniorByte-=256;
    }

    result[0] = juniorByte;
    result[1] = seniorByte;
    return result;
}

void MainWindow::readSigFile(){
    QString fileputh;
    fileputh = QFileDialog::getOpenFileName(0,"Выбрать файл","","*.sig*");
    ui->lineEdit->setText(fileputh);
    QFile fileinput(fileputh);
    fileinput.open(QIODevice::ReadOnly);

    massIn = fileinput.readAll();
}

void MainWindow::initContainersOfKeysAndValues(){
    int i = 0;
    int j = 0;
    while(i<massIn.size()){
        mapKeys.insert(j,i);
        mapSizes.insert(j,(quint8)massIn[i]+(quint8)massIn[i+1]*256);
        map.insert(i,(quint8)massIn[i]+(quint8)massIn[i+1]*256);
        i+=map[i] ;
        i+=2;
        j++;
    }
}

void MainWindow::initIpAddressStats(){
    for(int j=0;j<mapKeys.size();j++){
        if(isIP(mapKeys[j])){
             QVector<quint8> currentAddress = getAddress(mapKeys[j]);
             if(inDictionary(currentAddress)){
                 dictAddressToIndexes[currentAddress].append(j+1);
             }else{
                 QVector<int> *indexes = new QVector<int>{j+1};
                 dictAddressToIndexes.insert(currentAddress,*indexes);
             }
       }
    }
}

bool MainWindow::isIP(int strPos)
{

    if(QString::number((quint8)massIn[strPos+16])=="69"){
       return true;
    }
    return false;
}

QVector<quint8> MainWindow::getAddress(int strPos){
    QVector<quint8> vec;
    for(int i=28;i<36;i++){
        vec.append((quint8)massIn[strPos+i]);
    }
    return vec;
}

bool MainWindow::inDictionary(QVector<quint8> arr){
    QHashIterator<QVector<quint8>,QVector<int>> currAddressToIndexes(dictAddressToIndexes);
    while (currAddressToIndexes.hasNext()) {
        currAddressToIndexes.next();
        if(dictElemIsEqualToSupposed(currAddressToIndexes.key(),arr)){
            return true;
        }

    }
    return false;
}

bool MainWindow::dictElemIsEqualToSupposed(QVector<quint8> dictElement, QVector<quint8> supposed){
    int equalElems = 0;
    int equalBytesCondition = 8;
     for(int i=0;i<dictElement.size();i++){
         if(dictElement[i]==supposed[i]){
             equalElems++;
         }else{
             return false;
         }
     }
     return equalElems==equalBytesCondition;
}

void MainWindow::setResultToUI(){
    QHashIterator<QVector<quint8>,QVector<int>> currAddressToIndexes(dictAddressToIndexes);
    while (currAddressToIndexes.hasNext()) {
        currAddressToIndexes.next();
        ui->plainTextEdit->appendPlainText(getString(currAddressToIndexes.key()) + " -> " + QString::number(currAddressToIndexes.value().size()));
    }
}


QString MainWindow::getString(QVector<quint8> vec){
    QString result = "";
    foreach(int el , vec){
        result.append(QString::number(el)+" ");
    }
    return result;
}


