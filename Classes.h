// Xandra Best
// 1/23/15
// Header for Classes of Scheduler Application

#ifndef _CLASSES_
#define _CLASSES_
#include <vector>
using namespace std;

/* Time */
class Time {
public:
	string weekDay;
	int hour;
	int minute;
public:
	Time ();
	Time (string, int, int);
	void print ();
};

/* TimeList */
class TimeList {
public:
	vector<Time> timeList;
public:
	void add (Time);
	void print ();
};

/* Student */
class Student {
public:
	string name;
	vector<Time*> availability;
public:
	Student (string);
	void checkAvailability (TimeList&);
	void printAvailability ();
};

/* StudentList */
class StudentList {
public:
	vector<Student> studentList;
public:
	void add (Student);
	void checkAvailability (TimeList&);
	void printAvailability ();
};

/* Assignment */
class Assignment {
public:
	Time* time;
	Student* student;
	bool availability;
public:
	Assignment ();
	Assignment (Time&, Student&);
	void setArgs (Time&, Student&);
	void checkAvailability ();
	void print ();
};

/* Schedule */
class Schedule {
public:
	vector<Assignment> schedule;
public:
	void add (Assignment);
	void print ();
};

/* ScheduleGroup */
class ScheduleGroup {
public:
	vector<Schedule> scheduleGroup;
public:
	void add (Schedule);
	void print ();
};

/***Matches***/
/*
class Match {
protected:
	int matchVal;
	int xCoor;
	int yCoor;
public:
	Match();
	Match(int val, int x, int y);
	Match& operator=(const Match& m);
	int getMatchVal();
	int getXCoor() const;
	int getYCoor() const;
	void print();
	void halfPrint();

	friend ostream& operator<<(ostream& os, const Match& m);
};
*/

/***Combinations of Matches***/
/*
class Combo {
public: //change to protected when you fix the thing
	vector<Match> comboVec;
	float score;
	bool badCombo = false;
public:
	Combo();
	int size();
	float getScore() const;
	void setScore(float newScore);
	void calcScore(float multiplier);
	void add(Match newMatch);
	Match at(size_t index);
	void print();
	void printWithScore();
	void printWithSequence(string str1, int num);
	void printAlignment(string str1, string str2, int num);
	void sort();
	bool getBadCombo() const;
	void setBadCombo();
	Combo& operator=(const Combo& c);

	friend ostream& operator<<(ostream& os, const Combo& c);
};
*/

/***Combinations of Combinations of Matches***/
/*
class Catalog {
public: //change to protected when you fix the thing
	vector<Combo> catalogVec;
public:
	Catalog();
	int size();
	void calcScores(float multiplier = .15);
	void add(Combo newCombo);
	void erase(int i);
	void sort();
	Combo at(size_t index);
	void print();
	void printWithScores();
	void finalPrint(string str1, string str2, int num, int results);

	friend ostream& operator<<(ostream& os, const Catalog& c);
};
*/
#endif // _CLASSES_
