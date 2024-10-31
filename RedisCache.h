
#include <map>
#include <string>

class RedisCache {
public:
    RedisCache() = default;
    ~RedisCache() = default;

public:
    static bool SET(const std::string& k, const std::string& v, int timeout = 0);
    static std::string GET(const std::string& k);
    static bool DEL(const std::string& k);
    static bool APPEND(const std::string& k, const std::string& v);
    static bool HSET(const std::string& k, const std::string& field, const std::string& v, int timeout = 0);
    static std::string HGET(const std::string& k, const std::string& field);
    static bool HDEL(const std::string& k, const std::string& field);
    static std::map<std::string, std::string> HGETALL(const std::string& k);
    static bool LPUSH(const std::string& k, const std::string& v);
    static bool RPUSH(const std::string& k, const std::string& v);
    static std::string LPOP(const std::string& k);
    static std::string RPOP(const std::string& k);
    static int LLEN(const std::string& k);
    static std::string LINDEX(const std::string& k, int index);

    static bool start(std::string& err, const std::string& ip = "127.0.0.1", const std::string password = "", int port = 6379);
};
