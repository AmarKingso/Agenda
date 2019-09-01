#include"AgendaUI.hpp"
#include"ErrorReason.hpp"
#include<stdio.h>
using namespace std;

string cur_username;
string cur_password;

AgendaUI::AgendaUI() {
	startAgenda();
	OperationLoop();
}

void AgendaUI::OperationLoop(void) {
	string op = getOperation();
	cout << endl;
	while (executeOperation(op)) {
		op = getOperation();
		cout << endl;
	}
}

void AgendaUI::startAgenda(void) {
	cout << "----------------------- Agenda -------------------------------" << endl;
	cout << "Action : " << endl;
	cout << "l   - log in Agenda by user name and password" << endl;
	cout << "r   - register an Agenda account" << endl;
	cout << "q   - quit Agenda" << endl;
	cout << "--------------------------------------------------------------" << endl << endl;
	cout << "Agenda : ~$ ";
}

string AgendaUI::getOperation() {
	string op;
	cin >> op;
	return op;
}


bool AgendaUI::executeOperation(string t_operation) {
	if (t_operation == "l") {
		userLogIn();
		return true;
	}
	else if (t_operation == "r") {
		userRegister();
		return true;
	}
	else if (t_operation == "q") {
		quitAgenda();
		return false;
	}
	else if (t_operation == "o") {
		userLogOut();
		return true;
	}
	else if (t_operation == "dc") {
		deleteUser();
		return true;
	}
	else if (t_operation == "lu") {
		listAllUsers();
		return true;
	}
	else if (t_operation == "cm") {
		createMeeting();
		return true;
	}
	else if (t_operation == "amp") {
		addParticipator();
		return true;
	}
	else if (t_operation == "rmp") {
		removeParticipator();
		return true;
	}
	else if (t_operation == "rqm") {
		quitMeeting();
		return true;
	}
	else if (t_operation == "la") {
		listAllMeetings();
		return true;
	}
	else if (t_operation == "las") {
		listAllSponsorMeetings();
		return true;
	}
	else if (t_operation == "lap") {
		listAllParticipateMeetings();
		return true;
	}
	else if (t_operation == "qm") {
		queryMeetingByTitle();
		return true;
	}
	else if (t_operation == "qt") {
		queryMeetingByTimeInterval();
		return true;
	}
	else if (t_operation == "dm") {
		deleteMeetingByTitle();
		return true;
	}
	else if (t_operation == "da") {
		deleteAllMeetings();
		return true;
	}
	else {
		cout << "Don't exist instruction." << endl;
		if (cur_username == "")
			cout << endl << "Agenda : ~$ ";
		else
			cout << endl << "Agenda@" << cur_username << " :~# ";
	}
}

void AgendaUI::userLogIn(void) {
	string t_username, t_password;
	cout << "[log in] [user name] [password]" << endl;
	cout << "[log in] ";
	cin >> t_username >> t_password;
	if (m_agendaService.userLogIn(t_username, t_password)) {
		cout << "[log in] succeed!" << endl << endl;
		cout << "----------------------- Agenda -------------------------------" << endl;
		cout << "Action : " << endl;
		cout << "o    - log out Agenda" << endl;
		cout << "dc   - delete Agenda account" << endl;
		cout << "lu   - list all Agenda user" << endl;
		cout << "cm   - create a meeting" << endl;
		cout << "amp  - add meeting participator" << endl;
		cout << "rmp  - remove meeting participator" << endl;
		cout << "rqm  - request to quit meeting" << endl;
		cout << "la   - list all meetings" << endl;
		cout << "las  - list all sponsor meeting" << endl;
		cout << "lap  - list all participate meetings" << endl;
		cout << "qm   - query meeting by title" << endl;
		cout << "qt   - query meeting by time interval" << endl;
		cout << "dm   - delete meeting by title" << endl;
		cout << "da   - delete all meeting" << endl;
		cout << "--------------------------------------------------------------" << endl << endl;
		cout << "Agenda@" << t_username << " :~# ";
		cur_username = t_username;
		cur_password = t_password;
	}
	else {
		cout << "[log in] Password error or user doesn't exist" << endl;
		cout << endl << "Agenda : ~$ ";
	}
}

void AgendaUI::userRegister(void) {
	string t_username, t_password, t_email, t_phone;
	cout << "[register] [user name] [password] [email] [phone]" << endl;
	cout << "[register] ";
	cin >> t_username >> t_password >> t_email >> t_phone;
	if (m_agendaService.userRegister(t_username, t_password, t_email, t_phone)) {
		cout << "[register] succeed!" << endl;
	}
	else
		cout << "[register] This username has been registered!" << endl;
	cout<<endl<< "Agenda : ~$ ";
}

void AgendaUI::userLogOut(void) {
	cout << endl;
	cur_username = "";
	cur_password = "";
	startAgenda();
}

void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(void) {
	if (m_agendaService.deleteUser(cur_username, cur_password)) {
		cout << "[delete agenda account] succeed!" << endl;
		cout << endl;
		cur_username = "";
		cur_password = "";
		startAgenda();
	}
 }

void AgendaUI::listAllUsers(void) {
	cout << "[list all user]" << endl << endl;
	cout << "name    email           phone" << endl;
	list<User> temp = m_agendaService.listAllUsers();
	for (list<User>::iterator itor = temp.begin(); itor != temp.end(); itor++) {
		printf("%-8s%-16s%-11s\n", itor->getName().c_str(), itor->getEmail().c_str(), itor->getPhone().c_str());
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
 }

void AgendaUI::createMeeting(void) {
	int par_count = 0;
	vector<string> par;
	string temp_par;
	string temp_title;
	string start_date;
	string end_date;
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	cin >> par_count;
	for (int i = 1; i <= par_count; i++) {
		cout << "[create meeting] [please enter the participator " << i << " ]" << endl;
		cout << "[create meeting] ";
		cin >> temp_par;
		par.push_back(temp_par);
	}
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[create meeting] ";
	cin >> temp_title >> start_date >> end_date;
	try {
		if (m_agendaService.createMeeting(cur_username, temp_title, start_date, end_date, par))
			cout << "[create meeting] succeed!" << endl;
	}
	catch(ErrReason e){
		cout << "[create meeting] " << e.what() << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
 }

void AgendaUI::addParticipator() {
	string t_title, t_participator;
	cout << "[add participator] [meeting title] [participator username]" << endl;
	cout << "[add participator] ";
	cin >> t_title >> t_participator;
	try {
		if (m_agendaService.addMeetingParticipator(cur_username, t_title, t_participator))
			cout << "[add participator] succeed!" << endl;
	}
	catch(ErrReason e) {
		cout << "[add participator] " << e.what() << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::removeParticipator() {
	string t_title, t_participator;
	cout << "[remove participator] [meeting title] [participator username]" << endl;
	cout << "[remove participator] ";
	cin >> t_title >> t_participator;
	try {
		if (m_agendaService.removeMeetingParticipator(cur_username, t_title, t_participator))
			cout << "[remove participator] succeed!" << endl;
	}
	catch (ErrReason e) {
		cout << "[remove participator] " << e.what() << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::quitMeeting() {
	string t_title;
	cout << "[quit meeting] [meeting title]" << endl;
	cout << "[quit meeting] ";
	cin >> t_title;
	try {
		if (m_agendaService.quitMeeting(cur_username, t_title))
			cout << "[quit meeting] succeed!" << endl;
	}
	catch (ErrReason e) {
		cout << "[quit meeting] " << e.what() << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::listAllMeetings(void) {
	cout << "[list all meetings]" << endl << endl;
	cout << "title          sponsor        start time        end time          participators" << endl;
	list<Meeting> temp = m_agendaService.listAllMeetings(cur_username);
	for (list<Meeting>::iterator itor = temp.begin(); itor != temp.end(); itor++) {
		printf("%-15s%-15s%-18s%-18s", itor->getTitle().c_str(), itor->getSponsor().c_str(), Date::dateToString(itor->getStartDate()).c_str(), Date::dateToString(itor->getEndDate()).c_str());
		vector<string> participator = itor->getParticipator();
		for (int i = 0; i < participator.size(); i++) {
			cout << participator[i];
			if (i != participator.size() - 1)
				cout << ",";
		}
		cout << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::listAllSponsorMeetings(void) {
	cout << "[list all sponsor meetings]" << endl << endl;
	cout << "title          sponsor        start time        end time          participators" << endl;
	list<Meeting> temp = m_agendaService.listAllSponsorMeetings(cur_username);
	for (list<Meeting>::iterator itor = temp.begin(); itor != temp.end(); itor++) {
		printf("%-15s%-15s%-18s%-18s", itor->getTitle().c_str(), itor->getSponsor().c_str(), Date::dateToString(itor->getStartDate()).c_str(), Date::dateToString(itor->getEndDate()).c_str());
		vector<string> participator = itor->getParticipator();
		for (int i = 0; i < participator.size(); i++) {
			cout << participator[i];
			if (i != participator.size() - 1)
				cout << ",";
		}
		cout << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::listAllParticipateMeetings(void) {
	cout << "[list all participator meetings]" << endl << endl;
	cout << "title          sponsor        start time        end time          participators" << endl;
	list<Meeting> temp = m_agendaService.listAllParticipateMeetings(cur_username);
	for (list<Meeting>::iterator itor = temp.begin(); itor != temp.end(); itor++) {
		printf("%-15s%-15s%-18s%-18s", itor->getTitle().c_str(), itor->getSponsor().c_str(), Date::dateToString(itor->getStartDate()).c_str(), Date::dateToString(itor->getEndDate()).c_str());
		vector<string> participator = itor->getParticipator();
		for (int i = 0; i < participator.size(); i++) {
			cout << participator[i];
			if (i != participator.size() - 1)
				cout << ",";
		}
		cout << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::queryMeetingByTitle(void) {
	string t_title;
	cout << "[query meeting] [title]" << endl;
	cout << "[query meeting] ";
	cin >> t_title;
	cout << "title          sponsor        start time        end time          participators" << endl;
	list<Meeting> temp = m_agendaService.meetingQuery(cur_username,t_title);
	if (!temp.empty()) {
		list<Meeting>::iterator itor = temp.begin();
		printf("%-15s%-15s%-18s%-18s", itor->getTitle().c_str(), itor->getSponsor().c_str(), Date::dateToString(itor->getStartDate()).c_str(), Date::dateToString(itor->getEndDate()).c_str());
		vector<string> participator = itor->getParticipator();
		for (int i = 0; i < participator.size(); i++) {
			cout << participator[i];
			if (i != participator.size() - 1)
				cout << ",";
		}
		cout << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::queryMeetingByTimeInterval(void) {
	string start_date, end_date;
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meetings] ";
	cin >> start_date >> end_date;
	cout << "[query meetings]" << endl << endl;
	cout << "title          sponsor        start time        end time          participators" << endl;
	list<Meeting> temp = m_agendaService.meetingQuery(cur_username, start_date, end_date);
	for (list<Meeting>::iterator itor = temp.begin(); itor != temp.end(); itor++) {
		printf("%-15s%-15s%-18s%-18s", itor->getTitle().c_str(), itor->getSponsor().c_str(), Date::dateToString(itor->getStartDate()).c_str(), Date::dateToString(itor->getEndDate()).c_str());
		vector<string> participator = itor->getParticipator();
		for (int i = 0; i < participator.size(); i++) {
			cout << participator[i];
			if (i != participator.size() - 1)
				cout << ",";
		}
		cout << endl;
	}
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::deleteMeetingByTitle(void) {
	string t_title;
	cout << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	cin >> t_title;
	if (m_agendaService.deleteMeeting(cur_username, t_title))
		cout << "[delete meeting] succeed!" << endl;
	else
		cout << "[error] delete meeting fail!" << endl;
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::deleteAllMeetings(void) {
	if (m_agendaService.deleteAllMeetings(cur_username))
		cout << "[delete all meeting] succeed!" << endl;
	cout << endl << "Agenda@" << cur_username << " :~# ";
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {

}