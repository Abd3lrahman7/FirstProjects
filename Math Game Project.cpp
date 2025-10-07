#include <iostream>
#include <string >
#include <cstdlib>
#include <ctime>
using namespace std;

enum enquestionslevel { easylevel = 1, medlevel = 2, hardlevel = 3, mix = 4 };

enum enoperationtype { add = 1, sub = 2, mult = 3, Div = 4, mixop = 5 };

string getoptypesymbol(enoperationtype optype)
{
    switch (optype)
    {
    case enoperationtype::add:
        return "+";
    case enoperationtype::sub:
        return "-";
    case enoperationtype::mult:
        return "*";
    case enoperationtype::Div:
        return "/";
    default:
        return "mix";
    }
}

string getquestionleveltext(enquestionslevel questionlevel)
{
    string arrquestionleveltext[4] = { "easy", "med","hard","mix" };
    return arrquestionleveltext[questionlevel - 1];
}

int RandomNumber(int From, int To)
{
    return rand() % (To - From + 1) + From;
}

void setscreencolor(bool right)
{
    if (right)
        system("color 2F");
    else
    {

        system("color 4F");
        cout << "\a";
    }

}

short readhowmanyquestions()
{
    short numberofquestions;
    do
    {
        cout << "how many questions do you want to answer ?";
        cin >> numberofquestions;
    } while (numberofquestions < 1 || numberofquestions > 10);
    return numberofquestions;
}

enquestionslevel readquestionslevel()
{

    short questionlevel = 0;
    do
    {
        cout << "enter question level [1] easy ,[2] med, [3] hard, [4] mix ? ";
        cin >> questionlevel;


    } while (questionlevel < 1 || questionlevel > 4);
    return (enquestionslevel)questionlevel;

}

enoperationtype readoptype()
{

    short optype;
    do
    {
        cout << "enter operation type [1] add, [2] sub, [3] mul, [4] div, [5] mix ? ";
        cin >> optype;

    } while (optype < 1 || optype >5);

    return (enoperationtype)optype;
}

struct stquestion
{
    int number1 = 0;
    int number2 = 0;
    enoperationtype operationtype;
    enquestionslevel questionlevel;
    int correctanswer = 0;
    int playeranswer = 0;
    bool answerresult = false;
};

struct stquizz
{
    stquestion questionlist[100];
    short numberofquestions;
    enquestionslevel questionslevel;
    enoperationtype optype;
    short numberofwronganswers = 0;
    short numberofrightanswers = 0;
    bool ispass = false;

};

int  simplecaculator(int number1, int number2, enoperationtype optype)
{
    switch (optype)
    {
    case enoperationtype::add:
        return number1 + number2;
    case enoperationtype::sub:
        return number1 - number2;
    case enoperationtype::mult:
        return number1 * number2;
    case enoperationtype::Div:
        return number1 / number2;
    default:
        return number1 + number2;
    }
}

enoperationtype getrandomoperationtype()
{
    int op = RandomNumber(1, 4);
    return (enoperationtype)op;

}

stquestion generatequestion(enquestionslevel questionlevel, enoperationtype optype)
{
    stquestion question;

    if (questionlevel == enquestionslevel::mix)
    {
        questionlevel = (enquestionslevel)RandomNumber(1, 3);

    }

    if (optype == enoperationtype::mixop)
    {
        optype = getrandomoperationtype();
    }

    question.operationtype = optype;

    switch (questionlevel)
    {
    case enquestionslevel::easylevel:
        question.number1 = RandomNumber(1, 10);
        question.number2 = RandomNumber(1, 10);

        question.correctanswer = simplecaculator(question.number1, question.number2, question.operationtype);
        question.questionlevel = questionlevel;
        return question;
    case enquestionslevel::medlevel:
        question.number1 = RandomNumber(10, 50);
        question.number2 = RandomNumber(10, 50);

        question.correctanswer = simplecaculator(question.number1, question.number2, question.operationtype);
        question.questionlevel = questionlevel;
        return question;
    case enquestionslevel::hardlevel:
        question.number1 = RandomNumber(50, 100);
        question.number2 = RandomNumber(50, 100);

        question.correctanswer = simplecaculator(question.number1, question.number2, question.operationtype);
        question.questionlevel = questionlevel;
        return question;


    }

    return question;
}

void generatequizzquestions(stquizz& quizz)
{
    for (short question = 0; question < quizz.numberofquestions; question++)
    {
        quizz.questionlist[question] = generatequestion(quizz.questionslevel, quizz.optype);
    }

}

int readquestionanswer()
{
    int answer = 0;
    cin >> answer;
    return answer;
}

void printthequestion(stquizz& quizz, short questionnumber)
{
    cout << "\n";
    cout << "question [" << questionnumber + 1 << "/" << quizz.numberofquestions << "]\n\n";
    cout << quizz.questionlist[questionnumber].number1 << endl;
    cout << quizz.questionlist[questionnumber].number2 << " ";
    cout << getoptypesymbol(quizz.questionlist[questionnumber].operationtype);
    cout << "\n_________" << endl;
}

void correcthequestionanswer(stquizz& quizz, short questionnumber)
{
    if (quizz.questionlist[questionnumber].playeranswer != quizz.questionlist[questionnumber].correctanswer)
    {
        quizz.questionlist[questionnumber].answerresult = false;
        quizz.numberofwronganswers++;
        cout << "worng answer :-( \n";
        cout << "the right answer is: ";
        cout << quizz.questionlist[questionnumber].correctanswer;
        cout << "\n";
    }
    else
    {
        quizz.questionlist[questionnumber].answerresult = true;
        quizz.numberofrightanswers++;
        cout << "right answer :-) \n";
    }
    cout << endl;
    setscreencolor(quizz.questionlist[questionnumber].answerresult);
}

void askandcorrectquestionlistranswers(stquizz& quizz)
{
    for (short questionnumber = 0; questionnumber < quizz.numberofquestions; questionnumber++)
    {
        printthequestion(quizz, questionnumber);
        quizz.questionlist[questionnumber].playeranswer = readquestionanswer();
        correcthequestionanswer(quizz, questionnumber);
    }
    quizz.ispass = (quizz.numberofrightanswers >= quizz.numberofwronganswers);
}

string getfinalresultstext(bool pass)
{
    if (pass)
        return "pass :-)";
    else
        return "fail :-(";
}

void  printquizzresults(stquizz quizz)
{
    cout << "\n";
    cout << "_________________________________\n\n";
    cout << "final resutls is " << getfinalresultstext(quizz.ispass);
    cout << "\n_________________________________\n\n";
    cout << "number of questions : " << quizz.numberofquestions << endl;
    cout << "questions level     : " << getquestionleveltext(quizz.questionslevel) << endl;
    cout << "optype              : " << getoptypesymbol(quizz.optype) << endl;
    cout << "number of right answers: " << quizz.numberofrightanswers << endl;
    cout << "number of wrong answers: " << quizz.numberofwronganswers << endl;
    cout << "_________________________________\n";
}

void playmathgame()
{
    stquizz quizz;
    quizz.numberofquestions = readhowmanyquestions();
    quizz.questionslevel = readquestionslevel();
    quizz.optype = readoptype();

    generatequizzquestions(quizz);
    askandcorrectquestionlistranswers(quizz);
    printquizzresults(quizz);
}

void resetscreen()
{
    system("cls");
    system("color 0F");
}

void startgame()
{
    char Playagin = 'y';

    do
    {
        resetscreen();
        playmathgame();

        cout << endl << "do you want to play again ? y/n ?";
        cin >> Playagin;

    } while (Playagin == 'Y' || Playagin == 'y');
}

int main()
{
    srand((unsigned)time(NULL));

    startgame();
}
