#include "SqliteConnection.h"

#include <Poco/Data/SQLite/Connector.h>

namespace db {
    class SqliteConnector {
    public:
        SqliteConnector() {
            Poco::Data::SQLite::Connector::registerConnector();
        }
        ~SqliteConnector() {
            Poco::Data::SQLite::Connector::unregisterConnector();
        }
    };

    SqliteConnection::SqliteConnection() {
        static SqliteConnector con;
    }

    Poco::Data::Session* SqliteConnection::createSession(const Setting& opt, int timetout) {
        try {
            const std::string extension = ".db";
            if (opt.dbName.length() < extension.length()) {
                throw std::runtime_error("fileName too short");
            }
            if (opt.dbName.substr(opt.dbName.length() - extension.length()) != extension) {
                throw std::runtime_error("fileName need end whth .db");
            }
            return new Poco::Data::Session(Poco::Data::SQLite::Connector::KEY, opt.dbName, timetout);
        } catch (const Poco::Data::DataException& e) {
            throw std::runtime_error("DataException: " + e.displayText());
        } catch (const Poco::Exception& e) {
            throw std::runtime_error("PocoException: " + e.displayText());
        } catch (const std::exception& e) {
            throw std::runtime_error("StandardException: " + std::string(e.what()));
        } catch (...) {
            throw std::runtime_error("Unknown exception occurred while creating session.");
        }
        return nullptr;
    }
};  // namespace db