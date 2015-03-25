#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>
#include <QString>
#include <QtSql>
#include "linp_exception.h"


struct QuestionPair {
    QString question;
    QString answer;

    QuestionPair(const QString &q, const QString & a) : question(q), answer(a) {}
    QuestionPair() {}
};

class Database
{
    static const char *PATH_TO_DATABASE;
    QSqlDatabase database;

public:
    Database() throw(DatabaseException);
    ~Database();
    uint32_t getQuestionsNumber() const throw(DatabaseException);
    QuestionPair getQuestionPair(uint32_t number);
};

#endif // DATABASE_H
