//  This code is based on http://word2vec.googlecode.com/svn/trunk/distance.c
//  that is licensed under Apache License, version 2.0


#include "semantic.h"
#include <math.h>

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>


SemanticComparator::SemanticComparator()
{
}

int SemanticComparator::init()
{
    const std::string WORD_PROJECTIONS_FILE_NAME = "./vectors.bin";
    vocabulary_words_number = 0;
    word_vector_size = 0;

    FILE *word_projections_file = fopen(WORD_PROJECTIONS_FILE_NAME.c_str(), "rb");
    if (word_projections_file == NULL) {
        printf("Input file not found\n");
        return -1;
    }

    fscanf(word_projections_file, "%lld", &vocabulary_words_number);
    fscanf(word_projections_file, "%lld", &word_vector_size);

    // fill words_vectors and vocabulary
    words_vectors = std::vector<std::vector<float> >(vocabulary_words_number,
                                       std::vector<float>(word_vector_size));
    for (long long current_word_index = 0; current_word_index < vocabulary_words_number;
         current_word_index++) {

        std::string word;
        while (1) {
            char ch = fgetc(word_projections_file);
            if (feof(word_projections_file) || ch == ' ')
                break;
            if (ch == '\n')
                continue;
            word += ch;
        }
        vocabulary.push_back(word);

        for (long long a = 0; a < word_vector_size; a++) {
            fread(&(words_vectors[current_word_index][a]), sizeof(float), 1, word_projections_file);
        }
        normalize(words_vectors[current_word_index]);
    }
    fclose(word_projections_file);

    return 0;
}

void SemanticComparator::normalize(std::vector<float> &vector)
{
    float norm = sqrt(compute_vector_distance(vector, vector));
    if (norm < 0.001) { //FIXME
        return;
    }
    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] /= norm;
    }
    return;
}

float SemanticComparator::compute_vector_distance(const std::vector<float> &first_vector,
                                                 const std::vector<float> &second_vector)
{
    float distance = 0;
    for (size_t i = 0; i < first_vector.size(); i++) {
        distance += first_vector[i] * second_vector[i];
    }
    return distance;
}

std::vector<size_t> SemanticComparator::createVectorRepresentation(std::string phrase,
                                                                   int &status) const
{
    size_t lf_position = phrase.find('\n');
    if (lf_position != std::string::npos) {
        phrase.erase(lf_position);
    }

    std::vector<std::string> phrase_words;
    size_t space_position;
    while ((space_position = phrase.find(' ')) != std::string::npos) {
        phrase_words.push_back(phrase.substr(0, space_position));
        phrase.erase(0, space_position + 1);
    }
    phrase_words.push_back(phrase);

    std::vector<size_t> phrase_words_positions;
    for (size_t a = 0; a < phrase_words.size(); a++) {
        size_t b;
        for (b = 0; b < vocabulary.size(); b++) {
            if (vocabulary[b] == phrase_words[a]) {   // FIXME: toLower
                break;
            }
        }
        if (b == vocabulary.size()) {
            std::cout << "Out of dictionary word!\nCan't process input!\n";
            status = -1;
            return std::vector<size_t>();
        }
        phrase_words_positions.push_back(b);
    }

    status = 0;
    return phrase_words_positions;

    // FIXME this code leads to segfault ????
//    std:;vector<float> phrase_representation(word_vector_size, 0);
//    for (size_t i = 0; i < phrase_words_positions.size(); ++i) {
//        for (size_t j = 0; j < word_vector_size; ++j) {
//            phrase_representation[j] += words_vectors[phrase_words_positions[i]][j];
//        }
//    }
//    return phrase_representation;
}

void SemanticComparator::findNClosestWords(const std::vector<float> &target_word_representation,
                                           std::vector<std::string> &closest_words,
                                           std::vector<float> &closest_words_distances,
                                           const size_t N) const
{
    closest_words_distances = std::vector<float> (N, -1);
    closest_words = std::vector<std::string>(N);
    for (long long c = 0; c < vocabulary_words_number; c++) {
        // COMMENTED OUT: sense of this code is not obvious;
        // however after refactoring we have no information about words in target phrase
        //bool is_vocabulary_word_in_correct_string = false;
        //for (long long d = 0; d < correct_string_words_positions.size(); d++) {
        //    if (correct_string_words_positions[d] == c) {
        //        is_vocabulary_word_in_correct_string = true;
        //        break;
        //    }
        //}
        //if (is_vocabulary_word_in_correct_string) {
        //    continue;
        //}
        float dist = compute_vector_distance(target_word_representation, words_vectors[c]);
        for (long long a = 0; a < closest_words.size(); a++) {
            if (dist > closest_words_distances[a]) {
                for (long long d = closest_words.size() - 1; d > a; d--) {
                    closest_words_distances[d] = closest_words_distances[d - 1];
                    closest_words[d] = closest_words[d - 1];
                }
                closest_words_distances[a] = dist;
                closest_words[a] = vocabulary[c];
                break;
            }
        }
    }
    return;
}

double SemanticComparator::distance(const std::string &correct_string,
                                    const std::string &users_string)
{
    int status = 0;
    std::vector<size_t> correct_string_words_positions = createVectorRepresentation(correct_string,
                                                                                    status);
    if (status != 0) {
        return -1;
    }
    std::vector<float> correct_string_representation(word_vector_size, 0);
    for (size_t i = 0; i < correct_string_words_positions.size(); ++i) {
        for (size_t j = 0; j < word_vector_size; ++j) {
            correct_string_representation[j] += words_vectors[correct_string_words_positions[i]][j];
        }
    }

    normalize(correct_string_representation);

    correct_string_words_positions = createVectorRepresentation(users_string, status);
    if (status != 0) {
        return -1;
    }
    std::vector<float> users_string_vector_repr(word_vector_size, 0);
    for (size_t i = 0; i < correct_string_words_positions.size(); ++i) {
        for (size_t j = 0; j < word_vector_size; ++j) {
            users_string_vector_repr[j] += words_vectors[correct_string_words_positions[i]][j];
        }
    }

    normalize(users_string_vector_repr);

    return compute_vector_distance(correct_string_representation, users_string_vector_repr);
}
