#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFile>
#include<QDebug>
#include<QTextStream>
#include<QStringList>
#include<QFileDialog>

QString mFilename = "C:/test/myfile.txt";

void Write(QString Filename,QString str)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"無法開啟檔案";
        return;
    }

    QTextStream out(&mFile);
    out<<str;
    mFile.flush();
    mFile.close();
}

QString Read(QString Filename)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"無法開啟檔案";
        return "";
    }

    QTextStream in(&mFile);
    QString str = in.readAll();
    mFile.close();

    return str;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList ColTitle;

    ui->tableWidget->setColumnCount(4);
    ColTitle<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->tableWidget->setHorizontalHeaderLabels(ColTitle);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;
    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));
    int rc = ui->tableWidget->rowCount();
    qDebug()<<rc<<"\n";
    ui->tableWidget->insertRow(rc);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);

    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
}


void MainWindow::on_pushButton_2_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "儲存通訊錄",
        mFilename,
        "Text Files (*.txt);;CSV Files (*.csv);;All Files (*)"
        );

    if(filePath.isEmpty()){
        return;
    }

    QString saveFile="";

    for(int i=0; i < ui->tableWidget->rowCount(); i++){
        for(int j=0; j < ui->tableWidget->columnCount(); j++){
            saveFile += ui->tableWidget->item(i, j)->text();

            if(j < ui->tableWidget->columnCount() - 1){
                saveFile += ",";
            }
        }

        saveFile += "\n";
    }

    Write(filePath, saveFile);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "開啟通訊錄檔案",
        mFilename,
        "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)"
        );

    if(filePath.isEmpty()){
        return;
    }

    QString fileContent = Read(filePath);

    ui->tableWidget->setRowCount(0);

    QStringList lines = fileContent.split('\n', Qt::SkipEmptyParts);

    for(const QString &line : lines)
    {
        if(line.isEmpty()) continue;

        QStringList fields = line.split(',');

        if(fields.size() == ui->tableWidget->columnCount())
        {
            int currentRow = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(currentRow);

            for(int j = 0; j < fields.size(); ++j)
            {
                QTableWidgetItem *item = new QTableWidgetItem(fields.at(j));
                ui->tableWidget->setItem(currentRow, j, item);
            }
        } else {
            qDebug() << "因欄位數量不正確，已略過此行:" << line;
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    close();
}
