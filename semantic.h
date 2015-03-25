#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>

class SemanticComparator
{
    std::vector<std::string> vocabulary;
    std::vector<std::vector<float> > words_vectors;
    long long vocabulary_words_number;
    unsigned long long word_vector_size;

    static void normalize(std::vector<float> &vector);
    static float compute_vector_distance(const std::vector<float> &first_vector,
                                         const std::vector<float> &second_vector);
    std::vector<size_t> createVectorRepresentation(std::string phrase,
                                                                       int &status) const;
    void findNClosestWords(const std::vector<float> &target_word_representation,
                           std::vector<std::string> &closest_words,
                           std::vector<float> &closest_words_distances,
                           const size_t N = 20) const;
public:
    SemanticComparator();
    int init();
    double distance(const std::string &correct_string, const std::string &users_string);
};


#endif // SEMANTIC_H
