#pragma once

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include "ui_LogViewer.h"

#include "LogSearcher.h"
#include "Parser.h"
#include "utility.h"
#include "../DateTimeDLL/DateTime.h"

class LogViewer : public QMainWindow
{
    Q_OBJECT

public:
    LogViewer(QWidget *parent = Q_NULLPTR);

private slots:
    void OnRefreshButtonClicked();
    void OnSinceCheckBoxChecked(int state);
    void OnUntilCheckBoxChecked(int state);

private:
    void CreateUi(const QStringList& headers);
    void UpdateTableContents();
    QTableWidgetItem* MakeTableItem(const QString& content) const;
    std::vector<logger::LogOptions> GetLogLevelFilterOptions() const;
    void UpdateLogComboBox();

private:
    Ui::LogViewerClass ui;
    std::vector<std::string> paths_to_log_files;
};
