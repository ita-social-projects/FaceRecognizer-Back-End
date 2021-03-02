#pragma once

#include "SQLConnection.h"
#include "XMLParser.h"
#define CONFIG_FILE "config.xml"

class SQLSERVER_API SQLServer : public SQLConnection
{
public:

    struct PhotoType
    {
        SAString photoBytes;
        SAString photoName;
        SAString photoExtension;
    };

    SQLServer() = default;

    SQLServer(const ConnectParams& connect_string);

    ~SQLServer();
    void Connect() override;
    void Connect(const ConnectParams& connect_string) override;
    void InsertPhoto(const Photo& photo) override;
    void RollBack() override;
    void Disconnect() override;
    void ClearTable(const std::string& table) override;
    void DeleteRecord(int id) override;
    bool CheckTableExists() override;
    void GetIniParams(const std::string& path) override;
    void CreatePhotosTable();
    void DeleteTable(const std::string& table) override;
    bool IsConnected();
protected:
    /*Nested class for exceptions*/
    class SQLServerException :public SQLException
    {
    public:
        SQLServerException() = default;
        void GetParams(const SAException& ex);
    };
    /*Execute query*/
    void ExecSQLQuery(const std::string& query);
    /*Get path to .exe file*/
    std::string ExePath();
    SAConnection m_connection;
    SQLServerException sql_error;
    ConnectParams params;
};

