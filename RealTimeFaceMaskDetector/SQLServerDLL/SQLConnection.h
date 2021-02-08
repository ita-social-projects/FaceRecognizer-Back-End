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
    tm date;
};
/*Parameters needed to connect to the server*/
struct ConnectParams
{
    std::string server;
    std::string database;
    std::string username;
    std::string password;
};

class SQLSERVER_API SQLConnection abstract
{
public:
    /*Connect to database with params from inifile*/
    virtual void Connect() abstract;
    /*Connect to database with preset parameters*/
    virtual void Connect(const ConnectParams& connect_string) abstract;
    /*Insert one photo record into the table*/
    virtual void InsertPhoto(const Photo& photo) abstract;
    /*Rollback all actions in case of error occurence*/
    virtual void RollBack() abstract;
    /*Disconnect from the server*/
    virtual void Disconnect() abstract;
    /*Delete all records from the Photos table*/
    virtual void ClearTable(const std::string& table) abstract;
    /*Delete one record from the Photos table*/
    virtual void DeleteRecord(int id) abstract;
    /*Check if the table for Photos exists*/
    virtual bool CheckTableExists(const std::string& table) abstract;
    /*Create the table for Photos*/
    virtual void CreatePhotosTable(const std::string& table) abstract;
    /*Get connection parameters from ini file in the application folder*/
    virtual void GetIniParams(const std::string& path) abstract;
    /*Remove a table from the database*/
    virtual void DeleteTable(const std::string& table) abstract;
};