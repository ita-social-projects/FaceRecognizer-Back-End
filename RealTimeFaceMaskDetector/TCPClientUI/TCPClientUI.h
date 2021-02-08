#pragma once

#include "ui_TCPClientUI.h"

class TCPClientUI : public QMainWindow
{
    Q_OBJECT

public:
    TCPClientUI(QWidget *parent = Q_NULLPTR);

private slots:
    void Save();
    void Close();

public:
    Ui::TCPClientUIClass ui;
};