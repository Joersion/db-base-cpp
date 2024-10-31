#include "RedisCache.h"

#include <Poco/Exception.h>
#include <Poco/Redis/Array.h>
#include <Poco/Redis/Client.h>
#include <Poco/Redis/Command.h>
#include <Poco/Redis/Redis.h>

using namespace Poco::Redis;

class RedisHandler {
private:
    RedisHandler() : isconnect_(false) {
    }
    ~RedisHandler() {
    }

public:
    static RedisHandler& instance() {
        static RedisHandler ins;
        return ins;
    }

    bool connect(std::string& err, const std::string& ip, const std::string password, int port) {
        if (isconnect_) {
            session_.disconnect();
            isconnect_ = false;
        }
        std::string eMsg, code;
        try {
            Poco::Timespan t(10, 0);
            session_.connect(ip, port, t);
            isconnect_ = true;
            err = "";
            return true;
        } catch (Poco::Exception& e) {
            eMsg = e.message();
            code = std::to_string(e.code());
        }
        isconnect_ = false;
        err = "Couldn't connect to [" + ip + ":" + std::to_string(port) + "],code:" + code + ",msg:" + eMsg;
        return false;
    }

    bool set(const std::string& k, const std::string& v, int timeout) {
        if (!isconnect_) {
            return false;
        }
        Array command;
        command.add("SET").add(k).add(v);

        try {
            std::string result = session_.execute<std::string>(command);
            bool ret = false;
            !result.compare("OK") ? ret = true : ret = false;
            if (ret && timeout > 0) {
                Poco::Int64 result = session_.execute<Poco::Int64>(Command::expire(k, timeout));
                result > 0 ? ret = true : ret = false;
            }
            return ret;
        } catch (RedisException& e) {
            return false;
        }
    }

    std::string get(const std::string& k) {
        std::string ret = "";
        if (!isconnect_) {
            return ret;
        }
        try {
            BulkString result = session_.execute<BulkString>(Command::get(k));
            ret = result.value();
        } catch (...) {
        }
        return ret;
    }

    bool del(const std::string& k) {
        if (!isconnect_) {
            return false;
        }
        Command del = Command::del(k);
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(del);
            return result > 0;
        } catch (...) {
        }
        return false;
    }

    bool append(const std::string& k, const std::string& v) {
        if (!isconnect_) {
            return false;
        }
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(Command::append(k, v));
            return result > 0;
        } catch (...) {
        }
        return false;
    }

    bool hset(const std::string& k, const std::string& field, const std::string& v, int timeout) {
        if (!isconnect_) {
            return false;
        }
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(Command::hset(k, field, v));
            if (timeout > 0 && result > 0) {
                result = session_.execute<Poco::Int64>(Command::expire(k, timeout));
            }
            return result > 0;
        } catch (...) {
        }
        return false;
    }

    std::string hget(const std::string& k, const std::string& field) {
        if (!isconnect_) {
            return "";
        }
        Command hget = Command::hget(k, field);
        try {
            BulkString s = session_.execute<BulkString>(hget);
            return s.value();
        } catch (...) {
        }
        return "";
    }

    bool hdel(const std::string& k, const std::string& field) {
        if (!isconnect_) {
            return false;
        }
        Command hdel = Command::hdel(k, field);
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(hdel);
            return result > 0;
        } catch (...) {
        }
        return false;
    }

    std::map<std::string, std::string> hgetall(const std::string& k) {
        if (!isconnect_) {
            return std::map<std::string, std::string>();
        }
        std::map<std::string, std::string> content;
        Command hgetall = Command::hgetall(k);
        try {
            Array array = session_.execute<Array>(hgetall);
            std::string key, value;
            for (int i = 0; i < array.size(); ++i) {
                if (i % 2 == 1) {
                    content[key] = array.get<BulkString>(i).value();
                    continue;
                }
                key = array.get<BulkString>(i).value();
            }
        } catch (...) {
        }
        return content;
    }

    bool lpush(const std::string& k, const std::string& v) {
        if (!isconnect_) {
            return false;
        }
        Command lpush = Command::lpush(k, v);
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(lpush);
            return result > 0;
        } catch (...) {
        }
        return false;
    }

    bool rpush(const std::string& k, const std::string& v) {
        if (!isconnect_) {
            return false;
        }
        Command rpush = Command::rpush(k, v);
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(rpush);
            return result > 0;
        } catch (...) {
        }
        return false;
    }

    std::string lpop(const std::string& k) {
        if (!isconnect_) {
            return "";
        }
        Command lpop = Command::lpop(k);
        try {
            BulkString result = session_.execute<BulkString>(lpop);
            return result.value();
        } catch (...) {
        }
        return "";
    }

    std::string rpop(const std::string& k) {
        if (!isconnect_) {
            return "";
        }
        Command rpop = Command::rpop(k);
        try {
            BulkString result = session_.execute<BulkString>(rpop);
            return result.value();
        } catch (...) {
        }
        return "";
    }

    int llen(const std::string& k) {
        if (!isconnect_) {
            return 0;
        }
        Command llen = Command::llen(k);
        try {
            Poco::Int64 result = session_.execute<Poco::Int64>(llen);
            return result;
        } catch (...) {
        }
        return 0;
    }

    std::string lindex(const std::string& k, int index) {
        if (!isconnect_) {
            return "";
        }
        if (index > llen(k) - 1) {
            return "";
        }
        Command lindex = Command::lindex(k, index);
        try {
            BulkString result = session_.execute<BulkString>(lindex);
            return result.value();
        } catch (...) {
        }
        return "";
    }

private:
    bool isconnect_;
    Client session_;
};

bool RedisCache::start(std::string& err, const std::string& ip, const std::string password, int port) {
    return RedisHandler::instance().connect(err, ip, password, port);
}

bool RedisCache::SET(const std::string& k, const std::string& v, int timeout) {
    return RedisHandler::instance().set(k, v, timeout);
}

std::string RedisCache::GET(const std::string& k) {
    return RedisHandler::instance().get(k);
}

bool RedisCache::DEL(const std::string& k) {
    return RedisHandler::instance().del(k);
}

bool RedisCache::APPEND(const std::string& k, const std::string& v) {
    return RedisHandler::instance().append(k, v);
}

bool RedisCache::HSET(const std::string& k, const std::string& field, const std::string& v, int timeout) {
    return RedisHandler::instance().hset(k, field, v, timeout);
}

std::string RedisCache::HGET(const std::string& k, const std::string& field) {
    return RedisHandler::instance().hget(k, field);
}

bool RedisCache::HDEL(const std::string& k, const std::string& field) {
    return RedisHandler::instance().hdel(k, field);
}

std::map<std::string, std::string> RedisCache::HGETALL(const std::string& k) {
    return RedisHandler::instance().hgetall(k);
}

bool RedisCache::LPUSH(const std::string& k, const std::string& v) {
    return RedisHandler::instance().lpush(k, v);
}

bool RedisCache::RPUSH(const std::string& k, const std::string& v) {
    return RedisHandler::instance().rpush(k, v);
}

std::string RedisCache::LPOP(const std::string& k) {
    return RedisHandler::instance().lpop(k);
}

std::string RedisCache::RPOP(const std::string& k) {
    return RedisHandler::instance().rpop(k);
}

int RedisCache::LLEN(const std::string& k) {
    return RedisHandler::instance().llen(k);
}
std::string RedisCache::LINDEX(const std::string& k, int index) {
    return RedisHandler::instance().lindex(k, index);
}