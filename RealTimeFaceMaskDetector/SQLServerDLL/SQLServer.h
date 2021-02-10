#pragma once

#include "SQLConnection.h"
#include "IniParser.h"
#define CONFIG_FILE "config.ini"

class SQLSERVER_API SQLServer : public SQLConnection
{
public:
    virtual void Connect(const std::string& database_string, /*serverName@databaseName*/ const std::string& username, const std::string& password) abstract;
    virtual void InsertPhoto(const std::string& photo_path, const std::string& photo_name, const std::string& photo_extension) abstract;
    virtual void RollBack() abstract;
    virtual void Disconnect() abstract;
    virtual void ClearTable() abstract;
};

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
    SQLServer(const std::string& database_string, /*serverName@databaseName*/ const std::string& username, const std::string& password);

    SQLServer(const ConnectParams& connect_string);

    ~SQLServer();
    void Connect() override;
    void Connect(const ConnectParams& connect_string) override;
    void InsertPhoto(const Photo& photo) override;
    /*Get all photos int the vector*/
    std::vector<PhotoType> GetAllPhotos();
    void RollBack() override;
    void Disconnect() override;
    void ClearTable(const std::string& table) override;
    void DeleteRecord(int id) override;
    bool CheckTableExists() override;
    void GetIniParams(const std::string& path) override;
    void CreatePhotosTable();
    /*create files from the vector of photos*/
    static void CreatePhotos(const std::vector<PhotoType>& photos);
    void DeleteTable(const std::string& table) override;
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

