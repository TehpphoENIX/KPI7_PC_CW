#pragma once
#include <iinverted_index.h>

class InvertedIndex :
    public IInvertedIndex
{
    virtual void insert(const std::string token, const std::string document) override;
    virtual bool find(const std::string token) override;
    virtual const std::vector<std::string>& read(const std::string token) override;
};