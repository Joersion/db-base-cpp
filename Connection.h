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
        bool execute(ErrorInfo& err, std::function<void(const Result&)> cb, const std::string& sql, Args... args) {
            return doExecute(err, sql, cb, [&](Poco::Data::Statement& stmt) { bindParams(stmt, args...); });
        }

        template <typename... Args>
        bool execute(ErrorInfo& err, const std::string& sql, Args... args) {
            return doExecute(err, sql, nullptr, [&](Poco::Data::Statement& stmt) { bindParams(stmt, args...); });
        }

        bool execute(ErrorInfo& err, std::function<void(const Result&)> cb, const std::string& sql);

        bool execute(ErrorInfo& err, const std::string& sql);

        bool begin(ErrorInfo& err);

        bool commit(ErrorInfo& err);

        bool rollback(ErrorInfo& err);

        bool load(ErrorInfo& err, const Setting& opt);

    private:
        template <typename T, typename... Args>
        void bindParams(Poco::Data::Statement& stmt, T first, Args... args) {
            if constexpr (std::is_same<typename std::remove_cv<T>::type, std::vector<std::string>>::value) {
                for (const std::string& tmp : first) {
                    const char* cstr = tmp.c_str();
                    stmt, Poco::Data::Keywords::use(cstr);
                }
            } else if constexpr (std::is_same<T, std::string>::value) {
                std::string str = first;
                const char* cstr = str.c_str();
                stmt, Poco::Data::Keywords::use(cstr);
            } else if constexpr (std::is_same<T, int>::value || std::is_same<T, const char*>::value || std::is_same<T, long>::value) {
                stmt, Poco::Data::Keywords::use(first);
            } else {
                throw std::invalid_argument("unsupported argument type");
            }
            bindParams(stmt, args...);
        }

        void bindParams(Poco::Data::Statement&) {
        }

        bool doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
                       std::function<void(Poco::Data::Statement& stmt)> op);

    private:
        virtual Poco::Data::Session* createSession(const Setting& opt) = 0;

    protected:
        Poco::Data::Session* session_;
    };

};  // namespace db
