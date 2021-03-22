#pragma once

#ifdef SQLSERVER_EXPORTS
#define SQLSERVER_API __declspec(dllexport)
#else
#define SQLSERVER_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <SQLAPI.h>
#include <ctime>
#include "SQLException.h"

struct Photo
{
    std::string name;
    std::string path;
    std::string extension;
    std::string date;
};
/*Parameters needed to connect to the server*/
struct ConnectParams
{
    std::string server;
    std::string database;
    std::string username;
    std::string password;
    std::string table;
};

class SQLSERVER_API SQLConnection
{
public:
    /*Connect to database with params from inifile*/
    virtual void Connect() = 0;
    /*Connect to database with preset parameters*/
    virtual void Connect(const ConnectParams& connect_string) = 0;
    /*Insert one photo record into the table*/
    virtual void InsertPhoto(const Photo& photo) = 0;
    /*Rollback all actions in case of error occurence*/
    virtual void RollBack() = 0;
    /*Disconnect from the server*/
    virtual void Disconnect() = 0;
    /*Delete all records from the table*/
    virtual void ClearTable(const std::string& table) = 0;
    /*Delete one record from the Photos table*/
    virtual void DeleteRecord(int id) = 0;
    /*Check if the table for Photos exists*/
    virtual bool CheckTableExists() = 0;
    /*Create the table for Photos*/
    virtual void CreatePhotosTable() = 0;
    /*Get connection parameters from ini file in the application folder*/
    virtual void GetIniParams(const std::string& path) = 0;
    /*Remove a table from the database*/
    virtual void DeleteTable(const std::string& table) = 0;
    virtual bool IsConnected()=0;
    virtual ~SQLConnection() {};
};