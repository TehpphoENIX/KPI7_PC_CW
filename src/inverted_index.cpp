#include <inverted_index.h>
#include <assert.h>

void InvertedIndex::insert(const std::string token, const std::string document)
{
    std::unique_lock<std::mutex> lock(writerLock);
    _insertNonSync(token, document);
}

void InvertedIndex::insertBatch(const std::vector<std::pair<std::string, std::string>> pairs)
{
    std::unique_lock<std::mutex> lock(writerLock);
    for ( auto pair : pairs )
    {
        _insertNonSync(pair.first, pair.second);
    }
}

bool InvertedIndex::find(const std::string token)
{
    return false;
}

const std::vector<std::string> &InvertedIndex::read(const std::string token)
{
    // TODO: insert return statement here
}

void InvertedIndex::_insertNonSync(const std::string token, const std::string document)
{
    std::size_t index = probe(token, hash(token));
    if ( index == -1 )
    {
        resize();
        index = probe(token, hash(token));
        assert(( "InvertedIndex probing failed", index != -1 ));
    }

    buckets[index].first = token;
    buckets[index].second.push_back(document);

    if ( buckets.size() / buckets.capacity() > loadFactor )
    {
        resize();
    }
}
