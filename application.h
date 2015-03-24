#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <stdint.h>
#include <vector>

#include "database.h"
#include "semantic.h"
#include "speech_to_text.h"
#include "linp_exception.h"

class Application
{
    QString current_answer;
    QString current_question;
    Database *database;
    int QUESTIONS_NUMBER;
    std::vector<uint32_t> questions_numbers;
    size_t next_question_number_index;
    static const unsigned int GAME_SIZE = 10;
    unsigned int correct_answers_number;
    int last_correct_answer_index;

public:
    Application();
    ~Application();
    QString getNextQuestion() throw(ApplicationException);
    QString getCurrentAnswer() const;
    uint32_t getCurrentQuestionNumber() const;
    bool isAnswerCorrect(const QString &answer);
    void createNewGame();
    unsigned int getCorrectAnswersNumber() const;
    unsigned int getGameSize() const;
    int getCurrentQuestionIndex() const {
        return next_question_number_index;
    }
};

#endif // APPLICATION_H
