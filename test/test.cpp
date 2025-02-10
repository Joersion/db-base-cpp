#include <iostream>
#include <string>
#include <vector>

#include "../RedisCache.h"
#include "../SqliteConnection.h"

void print(const std::string& info) {
    std::cout << "-------------" << info << "-------------" << std::endl;
}

void print2(const std::string& info) {
    std::cout << info << std::endl;
}

void testSqlite(const std::string& name) {
    print("测试sqlite开始!!!");
    db::SqliteConnection db;
    db::Setting opt;
    opt.dbName = "test.db";
    opt.timeout = 1;
    db::ErrorInfo error;
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("加载不存在的库");
    opt.dbName = "/home/joersion/work/test";
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    opt.dbName = "/home/joersion/work/test/test.db";
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("检查表");
    if (!db.execute(
            error,
            [&](const db::Result& result) {
                if (result.rowCount() > 0) {
                    std::cout << "检查表成功，users" << std::endl;
                }
            },
            "select name from sqlite_master where type='table' and name=?", name)) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据插入");
    if (!db.execute(error, "insert into users (name,email,age,created_at) values(?,?,?,CURRENT_TIMESTAMP)", "wjx", "123456", 100)) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("查看插入是否成功");
    if (!db.execute(
            error,
            [](const db::Result& result) {
                for (int i = 0; i < result.rowCount(); ++i) {
                    std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << "|"
                              << result.data(i, "age") << "|" << result.data(i, "created_at") << std::endl;
                }
            },
            "select id,name,email,age,created_at from users")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据replace");
    if (!db.execute(error, "replace into users (name,email,age,created_at) values(?,?,?,CURRENT_TIMESTAMP)", "吴佳祥", "123456", "1")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据更新");
    if (!db.execute(error, "update users set name=? where id>?", "吴佳祥", "4")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("查看更新是否成功");
    if (!db.execute(
            error,
            [](const db::Result& result) {
                for (int i = 0; i < result.rowCount(); ++i) {
                    std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << "|"
                              << result.data(i, "created_at") << std::endl;
                }
            },
            "select id,name,email,age,created_at from users")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据删除");
    if (!db.execute(error, "delete from users where id>4")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("查看删除是否成功");
    if (!db.execute(
            error,
            [](const db::Result& result) {
                for (int i = 0; i < result.rowCount(); ++i) {
                    std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << "|"
                              << result.data(i, "created_at") << std::endl;
                }
            },
            "select id,name,email,age,created_at from users")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }
}

void testRedis() {
    print("测试redis开始!!!");
    std::string error;
    if (!RedisCache::start(error)) {
        std::cout << "RedisCache start error,msg:" << error << std::endl;
    }

    // string
    print("测试string开始...");
    if (RedisCache::SET("testRedis_string", "good set!")) {
        print2("SET sucess");
    }
    if (RedisCache::APPEND("testRedis_string", "good append!")) {
        print2("APPEND sucess");
    }
    if (RedisCache::GET("testRedis_string") == "good set!good append!") {
        print2("GET sucess");
    }
    if (RedisCache::DEL("testRedis_string") && RedisCache::GET("testRedis_string") == "") {
        print2("DEL sucess");
    }
    // hash
    print("测试hash开始...");
    if (RedisCache::HSET("testRedis_hash", "hush1", "my name is one") && RedisCache::HSET("testRedis_hash", "hush2", "my name is two")) {
        print2("HSET sucess");
    }
    if (RedisCache::HGET("testRedis_hash", "hush1") == "my name is one" && RedisCache::HGET("testRedis_hash", "hush2") == "my name is two") {
        print2("HGET sucess");
    }
    std::map<std::string, std::string> table = RedisCache::HGETALL("testRedis_hash");
    for (auto v : table) {
        std::cout << v.first << "," << v.second << std::endl;
    }
    if (RedisCache::HDEL("testRedis_hash", "hush1") && RedisCache::HDEL("testRedis_hash", "hush2")) {
        print2("HDEL sucess");
    }
    // list
    print("测试list开始...");
    if (RedisCache::LPUSH("testRedis_list", "hello1") && RedisCache::LPUSH("testRedis_list", "hello0")) {
        print2("LPUSH sucess");
    }
    if (RedisCache::RPUSH("testRedis_list", "hello2")) {
        print2("RPUSH sucess");
    }
    if (RedisCache::LINDEX("testRedis_list", 0) == "hello0" && RedisCache::LINDEX("testRedis_list", 1) == "hello1" &&
        RedisCache::LINDEX("testRedis_list", 2) == "hello2") {
        print2("LINDEX sucess");
    }
    if (RedisCache::LPOP("testRedis_list") == "hello0") {
        print2("LPOP sucess");
    }
    if (RedisCache::RPOP("testRedis_list") == "hello2") {
        print2("RPOP sucess");
    }
    if (RedisCache::LLEN("testRedis_list") == 1 && RedisCache::LINDEX("testRedis_list", 0) == "hello1" &&
        RedisCache::LINDEX("testRedis_list", 1) == "") {
        print2("LLEN sucess");
    }
    if (RedisCache::DEL("testRedis_list")) {
        print2("DEL sucess");
    }
    // 测试keys*
    print("测试keys*开始...");
    std::vector<std::string> v;
    RedisCache::KEYS(v, "*");
    for (int i = 0; i < v.size(); i++) {
        print2("KEYS *:" + v[i]);
    }
}

int main() {
    testSqlite("users");
    testRedis();
    return 0;
}