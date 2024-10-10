#include "Connection.h"
namespace db {
    class MysqlConnection : public Connection {
    public:
        MysqlConnection();
        virtual ~MysqlConnection() = default;

    private:
        virtual Poco::Data::Session* createSession(const Setting& opt) override;
    };

};  // namespace db