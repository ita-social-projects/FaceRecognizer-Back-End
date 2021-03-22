#pragma once

#include "ui_TCPClientUI.h"
#include "FaceRecognizer.h"

#define RETURN_BUTTON_CLICKED 2

class TCPClientUI : public QMainWindow
{
    Q_OBJECT

public:
    TCPClientUI(QWidget *parent = Q_NULLPTR);

private slots:
    void Save();
    void Close();
    void SetRegularExp();

private:
    Ui::TCPClientUIClass ui;
};