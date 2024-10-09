#include "Connection.h"
namespace db {
    struct MysqlOpt {
        std::string host;
        std::string user;
        std::string password;
        std::string database;
    };
    class MysqlConnection : public Connection {
    public:
        MysqlConnection();
        virtual ~MysqlConnection();

    public:
        bool load(ErrorInfo& err, const MysqlOpt& opt);

    protected:
        virtual bool doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
                               std::function<void(Poco::Data::Statement& stmt)> op) override;

    private:
        Poco::Data::Session* session_;
    };

};  // namespace db