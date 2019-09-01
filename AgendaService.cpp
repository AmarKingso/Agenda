#include"AgendaService.hpp"
#include"ErrorReason.hpp"
using namespace std;

AgendaService::AgendaService() {
	m_storage = Storage::getInstance();
}

AgendaService::~AgendaService() {}

bool AgendaService::userLogIn(const string &userName, const string &password) {
	list<User> cur_user_list = m_storage->queryUser([&userName, &password](const User &temp)->bool {
		if (userName == temp.getName() && password == temp.getPassword())
			return true;
		return false;
	});
	if (cur_user_list.empty())
		return false;
	return true;
}

bool AgendaService::userRegister(const string &userName, const string &password,
	const string &email, const string &phone) {
	list<User> repeat_user = m_storage->queryUser([&userName](const User &temp)->bool {
		if (temp.getName() == userName)
			return true;
		return false;
	});
	if (repeat_user.empty()) {
		m_storage->createUser(User(userName, password, email, phone));
		return true;
	}
	return false;
}

bool AgendaService::deleteUser(const string &userName, const string &password) {
	int del_count = m_storage->deleteUser([&](const User &temp)->bool {
		if (temp.getName() == userName&&temp.getPassword() == password) {
			deleteAllMeetings(userName);
			list<Meeting> par_meeting = listAllParticipateMeetings(userName);
			for (list<Meeting>::iterator itor = par_meeting.begin(); itor != par_meeting.end(); itor++) {
				removeMeetingParticipator(itor->getSponsor(), itor->getTitle(), userName);
				if (itor->getParticipator().empty())
					deleteMeeting(itor->getSponsor(), itor->getTitle());
			}
			return true;
		}
		return false;
	});
	if (del_count)
		return true;
	return false;
}

list<User> AgendaService::listAllUsers(void) const {
	return m_storage->queryUser([](const User &temp)->bool {
		return true;
	});
}

bool AgendaService::createMeeting(const string &userName, const string &title,
	const string &startDate, const string &endDate,
	const vector<string> &participator) {
	Date start_time(startDate);
	Date end_time(endDate);
	list<Meeting> isexist_title = m_storage->queryMeeting([&](const Meeting &temp)->bool {
		if (temp.getTitle() == title)
			return true;
		return false;
	});
	if (!isexist_title.empty()) {
		throw(ErrReason("The title has been used!"));
		return false;
	}
	list<User> isexist_user = m_storage->queryUser([&](const User &temp)->bool {
		if (userName == temp.getName())
			return true;
		for (int i = 0; i < participator.size(); i++) {
			if (participator[i] == temp.getName())
				return true;
		}
		return false;
	});
	if (isexist_user.size() != (participator.size() + 1)) {
		throw(ErrReason("At least one invalid participant!"));
		return false;
	}
	if (Date::isValid(start_time) && Date::isValid(end_time) && !participator.empty() && start_time < end_time) {
		list<Meeting> user_meeting = listAllMeetings(userName);//发起者参加的所有会议
		list<Meeting>::iterator itor = user_meeting.begin();
		while (itor != user_meeting.end()) {
			if (start_time < itor->getStartDate() && end_time > itor->getStartDate()) {
				throw(ErrReason("You has a time conflict!"));
				return false;
			}
			if (start_time >= itor->getStartDate() && start_time < itor->getEndDate()) {
				throw(ErrReason("You has a time conflict!"));
				return false;
			}
			itor++;
		}
		for (int i = 0; i < participator.size(); i++) {
			list<Meeting> participator_meeting = listAllMeetings(participator[i]);//每个参与者参加的所有会议
			list<Meeting>::iterator itor = participator_meeting.begin();
			while (itor != participator_meeting.end()) {
				if (start_time < itor->getStartDate() && end_time > itor->getStartDate()) {
					throw(ErrReason("At least one participator has a time conflict!"));
					return false;
				}
				if (start_time >= itor->getStartDate() && start_time < itor->getEndDate()) {
					throw(ErrReason("At least one participator has a time conflict!"));
					return false;
				}
				itor++;
			}
		}
		m_storage->createMeeting(Meeting(userName, participator, startDate, endDate, title));
		return true;
	}
	throw(ErrReason("Invalid date or no participator!"));
	return false;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
	const std::string &title,
	const std::string &participator){
	list<User> cur_user_list = m_storage->queryUser([&participator](const User &temp)->bool {
		if (temp.getName() == participator)
			return true;
		return false;
	});
	if (cur_user_list.empty()) {
		throw(ErrReason("The user doesn't exist!"));
		return false;
	}
	int add_count = m_storage->updateMeeting([&](const Meeting &temp)->bool {
		if (userName == temp.getSponsor()&&title == temp.getTitle()) {
			list<Meeting> user_meeting = meetingQuery(userName, title);
			Date start_time = user_meeting.begin()->getStartDate();
			Date end_time = user_meeting.begin()->getEndDate();
			list<Meeting> participator_meeting = listAllMeetings(participator);
			list<Meeting>::iterator itor = participator_meeting.begin();
			while (itor != participator_meeting.end()) {
				if (start_time<itor->getStartDate() && end_time>itor->getStartDate()) {
					throw(ErrReason("At least one participator has a time conflict!"));
					return false;
				}
				if (start_time >= itor->getStartDate() && start_time < itor->getEndDate()) {
					throw(ErrReason("At least one participator has a time conflict!"));
					return false;
				}
				itor++;
			}
			return true;
		}
		return false;
	}, [&](Meeting &temp) {
		temp.addParticipator(participator);
	});
	if (add_count)
		return true;
	throw(ErrReason("The title doesn't exist or you aren't the meeting's sponsor!"));
	return false;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
	const std::string &title,
	const std::string &participator) {
	int del_count = m_storage->updateMeeting([&](const Meeting &temp)->bool {
		if (userName == temp.getSponsor() && title == temp.getTitle()) {
			if (temp.isParticipator(participator))
				return true;
			else {
				throw(ErrReason("There was no one user at the meeting!"));
				return false;
			}
		}
		return false;
	}, [&](Meeting &temp) {
		temp.removeParticipator(participator);
		if (temp.getParticipator().empty()) {
			deleteMeeting(userName, title);
		}
	});
	if (del_count)
		return true;
	throw(ErrReason("The title doesn't exist or you aren't the meeting's sponsor!"));
	return false;
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
	int del_count = m_storage->updateMeeting([&](const Meeting &temp)->bool {
		if (title == temp.getTitle()) {
			if (temp.isParticipator(userName))
				return true;
			else
				throw(ErrReason("You aren't the meeting's participator!"));
		}
		return false;
	}, [&](Meeting &temp) {
		temp.removeParticipator(userName);
		if (temp.getParticipator().empty()) {
			deleteMeeting(temp.getSponsor(), title);
		}
	});
	if (del_count)
		return true;
	throw(ErrReason("The meeting doesn't exist!"));
	return false;
}

list<Meeting> AgendaService::meetingQuery(const std::string &userName,
	const std::string &title) const {
	list<Meeting> exist_meeting = m_storage->queryMeeting([&](const Meeting &temp)->bool {
		if (title == temp.getTitle())
			return true;
		return false;
	});
	if (exist_meeting.empty())
		return exist_meeting;
	if (exist_meeting.begin()->getSponsor() == userName || exist_meeting.begin()->isParticipator(userName))
		return exist_meeting;
	exist_meeting.pop_back();
	return exist_meeting;
}

list<Meeting> AgendaService::meetingQuery(const std::string &userName,
	const std::string &startDate,
	const std::string &endDate) const {
	Date start_date(startDate);
	Date end_date(endDate);
	list<Meeting> exist_meeting;
	if (!Date::isValid(start_date) || !Date::isValid(end_date) || start_date > end_date)
		return exist_meeting;
	exist_meeting = m_storage->queryMeeting([&](const Meeting &temp)->bool {
		if ((userName == temp.getSponsor() || temp.isParticipator(userName)) && (temp.getStartDate() <= end_date&&temp.getEndDate() >= start_date))
			return true;
		return false;
	});
	return exist_meeting;
}

list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
	list<Meeting> all_meetings = m_storage->queryMeeting([&](const Meeting &temp)->bool {
		if (userName == temp.getSponsor() || temp.isParticipator(userName))
			return true;
		return false;
	});
	return all_meetings;
}

list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
	list<Meeting> sponsor_meetings = m_storage->queryMeeting([&](const Meeting &temp)->bool {
		if (userName == temp.getSponsor())
			return true;
		return false;
	});
	return sponsor_meetings;
}

list<Meeting> AgendaService::listAllParticipateMeetings(
	const std::string &userName) const {
	list<Meeting> participate_meetings = m_storage->queryMeeting([&](const Meeting &temp)->bool {
		if (temp.isParticipator(userName))
			return true;
		return false;
	});
	return participate_meetings;
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
	int isdelete = m_storage->deleteMeeting([&](const Meeting &temp)->bool {
		if (userName == temp.getSponsor() && title == temp.getTitle())
			return true;
		return false;
	});
	if (isdelete)
		return true;
	return false;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
	int isdelete = m_storage->deleteMeeting([&](const Meeting &temp)->bool {
		if (userName == temp.getSponsor())
			return true;
		return false;
	});
	if (isdelete)
		return true;
	return false;
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage->sync();
}