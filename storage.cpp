#include<iostream>
#include<fstream>
#include<algorithm>
#include"Storage.hpp"
#include"Path.hpp"
using namespace std;

shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
	m_dirty = false;
	readFromFile();
}

bool Storage::readFromFile(void) {
	m_userList.clear();
	m_meetingList.clear();
	ifstream user_in;
	ifstream meeting_in;
	string msg;
	user_in.open(Path::userPath);
	meeting_in.open(Path::meetingPath);
	if (!(user_in.is_open() && meeting_in.is_open()))
		return false;
	while (getline(user_in, msg)) {
		User create_user;
		string sub_msg = "";
		bool is_msg = 0;
		int flag = 0;
		for (int i = 0; i < msg.length(); i++) {
			if (msg[i] == '\"' && is_msg) {
				is_msg = false;
				if (flag == 0)
					create_user.setName(sub_msg);
				else if (flag == 1)
					create_user.setPassword(sub_msg);
				else if (flag == 2)
					create_user.setEmail(sub_msg);
				else
					create_user.setPhone(sub_msg);
				sub_msg = "";
				flag++;
				continue;
			}
			if (is_msg)
				sub_msg += msg[i];
			if (msg[i] == '\"' && !is_msg)
				is_msg = true;
		}
		m_userList.push_back(create_user);
	}
	user_in.close();

	while (getline(meeting_in, msg)) {
		Meeting create_meeting;
		string sub_msg = "";
		bool is_msg = 0;
		int flag = 0;
		for (int i = 0; i < msg.length(); i++) {
			if (msg[i] == '\"' && is_msg) {
				is_msg = false;
				if (flag == 0)
					create_meeting.setSponsor(sub_msg);
				else if (flag == 1) {
					vector<string> the_participators;
					string every_participator = "";
					for (int j = 0; j < sub_msg.length(); j++) {
						if (sub_msg[j] != '&')
							every_participator += sub_msg[j];
						else {
							the_participators.push_back(every_participator);
							every_participator = "";
						}
						if (j == sub_msg.length() - 1) 
							the_participators.push_back(every_participator);
					}
					create_meeting.setParticipator(the_participators);
				}
				else if (flag == 2)
					create_meeting.setStartDate(sub_msg);
				else if (flag == 3)
					create_meeting.setEndDate(sub_msg);
				else
					create_meeting.setTitle(sub_msg);
				sub_msg = "";
				flag++;
				continue;
			}
			if (is_msg)
				sub_msg += msg[i];
			if (msg[i] == '\"' && !is_msg)
				is_msg = true;
		}
		m_meetingList.push_back(create_meeting);
	}
	meeting_in.close();
	return true;
}

bool Storage::writeToFile(void) {
	ofstream user_out;
	ofstream meeting_out;
	user_out.open(Path::userPath);
	meeting_out.open(Path::meetingPath);
	if (!(user_out.is_open() && meeting_out.is_open()))
		return false;
	for (list<User>::iterator itor = m_userList.begin(); itor != m_userList.end(); ) {
		user_out << "\"" << itor->getName() << "\",\"" << itor->getPassword() << "\",\"" << itor->getEmail() << "\",\"" << itor->getPhone() << "\"";
		itor++;
		if (itor != m_userList.end())
			user_out << endl;
	}
	user_out.close();
	for (list<Meeting>::iterator itor = m_meetingList.begin(); itor != m_meetingList.end(); ) {
		meeting_out << "\"" << itor->getSponsor() << "\",\"";
		for (int i = 0; i < itor->getParticipator().size();i++) {
			meeting_out << itor->getParticipator()[i];
			if (i != itor->getParticipator().size() - 1)
				meeting_out << "&";
		}
		meeting_out << "\",\"" << Date::dateToString(itor->getStartDate()) << "\",\"" << Date::dateToString(itor->getEndDate()) << "\",\"" << itor->getTitle() << "\"";
		itor++;
		if (itor != m_meetingList.end())
			meeting_out << endl;
	}
	meeting_out.close();
	return true;
}

/*¾²Ì¬º¯Êý*/
shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == NULL)
		m_instance = shared_ptr<Storage>(new Storage());
	return m_instance;
}

Storage::~Storage() {
	sync();
}

void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
}

list<User> Storage::queryUser(function<bool(const User &)> filter) const {
	list<User> user_list;
	for_each(m_userList.begin(), m_userList.end(), [&](User temp) {
		if (filter(temp))
			user_list.push_back(temp);
	});
	return user_list;
}

int Storage::updateUser(function<bool(const User &)> filter, function<void(User &)> switcher) {
	int count = 0;
	for_each(m_userList.begin(), m_userList.end(), [&](User temp) {
		if (filter(temp)) {
			switcher(temp);
			count++;
			m_dirty = true;
		}
	});

	return count;
}

int Storage::deleteUser(function<bool(const User &)> filter) {
	int count = 0;
	list<User>::iterator itor = m_userList.begin();
	while (itor != m_userList.end()) {
		if (filter(*itor)) {
			itor = m_userList.erase(itor);
			count++;
			m_dirty = true;
			continue;
		}
		itor++;
	}
	return count;
}

void Storage::createMeeting(const Meeting &t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const {
	list<Meeting> meeting_list;
	for_each(m_meetingList.begin(), m_meetingList.end(), [&](Meeting temp) {
		if (filter(temp))
			meeting_list.push_back(temp);
	});
	return meeting_list;
}

int Storage::updateMeeting(function<bool(const Meeting &)> filter, function<void(Meeting &)> switcher) {
	int count = 0;
	for (list<Meeting>::iterator itor = m_meetingList.begin(); itor != m_meetingList.end(); itor++) {
		if (filter(*itor)) {
			switcher(*itor);
			count++;
			break;
		}
	}
	m_dirty = true;
	return count;
}

int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
	int count = 0;
	list<Meeting>::iterator itor = m_meetingList.begin();
	while (itor != m_meetingList.end()) {
		if (filter(*itor)) {
			itor = m_meetingList.erase(itor);
			count++;
			m_dirty = true;
			continue;
		}
		itor++;
	}
	return count;
}

bool Storage::sync(void) {
	if (m_dirty == true) {
		m_dirty = false;
		return writeToFile();
	}
}