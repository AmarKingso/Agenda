#include <iostream>
#include "Date.hpp"
using namespace std;

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(const string &dateString) {
	bool valid;
	if (dateString.length() == 16) {
		int i = 0;
		for (i = 0; i < 16; i++) {
			if (dateString[4] != '-' || dateString[7] != '-' || dateString[10] != '/' || dateString[13] != ':') {
				valid = false;
				break;
			}
			if (i != 4 && i != 7 && i != 10 && i != 13) {
				if (!isdigit(dateString[i])) {
					valid = false;
					break;
				}
			}	
		}
		if (i == 16)
			valid = true;
	}
	else
		valid = false;
	if (valid) {
		string str_year = dateString.substr(0, 4);
		string str_month = dateString.substr(5, 2);
		string str_day = dateString.substr(8, 2);
		string str_hour = dateString.substr(11, 2);
		string str_minute = dateString.substr(14, 2);
		m_year = stoi(str_year);
		m_month = stoi(str_month);
		m_day = stoi(str_day);
		m_hour = stoi(str_hour);
		m_minute = stoi(str_minute);
		if (!Date::isValid(*this)) {
			m_year = 0;
			m_month = 0;
			m_day = 0;
			m_hour = 0;
			m_minute = 0;
		}
	}
	else {
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_hour = 0;
		m_minute = 0;
	}
}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date &t_date) {
	if (t_date.m_year > 9999 || t_date.m_year < 1000)
		return false;
	if (t_date.m_day < 1)
		return false;
	if (t_date.m_hour > 23 || t_date.m_hour < 0)
		return false;
	if (t_date.m_minute > 59 || t_date.m_minute < 0)
		return false;
	switch (t_date.m_month) {
	case 1:case 3:case 5:case 7:case 8:case 10:case 12:
		if (t_date.m_day > 31)
			return false;
		break;
	case 4:case 6:case 9:case 11:
		if (t_date.m_day >30)
			return false;
		break;
	case 2:
		if ((t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0) || t_date.m_year % 400 == 0) {
			if (t_date.m_day > 29)
				return false;
		}
		else {
			if (t_date.m_day > 28)
				return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

Date Date::stringToDate(const string &t_dateString) {
		return Date(t_dateString);
}

string Date::dateToString(const Date &t_date) {
	string str_date = "";
	if (Date::isValid(t_date)) {
		str_date += to_string(t_date.m_year) + "-";
		if (t_date.m_month < 10)
			str_date += "0" + to_string(t_date.m_month) + "-";
		else
			str_date += to_string(t_date.m_month) + "-";
		if (t_date.m_day < 10)
			str_date += "0" + to_string(t_date.m_day) + "/";
		else
			str_date += to_string(t_date.m_day) + "/";
		if (t_date.m_hour < 10)
			str_date += "0" + to_string(t_date.m_hour) + ":";
		else
			str_date += to_string(t_date.m_hour) + ":";
		if (t_date.m_minute < 10)
			str_date += "0" + to_string(t_date.m_minute);
		else
			str_date += to_string(t_date.m_minute);
		return str_date;
	}
	else {
		return "0000-00-00/00:00";
	}
}

Date &Date::operator=(const Date &t_date) {
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
	return *this;
}

bool Date::operator==(const Date &t_date) const {
	if (m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour == t_date.m_hour && m_minute == t_date.m_minute)
		return true;
	else
		return false;
}

bool Date::operator>(const Date &t_date) const {
	if (m_year < t_date.m_year)
		return false;
	if (m_year > t_date.m_year)
		return true;
	else {
		if (m_month < t_date.m_month)
			return false;
		if (m_month > t_date.m_month)
			return true;
		else {
			if (m_day < t_date.m_day)
				return false;
			if (m_day > t_date.m_day)
				return true;
			else {
				if (m_hour < t_date.m_hour)
					return false;
				if (m_hour > t_date.m_hour)
					return true;
				else {
					if (m_minute <= t_date.m_minute)
						return false;
					else
						return true;
				}
			}
		}
	}
}

bool Date::operator<(const Date &t_date) const {
	if (m_year > t_date.m_year)
		return false;
	if (m_year < t_date.m_year)
		return true;
	else {
		if (m_month > t_date.m_month)
			return false;
		if (m_month < t_date.m_month)
			return true;
		else {
			if (m_day > t_date.m_day)
				return false;
			if (m_day < t_date.m_day)
				return true;
			else {
				if (m_hour > t_date.m_hour)
					return false;
				if (m_hour < t_date.m_hour)
					return true;
				else {
					if (m_minute >= t_date.m_minute)
						return false;
					else
						return true;
				}
			}
		}
	}
}

bool Date::operator>=(const Date &t_date) const {
	if (m_year < t_date.m_year)
		return false;
	if (m_year > t_date.m_year)
		return true;
	else {
		if (m_month < t_date.m_month)
			return false;
		if (m_month > t_date.m_month)
			return true;
		else {
			if (m_day < t_date.m_day)
				return false;
			if (m_day > t_date.m_day)
				return true;
			else {
				if (m_hour < t_date.m_hour)
					return false;
				if (m_hour > t_date.m_hour)
					return true;
				else {
					if (m_minute < t_date.m_minute)
						return false;
					else
						return true;
				}
			}
		}
	}
}

bool Date::operator<=(const Date &t_date) const {
	if (m_year > t_date.m_year)
		return false;
	if (m_year < t_date.m_year)
		return true;
	else {
		if (m_month > t_date.m_month)
			return false;
		if (m_month < t_date.m_month)
			return true;
		else {
			if (m_day > t_date.m_day)
				return false;
			if (m_day < t_date.m_day)
				return true;
			else {
				if (m_hour > t_date.m_hour)
					return false;
				if (m_hour < t_date.m_hour)
					return true;
				else {
					if (m_minute > t_date.m_minute)
						return false;
					else
						return true;
				}
			}
		}
	}
}