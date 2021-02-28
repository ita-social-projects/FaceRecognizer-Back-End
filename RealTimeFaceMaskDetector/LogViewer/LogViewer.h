#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <memory>
#include "ui_LogViewer.h"

class LogViewer : public QMainWindow
{
    Q_OBJECT

public:
    LogViewer(QWidget *parent = Q_NULLPTR);

private:
    void CreateUi(const QStringList& headers);
    void AddRow(const int id);

private:
    Ui::LogViewerClass ui;
};
