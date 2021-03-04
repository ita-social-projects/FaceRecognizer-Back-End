#include "LogViewer.h"

LogViewer::LogViewer(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.refreshButton, SIGNAL(clicked()), this, SLOT(OnRefreshButtonClicked()));
    connect(ui.sinceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnSinceCheckBoxChecked(int)));
    connect(ui.untilCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnUntilCheckBoxChecked(int)));

    CreateUi(QStringList() << QString{ "Time" }
                           << QString{ "Log level" }
                           << QString{ "Trace" }
                           << QString{ "Message"}
    );
}

void LogViewer::OnSinceCheckBoxChecked(int state)
{
    if (state == Qt::CheckState::Checked)
    {
        ui.fromTimeEdit->setEnabled(true);
    }
    else if (state == Qt::CheckState::Unchecked)
    {
        ui.fromTimeEdit->setEnabled(false);
    }
}

void LogViewer::OnUntilCheckBoxChecked(int state)
{
    if (state == Qt::CheckState::Checked)
    {
        ui.toTimeEdit->setEnabled(true);
    }
    else if (state == Qt::CheckState::Unchecked)
    {
        ui.toTimeEdit->setEnabled(false);
    }
}

void LogViewer::CreateUi(const QStringList& headers)
{
    // Setting up table widget.
    ui.tableWidget->setColumnCount(headers.size());
    ui.tableWidget->setShowGrid(true);
    ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->setHorizontalHeaderLabels(headers);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui.tableWidget->horizontalHeader()->setStretchLastSection(true);

    // Setting up checkboxes.
    ui.errorCheckBox->setChecked(true);
    ui.warningCheckBox->setChecked(true);
    ui.infoCheckBox->setChecked(true);
    ui.debugCheckBox->setChecked(true);
    ui.traceCheckBox->setChecked(true);
    ui.fatalCheckBox->setChecked(true);
    ui.sinceCheckBox->setChecked(false);
    ui.untilCheckBox->setChecked(false);

    // Setting up DateTimeEdit format.
    QString datetime_format{ "yyyy-mm-dd hh:mm:ss" };
    ui.fromTimeEdit->setDisplayFormat(datetime_format);
    ui.toTimeEdit->setDisplayFormat(datetime_format);

    // DateTimeEdits are disabled on start.
    ui.fromTimeEdit->setEnabled(false);
    ui.toTimeEdit->setEnabled(false);

    // Setting up combo box with log files paths.
    UpdateLogComboBox();

    UpdateTableContents();

    ui.retranslateUi(this);
}

void LogViewer::UpdateTableContents()
{
    if (ui.logComboBox->count() == 0)
    {
        return;
    }

    ui.tableWidget->clearContents();

    const std::string EMPTY_DATETIME{ "0000-00-00 00:00:00" };

    // Parsing log file.
    const std::string& PATH_TO_LOG_FILE{ ui.logComboBox->currentText().toStdString() };
    const std::string& DATETIME_SINCE{ ui.sinceCheckBox->isChecked() ? ui.fromTimeEdit->text().toStdString() : EMPTY_DATETIME };
    const std::string& DATETIME_TILL{ ui.untilCheckBox->isChecked() ? ui.toTimeEdit->text().toStdString() : EMPTY_DATETIME };
    const auto LOGS = ParseLogFile(PATH_TO_LOG_FILE, GetLogLevelFilterOptions(), DATETIME_SINCE, DATETIME_TILL);

    // Filling up table widget's contents with brushed items.
    ui.tableWidget->setRowCount(LOGS.size());

    const size_t DATETIME_COLUMN{ 0 };
    const size_t LOG_LEVEL_COLUMN{ 1 };
    const size_t TRACE_COLUMN{ 2 };
    const size_t MESSAGE_COLUMN{ 3 };

    for (size_t i{ 0 }; i < LOGS.size(); i++)
    {
        const QColor color{ log_opt_to_color.at(LOGS[i].option) };

        // datetime
        QString datetime_str{ LOGS[i].datetime.to_string().c_str() };
        auto brushed_datetime_item = SetBackgroundColor(MakeTableItem(datetime_str), color);
        ui.tableWidget->setItem(i, DATETIME_COLUMN, brushed_datetime_item);

        // log level
        QString log_level_str{ log_opt_to_str.at(LOGS[i].option) };
        auto brushed_log_level_item = SetBackgroundColor(MakeTableItem(log_level_str), color);
        ui.tableWidget->setItem(i, LOG_LEVEL_COLUMN, brushed_log_level_item);

        // trace
        QString trace_str{ LOGS[i].trace.c_str() };
        auto brushed_trace_item = SetBackgroundColor(MakeTableItem(trace_str), color);
        ui.tableWidget->setItem(i, TRACE_COLUMN, brushed_trace_item);

        // message
        QString message_str{ LOGS[i].message.c_str() };
        auto brushed_message_item = SetBackgroundColor(MakeTableItem(message_str), color);
        ui.tableWidget->setItem(i, MESSAGE_COLUMN, brushed_message_item);
    }
}

QTableWidgetItem* LogViewer::MakeTableItem(const QString& text) const
{
    QVariant table_item_variant(text);
    QTableWidgetItem* table_item = new QTableWidgetItem();
    table_item->setData(Qt::DisplayRole, table_item_variant);
    return table_item;
}

using VectorLogOptions = std::vector<logger::LogOptions>;
auto LogViewer::GetLogLevelFilterOptions() const -> VectorLogOptions
{
    VectorLogOptions result;

    using namespace logger;
    if (ui.errorCheckBox->isChecked()) 
    {
        result.push_back(LogOptions::ERR);
    }
    if (ui.warningCheckBox->isChecked()) 
    {
        result.push_back(LogOptions::WARNING);
    }
    if (ui.debugCheckBox->isChecked())
    {
        result.push_back(LogOptions::DEBUG);
    }
    if (ui.infoCheckBox->isChecked())
    {
        result.push_back(LogOptions::INFO);
    }
    if (ui.fatalCheckBox->isChecked())
    {
        result.push_back(LogOptions::FATAL);
    }
    if (ui.traceCheckBox->isChecked())
    {
        result.push_back(LogOptions::TRACE);
    }

    return result;
}

void LogViewer::UpdateLogComboBox()
{
    QStringList paths;
    paths_to_log_files = LogSearcher::GetLogFilePaths();
    for (const auto& path : paths_to_log_files)
    {
        paths.push_back(QString::fromStdString(path));
    }
    ui.logComboBox->clear();
    ui.logComboBox->addItems(paths);
}

void LogViewer::OnRefreshButtonClicked()
{
    UpdateTableContents();
    ui.retranslateUi(this);
}