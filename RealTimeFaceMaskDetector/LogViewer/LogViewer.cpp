#include "LogViewer.h"

LogViewer::LogViewer(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.refreshButton, SIGNAL(QPushButton::clicked), SLOT(OnRefreshButtonClicked));

    CreateUi(QStringList() << QString{ "Id" }
                           << QString{ "Time" }
                           << QString{ "Category" }
                           << QString{ "Function" }
                           << QString{ "Message"}
    );
}

void LogViewer::CreateUi(const QStringList& headers) 
{
    ui.tableWidget->setColumnCount(headers.size());
    ui.tableWidget->setShowGrid(true);
    ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->setHorizontalHeaderLabels(headers);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    ui.errorCheckBox->setChecked(true);
    ui.warningCheckBox->setChecked(true);
    ui.infoCheckBox->setChecked(true);
    ui.debugCheckBox->setChecked(true);
    ui.traceCheckBox->setChecked(true);
}

void LogViewer::OnRefreshButtonClicked()
{
    qDebug() << "Somebody clicked on the refresh button! Stop this man!";
}