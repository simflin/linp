#include "linp_core.h"
#include <QtGlobal>
#include <set>
#include <iostream>

LinpCore::LinpCore()
{
    this->database = new Database();
    this->QUESTIONS_NUMBER = this->database->getQuestionsNumber();
    semantic_comparator.init();

    createNewGame();
}

LinpCore::~LinpCore()
{
    delete this->database;
}

void LinpCore::createNewGame()
{
    this->questions_numbers.clear();
    std::set<int> numbers;
    while (numbers.size() < this->GAME_SIZE) { // FIXME: infinite loop is possible (in worst case)
        numbers.insert(qrand() % this->QUESTIONS_NUMBER + 1);
    }
    this->questions_numbers.insert(this->questions_numbers.end(), numbers.begin(), numbers.end());
    this->next_question_number_index = 0;
    this->current_question = "";
    this->current_answer = "";
    this->correct_answers_number = 0;
    this->last_correct_answer_index = -1;
}

uint32_t LinpCore::getCurrentQuestionNumber() const
{
    if (next_question_number_index) {
        return questions_numbers[next_question_number_index - 1];
    } else {
        return 0;
    }
}

unsigned int LinpCore::getCorrectAnswersNumber() const
{
    return this->correct_answers_number;
}

unsigned int LinpCore::getGameSize() const
{
    return this->GAME_SIZE;
}

QString LinpCore::getNextQuestion() throw(ApplicationException)
{
    if (next_question_number_index == GAME_SIZE) {
        throw ApplicationException("The game is over", false);
    }
    uint32_t next_question_number = questions_numbers[next_question_number_index];
    QuestionPair question_pair = this->database->getQuestionPair(next_question_number);
    this->next_question_number_index += 1;
    this->current_answer = question_pair.answer;
    this->current_question = question_pair.question;
    return this->current_question;
}

QString LinpCore::getCurrentAnswer() const
{
    return this->current_answer;
}

bool LinpCore::isAnswerCorrect(const QString &answer)
{
    if (this->current_answer.length() == 0 || answer.length() == 0) {
        return false;
    }

    double res = semantic_comparator.distance(this->current_answer.toLower().toStdString(),
                                              answer.toLower().toStdString());

    std::cout << "RES: " << res << std::endl;
    if (res > MIN_COS_VALUE || res == -1 && this->current_answer.toLower() == answer.toLower()) {
//    if (this->current_answer.toLower() == answer.toLower()) {
        if (this->next_question_number_index > 0 &&
                (this->last_correct_answer_index == -1 ||
                    this->last_correct_answer_index != this->next_question_number_index - 1)) {
            this->last_correct_answer_index = this->next_question_number_index - 1;
            this->correct_answers_number += 1;
        }
        return true;
    }
    return false;
}
