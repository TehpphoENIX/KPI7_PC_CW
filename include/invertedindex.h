#pragma once
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class InvertedIndex
{
private:
    static const std::size_t defaultInitialSize = 4;
    static constexpr float defaultLoadFactor = 0.6f;
    std::vector< std::pair< std::string, std::set< std::string >>> buckets;
    std::mutex readerLock, writerLock;
    float loadFactor;
    std::size_t size;
    std::hash<std::string> hash_v = std::hash<std::string>();
    bool finished = false;
public:
    InvertedIndex(const std::size_t initialSize = defaultInitialSize, const float loadFactor = defaultLoadFactor);
    virtual void insert(const std::string token, const std::string document);
    virtual void insertBatch(const std::vector<std::pair<std::string,std::string>>& pairs);
    virtual bool find(const std::string token);
    virtual const std::set<std::string>& read(const std::string token);
    void finish();
private:
    std::size_t hash(const std::string token);
    std::size_t probe(const std::string token, std::size_t index);
    void insertNonSync(const std::string token, const std::string document);
    void resize();
};