#include <invertedindex.h>

#include <assert.h>
#include <string>
#include <vector>

int invertedindex_write_singlethread_test(int argc, char** argv)
{
    InvertedIndex ii;
    std::vector<std::pair<std::string, std::string>> pairs;
    for (int i = 0; i < 10; i++)
    {
        pairs.push_back({std::to_string(i), std::to_string(i+100)});
    }


    ii.insertBatch(pairs);
    ii.finish();

    for (auto pair : pairs)
    {
        assert(("element found", ii.find(pair.first) ));
        
        assert(("element retrievable", *ii.read(pair.first).begin() == pair.second));
    }
    return 0;
}