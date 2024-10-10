#include "Connection.h"
namespace db {
    class SqliteConnection : public Connection {
    public:
        SqliteConnection();
        virtual ~SqliteConnection() = default;

    private:
        virtual Poco::Data::Session* createSession(const Setting& opt, int timetout) override;
    };

};  // namespace db