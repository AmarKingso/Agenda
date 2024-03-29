#include <iostream>
#include<algorithm>
#include "Meeting.hpp"
using namespace std;

Meeting::Meeting(const string &t_sponsor, const vector<string> &t_participator, const Date &t_startTime,
	const Date &t_endTime, const string &t_title) {
	m_sponsor = t_sponsor;
	m_participators = t_participator;
	m_startDate = t_startTime;
	m_endDate = t_endTime;
	m_title = t_title;
}

Meeting::Meeting(const Meeting &t_meeting) {
	m_sponsor = t_meeting.m_sponsor;
	m_participators = t_meeting.m_participators;
	m_startDate = t_meeting.m_startDate;
	m_endDate = t_meeting.m_endDate;
	m_title = t_meeting.m_title;
}

string Meeting::getSponsor(void) const {
	return m_sponsor;
}

void Meeting::setSponsor(const std::string &t_sponsor) {
	m_sponsor = t_sponsor;
}

vector<string> Meeting::getParticipator(void) const {
	return m_participators;
}

void Meeting::setParticipator(const vector<string> &t_participators) {
	m_participators = t_participators;
}
Date Meeting::getStartDate(void) const {
	return m_startDate;
}

void Meeting::setStartDate(const Date &t_startTime) {
	m_startDate = t_startTime;
}

Date Meeting::getEndDate(void) const {
	return m_endDate;
}

void Meeting::setEndDate(const Date &t_endTime) {
	m_endDate = t_endTime;
}

string Meeting::getTitle(void) const {
	return m_title;
}

void Meeting::setTitle(const string &t_title) {
	m_title = t_title;
}

bool Meeting::isParticipator(const string &t_username) const {
	for (auto i : m_participators) {
		if (i == t_username)
			return true;
	}
	return false;
}

void Meeting::addParticipator(const std::string &t_participator) {
	if (isParticipator(t_participator));
	else
		m_participators.push_back(t_participator);
}

void Meeting::removeParticipator(const std::string &t_participator) {
	if (isParticipator(t_participator)) {
		vector<string>::iterator itor = find(m_participators.begin(), m_participators.end(), t_participator);
		m_participators.erase(itor);
	}
}