#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>

class SemanticComparator
{
    std::string vocab;
    std::vector<float> M;
    long long words, size;
public:
    SemanticComparator();
};
double dist(const std::string &correct_string, const std::string &users_string);


#endif // SEMANTIC_H
