#include <invertedindex.h>

#include <assert.h>

InvertedIndex::InvertedIndex(const std::size_t initialSize, const float loadFactor) :
    loadFactor((loadFactor > 1 || loadFactor <= 0)? defaultLoadFactor : loadFactor)
{
    if(initialSize < 1)
    {
        size = defaultInitialSize;
    }
    else
    {
        size = initialSize;
    }
    buckets = std::vector< std::pair< std::string, std::set< std::string >>>(size);
}

void InvertedIndex::insert(const std::string token, const std::string document)
{
    if ( finished )
        throw std::exception();

    std::unique_lock<std::mutex> lock(writerLock);
    insertNonSync(token, document);
}

void InvertedIndex::insertBatch(const std::vector<std::pair<std::string, std::string>>& pairs)
{
    if ( finished )
        throw std::exception();

    std::unique_lock<std::mutex> lock(writerLock);
    for ( auto pair : pairs )
    {
        insertNonSync(pair.first, pair.second);
    }
}

bool InvertedIndex::find(const std::string token)
{
    if(token.empty() || token[0] == '\0')
        return false;

    if (!finished )
        throw std::exception();

    std::size_t index = probe(token, hash(token));
    return index != -1;
}

const std::set<std::string> &InvertedIndex::read(const std::string token)
{
    if(token.empty() || token[0] == '\0')
        throw std::exception();
    
    if (!finished )
        throw std::exception();

    std::size_t index = probe(token, hash(token));
    if ( index != -1)
    {
        return buckets[index].second;
    }
    else
    {
        throw std::exception();
    }
}

void InvertedIndex::insertNonSync(const std::string token, const std::string document)
{
    std::size_t index = probe(token, hash(token));
    if ( index == -1 )
    {
        resize();
        index = probe(token, hash(token));
        assert(( "InvertedIndex probing failed", index != -1 ));
    }

    if(buckets[index].first != token)
    {
        assert(( "Insertion does not override", buckets[index].first == std::string() ));
        buckets[index].first = token;
        size++;
    }
    
    buckets[index].second.insert(document);

    if ( static_cast<float>(size) / buckets.capacity() > loadFactor )
    {
        resize();
    }
}

void InvertedIndex::finish()
{
    finished = true;
}

void InvertedIndex::resize()
{
    auto old_buckets = buckets;
    buckets = std::vector< std::pair< std::string, std::set< std::string >>>(buckets.capacity()*2);

    for(auto pair : old_buckets)
    {
        std::size_t index = probe(pair.first, hash(pair.first));
        assert(( "Probing failed on resize", index != -1 ));
        buckets[index].first = pair.first;
        buckets[index].second = pair.second;
    }
}

std::size_t InvertedIndex::hash(const std::string token)
{
    return hash_v(token) % buckets.size();
}

std::size_t InvertedIndex::probe(const std::string token, std::size_t index)
{
    auto position = index;
    unsigned long i = 0;
    while (buckets[position].first != token && buckets[position].first != std::string())
    {
        i++;
        position = (index + i + i * i) % buckets.size();
    }
    return position;
}
