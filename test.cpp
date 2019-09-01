#include <iostream>
#include<csignal>
#include "AgendaUI.hpp"
using namespace std;

void cs(int sig) {
	Storage::getInstance()->sync();
	exit(sig);
}

int main() {
	signal(SIGINT, cs);
	AgendaUI agenda;
	return 0;
}