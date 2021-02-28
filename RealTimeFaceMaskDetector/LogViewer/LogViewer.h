#pragma once

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <qDebug>
#include "ui_LogViewer.h"

class LogViewer : public QMainWindow
{
    Q_OBJECT

public:
    LogViewer(QWidget *parent = Q_NULLPTR);

private slots:
    // TODO: implement refreshing table, also implement filters feature
    void OnRefreshButtonClicked();

private:
    void CreateUi(const QStringList& headers);
    // TODO: void AddRow(...);

private:
    Ui::LogViewerClass ui;
    // TODO: add container of elements for further
};
