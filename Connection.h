#include <Poco/Data/DataException.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Statement.h>
#include <Poco/Exception.h>

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

namespace db {
    class Result {
    public:
        Result(const Poco::Data::RecordSet& rs);
        ~Result() = default;

    public:
        size_t rowCount() const;

        std::string data(size_t row, const std::string& colName) const {
            try {
                auto value = rs_.value(colName, row);
                return value;
            } catch (...) {
                return "";
            }
        }

    private:
        Poco::Data::RecordSet rs_;
    };

    struct ErrorInfo {
        std::string massage;
        int code;
    };

    struct Setting {
        int timeout;
        std::string host;
        std::string user;
        std::string password;
        std::string database;

        std::string dbName;
    };

    class Connection {
    public:
        Connection();
        Connection(ErrorInfo& err, const Setting& opt);
        virtual ~Connection();

    public:
        template <typename... Args>
        bool execute(ErrorInfo& err, const std::string& sql, Args... args, std::function<void(const Result&)> cb = nullptr) {
            return doExecute(err, sql, cb, [&](Poco::Data::Statement& stmt) { bindParams(stmt, args...); });
        }

        bool execute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb = nullptr);

        bool begin(ErrorInfo& err);

        bool commit(ErrorInfo& err);

        bool rollback(ErrorInfo& err);

        bool load(ErrorInfo& err, const Setting& opt);

    private:
        template <typename T, typename... Args>
        void bindParams(Poco::Data::Statement& stmt, T first, Args... args) {
            stmt << Poco::Data::Keywords::bind(first);
            bindParams(stmt, args...);
        }

        void bindParams(Poco::Data::Statement&) {
        }

        bool doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
                       std::function<void(Poco::Data::Statement& stmt)> op);

    private:
        virtual Poco::Data::Session* createSession(const Setting& opt, int timetout) = 0;

    protected:
        Poco::Data::Session* session_;
    };

};  // namespace db
