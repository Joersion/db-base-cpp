#include "MysqlConnection.h"

#include <Poco/Data/MySQL/Connector.h>

namespace db {
    class MySQLConnector {
    public:
        MySQLConnector() {
            Poco::Data::MySQL::Connector::registerConnector();
        }
        ~MySQLConnector() {
            Poco::Data::MySQL::Connector::unregisterConnector();
        }
    };
    MysqlConnection::MysqlConnection() {
        Poco::Data::MySQL::Connector::registerConnector();
    }

    Poco::Data::Session* MysqlConnection::createSession(const Setting& opt) {
        char buf[256] = {0};
        sprintf(buf, "host=%s;user=%s;password=%s;database=%s", opt.host.data(), opt.user.data(), opt.password.data(), opt.database.data());
        return new Poco::Data::Session(Poco::Data::MySQL::Connector::KEY, buf);
    }
};  // namespace db