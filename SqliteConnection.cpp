#include "SqliteConnection.h"

#include <Poco/Data/SQLite/Connector.h>

namespace db {
    SqliteConnection::SqliteConnection() : session_(nullptr) {
        Poco::Data::SQLite::Connector::registerConnector();
    }
    SqliteConnection::~SqliteConnection() {
        Poco::Data::SQLite::Connector::unregisterConnector();
        if (!session_) {
            delete session_;
            session_ = nullptr;
        }
    }

    bool SqliteConnection::load(ErrorInfo& err, const std::string& dbName) {
        try {
            session_ = new Poco::Data::Session(Poco::Data::SQLite::Connector::KEY, dbName);
            return true;
        } catch (const Poco::Data::DataException& e) {
            err.massage = e.displayText();
            err.code = e.code();
        } catch (const Poco::Exception& e) {
            err.massage = e.displayText();
            err.code = e.code();
        } catch (const std::exception& e) {
            err.massage = e.what();
        } catch (...) {
            err.massage = "unknow error";
        }
        return false;
    }

    bool SqliteConnection::doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
                                     std::function<void(Poco::Data::Statement& stmt)> op) {
        if (session_ == nullptr) {
            err.massage = "session error";
            return false;
        }
        try {
            Poco::Data::Statement stmt(*session_);
            stmt << sql;
            if (op) {
                op(stmt);
            }
            stmt.execute();
            if (cb) {
                Poco::Data::RecordSet rs(stmt);
                Result result(rs);
                cb(result);
            }
            return true;
        } catch (const Poco::Data::DataException& e) {
            err.massage = e.displayText();
            err.code = e.code();
        } catch (const Poco::Exception& e) {
            err.massage = e.displayText();
            err.code = e.code();
        } catch (const std::exception& e) {
            err.massage = e.what();
        } catch (...) {
            err.massage = "unknow error";
        }
        return false;
    }
};  // namespace db