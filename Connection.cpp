#include "Connection.h"

#include <Poco/Data/SQLite/Connector.h>
namespace db {
    Result::Result(const Poco::Data::RecordSet& rs) : rs_(rs) {
    }

    size_t Result::rowCount() const {
        return rs_.rowCount();
    }

    Connection::Connection() : session_(nullptr) {
    }

    Connection::Connection(ErrorInfo& err, const Setting& opt) : session_(nullptr) {
        load(err, opt);
    }

    Connection::~Connection() {
        if (session_) {
            delete session_;
            session_ = nullptr;
        }
    }

    bool Connection::execute(ErrorInfo& err, std::function<void(const Result&)> cb, const std::string& sql) {
        return doExecute(err, sql, cb, nullptr);
    }

    bool Connection::execute(ErrorInfo& err, const std::string& sql) {
        return doExecute(err, sql, nullptr, nullptr);
    }

    bool Connection::doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
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

    bool Connection::begin(ErrorInfo& err) {
        if (session_ == nullptr) {
            err.massage = "session error";
            return false;
        }
        session_->begin();
        return true;
    }

    bool Connection::commit(ErrorInfo& err) {
        if (session_ == nullptr) {
            err.massage = "session error";
            return false;
        }
        session_->commit();
        return true;
    }

    bool Connection::rollback(ErrorInfo& err) {
        if (session_ == nullptr) {
            err.massage = "session error";
            return false;
        }
        session_->rollback();
        return true;
    }

    bool Connection::load(ErrorInfo& err, const Setting& opt) {
        int timeout = 30;
        if (session_) {
            delete session_;
            session_ = nullptr;
        }
        try {
            if (opt.timeout > 0) {
                timeout = opt.timeout;
            }
            session_ = createSession(opt, timeout);
            if (!session_) {
                return false;
            }
            return true;
        } catch (const std::runtime_error& e) {
            err.massage = e.what();
            err.code = -1;
        } catch (...) {
            err.massage = "unknow error";
        }
        return false;
    }
};  // namespace db