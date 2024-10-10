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

    Poco::Data::Session* SqliteConnection::createSession(const Setting& opt) {
        return new Poco::Data::Session(Poco::Data::SQLite::Connector::KEY, opt.dbName);
    }
};  // namespace db