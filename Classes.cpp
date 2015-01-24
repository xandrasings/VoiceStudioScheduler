// Xandra Best
// 1/23/15
// Classes of Scheduler Application

#include <iostream>
#include <vector>
#include "classes.h"
using namespace std;

/* Time */
Time::Time () {
	weekDay = "Monday";
	hour = 12;
	minute = 30;
}

Time::Time (string w, int h, int m) {
	weekDay = w;
	hour = h;
	minute = m;
}

bool operator==(const Time& lhs, const Time& rhs) {
	return (
		lhs.weekDay == rhs.weekDay &&
		lhs.hour == rhs.hour &&
		lhs.minute == rhs.minute);
}

void Time::print () {
	cout << weekDay << " at " << hour << ":";
	if (minute == 0) {
		cout << "00";
	}
	else {
		cout << minute;
	}
	cout << endl;
}

/* TimeList */
void TimeList::add(Time newTime) {
	timeList.push_back(newTime);	
}

void TimeList::print() {
	cout << "All times: " << endl;
	for (unsigned int i = 0; i < timeList.size(); i++) {
		timeList[i].print();
	}
}

/* Student */
Student::Student (string n) {
	name = n;
}

void Student::checkAvailability (TimeList& myList) {
	char available = 'n';
	cout << "(Type 'y' for yes or 'n' for no.)" << endl;
	for (unsigned int i = 0; i < myList.timeList.size(); i++) {
		cout << "Is " << name << " available at the following time?" << endl;
		myList.timeList[i].print();
		cin >> available;
		if (available == 'y') {
			availability.push_back(& myList.timeList[i]);
		}
		//make a protective loop here
		available = 'n';
	}
}

void Student::printAvailability () {
	for (unsigned int i = 0; i < availability.size(); i++) {
		availability[i]->print();
	}
}

/* Student List */
void StudentList::add (Student newStudent) {
	studentList.push_back(newStudent);
}

void StudentList::checkAvailability (TimeList& myList) {
	for (unsigned int i = 0; i < studentList.size(); i++) {
		studentList[i].checkAvailability(myList);
		cout << endl;
	}
}

void StudentList::printAvailability () {
	for (unsigned int i = 0; i < studentList.size(); i++) {
		cout << "Student " << studentList[i].name << " is available at the following times: " << endl;
		studentList[i].printAvailability();
		cout << endl;
	}
}

/* Assignment */
Assignment::Assignment () {}

Assignment::Assignment (Time& t, Student& s) {
	time = &t;
	student = &s;
	availability = false;
}

void Assignment::setArgs (Time& t, Student& s) {
	time = &t;
	student = &s;
	availability = false;
}

void Assignment::checkAvailability () {
	bool availCheck = false;
	for (unsigned int i = 0; i < student->availability.size(); i++) {
		if (*time == *(student->availability[i])) {
			availCheck = true;
			break;
		}
	}
	availability = availCheck;
}

void Assignment::print () {
	cout << student->name << " on ";
	time->print();
	//cout << "Available: " << availability << endl;
}

/* Schedule */
void Schedule::add (Assignment newAssignment) {
	schedule.push_back(newAssignment);
}

void Schedule::print () {
	for (unsigned int i = 0; i < schedule.size(); i++) {
		schedule[i].print();
	}
}

/* ScheduleGroup */
void ScheduleGroup::add (Schedule newSchedule) {
	scheduleGroup.push_back(newSchedule);
}

void ScheduleGroup::print () {
	for (unsigned int i = 0; i < scheduleGroup.size(); i++) {
		scheduleGroup[i].print();
		cout << endl;
	}
}
/***Matches***/
/*
Match::Match() {
	matchVal = 0;
	xCoor = -1;
	yCoor = -1;
}

Match::Match(int val, int x, int y){
	matchVal = val;
	xCoor = x;
	yCoor = y;
}

bool operator==(const Match& lhs, const Match& rhs) { return lhs.getXCoor() == rhs.getXCoor(); }
bool operator!=(const Match& lhs, const Match& rhs) { return !operator==(lhs, rhs); }
bool operator< (const Match& lhs, const Match& rhs) { return lhs.getXCoor() < rhs.getXCoor(); }
bool operator> (const Match& lhs, const Match& rhs) { return operator< (rhs, lhs); }
bool operator<=(const Match& lhs, const Match& rhs) { return !operator> (lhs, rhs); }
bool operator>=(const Match& lhs, const Match& rhs) { return !operator< (lhs, rhs); }

Match& Match::operator=(const Match& m){
	matchVal = m.matchVal;
	xCoor = m.xCoor;
	yCoor = m.yCoor;
	return *this;
}

int Match::getMatchVal(){
	return matchVal;
}

int Match::getXCoor() const {
	return xCoor;
}

int Match::getYCoor() const {
	return yCoor;
}

void Match::print(){
	cout << *this << endl;
}

void Match::halfPrint(){
	cout << matchVal << " (" << xCoor << "," << yCoor << ") ";
}

ostream& operator<<(ostream& os, const Match& m) {
    os << m.matchVal << " (" << m.xCoor << "," << m.yCoor << ")";
    return os;
}
*/

/***Combinations***/
/*
Combo::Combo() {}

bool operator==(const Combo& lhs, const Combo& rhs) { return lhs.getScore() == rhs.getScore(); }
bool operator!=(const Combo& lhs, const Combo& rhs) { return !operator==(lhs, rhs); }
bool operator< (const Combo& lhs, const Combo& rhs) { return lhs.getScore() < rhs.getScore(); }
bool operator> (const Combo& lhs, const Combo& rhs) { return operator< (rhs, lhs); }
bool operator<=(const Combo& lhs, const Combo& rhs) { return !operator> (lhs, rhs); }
bool operator>=(const Combo& lhs, const Combo& rhs) { return !operator< (lhs, rhs); }

int Combo::size(){
	return comboVec.size();
}

float Combo::getScore() const {
	return score;
}

void Combo::setScore(float newScore){
	score = newScore;
}

void Combo::calcScore(float multiplier) {
	float matches = (float)(size()); //number of matches (pos)
	float sum = 0; //unimportant
	for (Match match : comboVec) {
		sum += (float)(match.getMatchVal());
	}
	float mean = sum / matches; //average match value
	float error = 0; //aggregate error from mean
	for (Match match : comboVec) {
		error += std::abs((float)(match.getMatchVal()) - mean);
	}

	setScore(matches - multiplier * error);
}

void Combo::add(Match newMatch) {
	comboVec.push_back(newMatch);
}

Match Combo::at(size_t index) {
	return comboVec.at(index);
}

void Combo::print() {
	cout << *this;
}

void Combo::printWithScore() {
	cout << *this << endl;
	cout << score << endl;
}

void Combo::printWithSequence(string str1, int num) {
	for(unsigned int i = 0; i < size(); i++) {
		comboVec.at(i).halfPrint();
		for(unsigned int j = 0; j < num; j++) {
			cout << str1[comboVec.at(i).getXCoor()+j];
		}
	cout << endl;
	}
}

void Combo::printAlignment(string str1, string str2, int num) {

	
}

void Combo::sort() {
	::sort(comboVec.begin(), comboVec.end(), less<Match>());
}

bool Combo::getBadCombo() const{
	return badCombo;
}

void Combo::setBadCombo(){
	badCombo = true;
}

Combo& Combo::operator=(const Combo& c){
	score = c.getScore();
	if (comboVec.size() > c.comboVec.size()){
		comboVec.resize(c.comboVec.size());
	}
	int i;
	for (i = 0; i < comboVec.size() - 1; i++) {
		comboVec[i]=c.comboVec[i];
	}
	for (; i < c.comboVec.size(); i++) {
		comboVec.push_back(c.comboVec[i]);
	}
	return *this;
}

ostream& operator<<(ostream& os, const Combo& c) {
	for (Match match : c.comboVec) {
		os << match << endl;
	}
	return os;
}
*/

/***Catalogs***/
/*
Catalog::Catalog() {}

int Catalog::size(){
	return catalogVec.size();
}

void Catalog::calcScores(float multiplier) {
	for(unsigned int i = 0; i < size(); i++) {
		catalogVec.at(i).calcScore(multiplier);
	}
}

void Catalog::add(Combo newCombo) {
	catalogVec.push_back(newCombo);
}

void Catalog::erase(int i) {
	catalogVec.erase(catalogVec.begin()+i);
}



void Catalog::sort() {
	::sort(catalogVec.begin(), catalogVec.end(), greater<Combo>());
}


Combo Catalog::at(size_t index) {
	return catalogVec.at(index);
}

void Catalog::print(){
	cout << *this;
}

void Catalog::printWithScores() {
	size_t i = 0;
	for (Combo combo : catalogVec) {
		cout << "COMBO " << i++ << " with SCORE " << combo.score << endl;
		cout << combo << endl;
	}
}

void Catalog::finalPrint(string str1, string str2, int num, int results) {
	for (unsigned int i = 0; i < min(size(), results); i++) {
	//for (Combo combo : catalogVec) {
		cout << "COMBO " << i+1 << " with score of " << catalogVec[i].score << ":" << endl;
		catalogVec[i].printWithSequence(str1, num);
		cout << endl;
		catalogVec[i].printAlignment(str1, str2, num);
		cout << endl;
	}
}

ostream& operator<<(ostream& os, const Catalog& c) {
	size_t i = 0;
	for (Combo combo : c.catalogVec) {
		os << "COMBO " << i++ << endl;
		os << combo << endl;
	}
	return os;
}
*/