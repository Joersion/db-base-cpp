#include "Connection.h"
namespace db {
    class SqliteConnection : public Connection {
    public:
        SqliteConnection();
        virtual ~SqliteConnection();

    public:
        bool load(ErrorInfo& err, const std::string& dbName);

    protected:
        virtual bool doExecute(ErrorInfo& err, const std::string& sql, std::function<void(const Result&)> cb,
                               std::function<void(Poco::Data::Statement& stmt)> op) override;

    private:
        Poco::Data::Session* session_;
    };

};  // namespace db