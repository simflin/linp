#include "database.h"
#include <iostream>
#include <QMessageBox>

const char *Database::PATH_TO_DATABASE = "./clues.db";

Database::Database() throw(DatabaseException)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(PATH_TO_DATABASE);
    if (!database.open()) {
        throw DatabaseException("Can't open the database with questions.");
    }
}
Database::~Database()
{
    database.close();
}

uint32_t Database::getQuestionsNumber() const throw(DatabaseException)
{
    QSqlQuery query("SELECT count(answer) FROM documents");
    if (!query.isActive() || !query.first()) {
        throw DatabaseException("Can't count questions in database. Something wrong with database.");
        return -1;
    }

    return query.value(0).toInt();
}
QuestionPair Database::getQuestionPair(uint32_t number)
{
    QSqlQuery query("SELECT clue, answer FROM documents WHERE id = " + QString::number(number));
    if (!query.isActive() || !query.first()) {
        std::cout << "ERROR!!! " << number << std::endl; //FIXME
        return QuestionPair("", "");
    }
    QSqlRecord record = query.record();

    return QuestionPair(query.value(record.indexOf("clue")).toString(),
                        query.value(record.indexOf("answer")).toString());
}

