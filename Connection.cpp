#include "Connection.h"

#include <Poco/Data/SQLite/Connector.h>
namespace db {
    Result::Result(const Poco::Data::RecordSet& rs) : rs_(rs) {
    }

    size_t Result::rowCount() const {
        return rs_.rowCount();
    }

    Connection::Connection() {
    }

    bool Connection::execute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb) {
        return doExecute(err, sql, cb, nullptr);
    }

};  // namespace db