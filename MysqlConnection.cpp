#include "MysqlConnection.h"

#include <Poco/Data/MySQL/Connector.h>

namespace db {
    MysqlConnection::MysqlConnection() : session_(nullptr) {
        Poco::Data::MySQL::Connector::registerConnector();
    }
    MysqlConnection::~MysqlConnection() {
        Poco::Data::MySQL::Connector::unregisterConnector();
        if (!session_) {
            delete session_;
            session_ = nullptr;
        }
    }

    bool MysqlConnection::load(ErrorInfo& err, const MysqlOpt& opt) {
        char buf[256] = {0};
        sprintf(buf, "host=%s;user=%s;password=%s;database=%s", opt.host, opt.user, opt.password, opt.database);
        try {
            session_ = new Poco::Data::Session(Poco::Data::MySQL::Connector::KEY, buf);
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

    bool MysqlConnection::doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
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