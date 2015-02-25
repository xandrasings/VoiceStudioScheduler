// Xandra Best
// 1/23/15
// Classes of Scheduler Application

#ifndef _CLASSESCONTENT_
#define _CLASSESCONTENT_
#include <iostream>
#include <vector>
#include "Classes.h"
using namespace std;

/* Weekday */
bool operator<(const Weekday& lhs, const Weekday& rhs) {
  return (lhs.avPreference > rhs.avPreference);
}

Weekday::Weekday (string d) {
  day = d;
  count = 1;
  preferenceTotal = preferenceCount = 0;
  avPreference = 0;
  assigned = false;
}

void Weekday::print () {
  cout << "Day: " << day << endl;
  cout << "Count: " << count << endl;
  cout << "Pref Total: " << preferenceTotal << endl;
  cout << "Pref Count: " << preferenceCount << endl;
  cout << "Pref Av: " << avPreference << endl;
  cout << endl;
}

void Weekday::calcPref () {
  avPreference = ((float)preferenceTotal)/((float)preferenceCount);
}

/* Week */
void Week::add (Weekday w) {
  fullWeek.push_back(w);
}

void Week::calcPref () {
  for (unsigned int i = 0; i < fullWeek.size(); i++) {
    fullWeek[i].calcPref();
  }
}

void Week::sortWeek () {
  sort(fullWeek.begin(), fullWeek.end());
}

void Week::print () {
  for (unsigned int i = 0; i < fullWeek.size(); i++) {
    fullWeek[i].print();
  }
}

int Week::nextDay () {
  float min = 10;
  int ref = -1;
  for (unsigned int i = 0; i < fullWeek.size(); i++) {
    if (fullWeek[i].avPreference < min && (!fullWeek[i].assigned)) {
      min = fullWeek[i].avPreference;
      ref = i;
    }
  }
  return ref;
}

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
bool operator<(const Student& lhs, const Student& rhs) {
  return (lhs.sortablePreference > rhs.sortablePreference);
}


Student::Student (string n) {
  name = n;
  assigned = false;
}


Student::Student (string n, int nd) {
  name = n;
  for (int i = 0; i < nd; i++) {
    preferenceByDay.push_back(0);
  }
  assigned = false;
}

void Student::checkAvailability (TimeList& myList) {
  int available = 0;
  cout << "Is " << name << " available at the following times?" << endl;
  cout << "(Type '1' for yes or '0' for no.)" << endl;
  for (unsigned int i = 0; i < myList.timeList.size(); i++) {
    myList.timeList[i].print();
    cin >> available;
    if (available != 0) {
      availability.push_back(& myList.timeList[i]);
    }
    //make a protective loop here
    available = 'n';
  }
}

void Student::print () {
  cout << name;
}

void Student::printAvailability () {
  if (assigned) {
    cout << "ASSIGNED!" << endl;
  }
  else {
    cout << "NOT assigned" << endl;
  }
  for (unsigned int i = 0; i < availability.size(); i++) {
    //cout << preference[i] << " ";
    availability[i]->print();
  }

  for (unsigned int i = 0; i < preferenceByDay.size(); i++) {
    cout << preferenceByDay[i] << ", ";
  }
}

void Student::setPreference (int dayRef) {
  sortablePreference = preferenceByDay[dayRef];
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
    cout << "Student " << studentList[i].name << " needs ";
    if (!studentList[i].hourLong) {
      cout << "half  ";
    }
    cout << "hour long lessons, and is available at the following times: " << endl;
    studentList[i].printAvailability();
    cout << endl;
  }
}

void StudentList::setPreference (int dayRef) {
  for (unsigned int i = 0; i < studentList.size(); i++) {
    studentList[i].setPreference (dayRef);
  }
}

void StudentList::sortByDay (int dayRef) {
  setPreference (dayRef);
  sort(studentList.begin(), studentList.end());
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
      preferenceRank = student->preference[i];
      availCheck = true;
      break;
    }
  }
  availability = availCheck;
}

void Assignment::print () {
  //cout << preferenceRank << " ";
  cout << student->name << " on ";
  time->print();
  //cout << "Available: " << availability << endl;
}

void Assignment::clear () {
  preferenceRank = 0;
  availability = 0;
}

/* Schedule */
bool operator<(const Schedule& lhs, const Schedule& rhs) {
  return (lhs.preferenceRank > rhs.preferenceRank);
}

void Schedule::add (Assignment newAssignment) {
  schedule.push_back(newAssignment);
}

void Schedule::print () {
  cout << "Average assignment rating: " << preferenceRank << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  for (unsigned int i = 0; i < schedule.size(); i++) {
    schedule[i].print();
  }
  cout << endl;
}

void Schedule::clear () {
  schedule.clear();
  preferenceRank = 0;
}

/* ScheduleGroup */
void ScheduleGroup::add (Schedule newSchedule) {
  scheduleGroup.push_back(newSchedule);
}

void ScheduleGroup::print (int n) {
  if (scheduleGroup.size() == 0) {
    //cout << "NO VIABLE SCHEDULES" << endl;
    return;
  }
  unsigned int i = 0;
  if (n == -2) {
    float preferenceRank = scheduleGroup[0].preferenceRank;
    //cout << "Top schedules: " << endl;
    cout << "Top " << scheduleGroup[0].schedule[0].time->weekDay << " schedules: " << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    int count = 0;
    while (scheduleGroup[i].preferenceRank == preferenceRank) {
      count++;
      cout << "Schedule #" << i+1 << endl;
      scheduleGroup[i].print();
      i++;
      if (count >= 5) {
        break;
      }
    }
    //cout << endl << endl;
    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    cout << "showing top " << i;
  }
  else if (n == -3) {
    float preferenceRank = scheduleGroup[0].preferenceRank;
    //cout << "Top schedules: " << endl;
    cout << "Top " << scheduleGroup[0].schedule[0].time->weekDay << " schedules: " << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    int count = 0;
    while (scheduleGroup[i].preferenceRank == preferenceRank) {
      count++;
      cout << "Schedule #" << i+1 << endl;
      scheduleGroup[i].print();
      i++;
      if (count >= 50) {
	break;
      }
    }
    //cout << endl << endl;
    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    cout << "showing top " << i << " of " << scheduleGroup.size() << " viable results" << endl;
  }
  else {
    if (n == -1) {
      n = scheduleGroup.size();
    }
    if (n > scheduleGroup.size()) {
      n = scheduleGroup.size();
    }
    if (n > 0) {
      cout << n << " viable schedules: " << endl;
      cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
      for (i = 0; i < n; i++) {
	cout << "Schedule #" << i << endl;
	scheduleGroup[i].print();
      }
      cout << endl << endl;
      //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
      //cout << "printed top " << i << " schedules." << endl;
    }
    else {
      cout << "No viable schedules :[" << endl;
    }
  }
}

bool checkHourSequence (Assignment& firstAssignment, Assignment& secondAssignment) {
  bool dayCheck, timeCheck;
  dayCheck = (firstAssignment.time->weekDay == secondAssignment.time->weekDay);
  if (
      firstAssignment.time->hour == secondAssignment.time->hour ||
      (firstAssignment.time->hour + 1)%12 == (secondAssignment.time->hour)%12)
    {
      if ((firstAssignment.time->minute + 30)%60 == (secondAssignment.time->minute)%60) {
	timeCheck = true;
      }
      else {
	timeCheck = false;
      }
    }
  else {
    timeCheck = false;
  }
  return (dayCheck && timeCheck);
}


#endif // _CLASSESCONTENT_
