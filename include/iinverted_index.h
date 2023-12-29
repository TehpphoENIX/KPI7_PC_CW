#pragma once
#include <string>
#include <vector>
#include <set>

class IInvertedIndex
{
public:
    virtual void insert(const std::string token, const std::string document) = 0;
    virtual void insertBatch(const std::vector<std::pair<std::string,std::string>>& pairs) = 0;
    virtual bool find(const std::string token) = 0;
    virtual const std::vector<std::string>& read(const std::string token) = 0;
};