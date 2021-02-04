#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/QRegExpValidator>

#include "ui_TCPClientUI.h"
#include "MaskRecognizer.h"

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