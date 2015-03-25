#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <stdint.h>
#include <vector>

#include "database.h"
#include "semantic.h"
#include "speech_to_text.h"
#include "linp_exception.h"

class LinpCore
{
    QString current_answer;
    QString current_question;
    Database *database;
    int QUESTIONS_NUMBER;
    static const double MIN_COS_VALUE = 0.8;
    std::vector<uint32_t> questions_numbers;
    int next_question_number_index;
    static const unsigned int GAME_SIZE = 10;
    unsigned int correct_answers_number;
    int last_correct_answer_index;
    SemanticComparator semantic_comparator;

public:
    LinpCore();
    ~LinpCore();
    QString getNextQuestion() throw(ApplicationException);
    QString getCurrentAnswer() const;
    uint32_t getCurrentQuestionNumber() const;
    bool isAnswerCorrect(QString answer);
    void createNewGame();
    unsigned int getCorrectAnswersNumber() const;
    unsigned int getGameSize() const;
    int getCurrentQuestionIndex() const {
        return next_question_number_index;
    }
};

#endif // APPLICATION_H
