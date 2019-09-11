/// @Author Michael Rainsford Ryan
/// @Date 11/09/2019

#include <iostream>

void start();
void backstory();

int main() {

	start();

	return 0;
}

void start() {
	std::cout
		<< "\t _____ _____ __ __ _____    _____ _____ _____ _____ _____ __    _____ \n"
		<< "\t|_   _|   __|  |  |_   _|  |     |     |   __|   __|     |  |  |   __|\n"
		<< "\t  | | |   __|-   -| | |    | | | |-   -|__   |__   |-   -|  |__|   __|\n"
		<< "\t  |_| |_____|__|__| |_|    |_|_|_|_____|_____|_____|_____|_____|_____|\n";

	std::cout << "\n\n\n\n\t\t\t";
	system("pause");
	backstory();
}

void backstory() {
	system("cls");
	std::cout << "You are the captain of the SS Bourke.\n\nOn a routine supply trip to an offshore military base, your ship was suddenly attacked, and seemingly surrounded.\n"
		<< "Your ship, while mainly a cargo ship, is equiped with a single, slow firing, mounted missile launcher. You are vastly\noutnumbered, but with quick thinking, and"
		<< "precise aim you think you can take down the attackers...\n\n";
	system("pause");
}