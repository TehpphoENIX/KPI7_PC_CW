#pragma once
#include <iinverted_index.h>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>
#include <map>

class InvertedIndex :
    public IInvertedIndex
{
private:
    static const std::size_t defaultInitialSize = 4;
    static const float defaultLoadFactor = 0.6;
    std::vector< std::pair< std::string, std::vector< std::string >>> buckets;
    std::mutex readerLock, writerLock;
    float loadFactor;
    std::size_t size;
    std::hash<std::string> hash_v = std::hash<std::string>();
public:
    InvertedIndex(const std::size_t initialSize = defaultInitialSize, const float loadFactor);
    virtual void insert(const std::string token, const std::string document) override;
    virtual void insertBatch(const std::vector<std::pair<std::string,std::string>> pairs) override;
    virtual bool find(const std::string token) override;
    virtual const std::vector<std::string>& read(const std::string token) override;
private:
    std::size_t hash(const std::string token);
    std::size_t probe(const std::string token, std::size_t index);
    void insertNonSync(const std::string token, const std::string document);
    void resize();
};