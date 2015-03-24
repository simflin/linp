//  This code is based on http://word2vec.googlecode.com/svn/trunk/distance.c
//  that is licensed under Apache License, version 2.0


#include "semantic.h"

//SemanticComparator::SemanticComparator()
//{
//}


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>

const long long MAX_SIZE = 2000;            // max length of strings
const long long CLOSEST_WORDS_NUMBER = 40;  // number of closest words that will be shown
const long long MAX_WORD_LENGTH = 50;       // max length of vocabulary entries


double dist(const std::string &correct_string, const std::string &users_string)
{
    const std::string WORD_PROJECTIONS_FILE_NAME = "./vectors.bin";

    FILE *word_projections_file = fopen(WORD_PROJECTIONS_FILE_NAME.c_str(), "rb");
    if (word_projections_file == NULL) {
        printf("Input file not found\n");
        return -1;
    }

    long long vocabulary_words_number, word_vector_size;
    fscanf(word_projections_file, "%lld", &vocabulary_words_number);
    fscanf(word_projections_file, "%lld", &word_vector_size);
    std::string vocabulary(vocabulary_words_number * MAX_WORD_LENGTH, '\0');

    std::vector<float> M(vocabulary_words_number * word_vector_size);

    for (long long b = 0; b < vocabulary_words_number; b++) {
        long long a = 0;
        while (1) {
            vocabulary[b * MAX_WORD_LENGTH + a] = fgetc(word_projections_file);
            if (feof(word_projections_file) || (vocabulary[b * MAX_WORD_LENGTH + a] == ' '))
                break;
            if ((a < MAX_WORD_LENGTH) && (vocabulary[b * MAX_WORD_LENGTH + a] != '\n'))
                a++;
        }
        vocabulary[b * MAX_WORD_LENGTH + a] = 0;

        for (a = 0; a < word_vector_size; a++) {
            fread(&M[a + b * word_vector_size], sizeof(float), 1, word_projections_file);
        }
        float len = 0;
        for (a = 0; a < word_vector_size; a++)
            len += M[a + b * word_vector_size] * M[a + b * word_vector_size];
        len = sqrt(len);
        for (a = 0; a < word_vector_size; a++)
            M[a + b * word_vector_size] /= len;
    }
    fclose(word_projections_file);

    long long aa = 0;
    char st1[MAX_SIZE];
    while (1) {
        st1[aa] = correct_string[aa];
        if ((st1[aa] == '\n') || (aa >= MAX_SIZE - 1)) {
            st1[aa] = 0;
            break;
        }
        aa++;
    }

    long long cn = 0;
    long long bb = 0;
    long long cc = 0;
    char st[100][MAX_SIZE];
    while (1) {
        st[cn][bb] = st1[cc];
        bb++;
        cc++;
        st[cn][bb] = 0;
        if (st1[cc] == 0)
            break;
        if (st1[cc] == ' ') {
            cn++;
            bb = 0;
            cc++;
        }
    }
    cn++;

    long long bi[100];
    long long b;
    for (long long a = 0; a < cn; a++) {
        for (b = 0; b < vocabulary_words_number; b++)
            if (!strcmp(&(vocabulary.c_str()[b * MAX_WORD_LENGTH]), st[a]))
                break;
        if (b == vocabulary_words_number)
            b = -1;
        bi[a] = b;
        //printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
        if (b == -1) {
            printf("Out of dictionary word!\n");
            break;
        }
    }
    if (b == -1) {
        printf("Can't process input!");
        return -1;
    }

    float vec[MAX_SIZE];
    for (long long a = 0; a < word_vector_size; a++)
        vec[a] = 0;
    for (long long d = 0; d < cn; d++) {
        if (bi[d] == -1)
            continue;
        for (long long a = 0; a < word_vector_size; a++)
            vec[a] += M[a + bi[d] * word_vector_size];
    }


    float len = 0;
    for (long long a = 0; a < word_vector_size; a++)
        len += vec[a] * vec[a];
    len = sqrt(len);
    for (long long a = 0; a < word_vector_size; a++)
        vec[a] /= len;


    std::vector<float> closest_words_distances(CLOSEST_WORDS_NUMBER, -1);

    std::string closest_words[CLOSEST_WORDS_NUMBER];
    for (long long a = 0; a < CLOSEST_WORDS_NUMBER; a++) {
        closest_words[a] = std::string(MAX_SIZE, '\0');
    }

    for (long long c = 0; c < vocabulary_words_number; c++) {
        long long a = 0;
        for (long long d = 0; d < cn; d++)
            if (bi[d] == c)
                a = 1;
        if (a == 1)
            continue;
        float dist = 0;
        for (long long a = 0; a < word_vector_size; a++)
            dist += vec[a] * M[a + c * word_vector_size];
        for (long long a = 0; a < CLOSEST_WORDS_NUMBER; a++) {
            if (dist > closest_words_distances[a]) {
                for (long long d = CLOSEST_WORDS_NUMBER - 1; d > a; d--) {
                    closest_words_distances[d] = closest_words_distances[d - 1];
                    closest_words[d] = closest_words[d - 1];
                }
                closest_words_distances[a] = dist;
                closest_words[a] = vocabulary.substr(c * MAX_WORD_LENGTH);
                break;
            }
        }
    }

    int counter = 0;
    for (long long  a = 0; a < CLOSEST_WORDS_NUMBER; a++) {
        if (closest_words[a] == users_string) {
            printf("Got it!\n");
            //printf("Resulting word is: %s\n",bestw[a]);
            printf("Cosine distance is: %f\n",closest_words_distances[a]);
            counter++;
            return closest_words_distances[a];
        }
    }
    if (counter == 0) {
        printf("Sorry, wrong guess!\n");
        return -1;
    }

    return -1;
}
