/// @Author Michael Rainsford Ryan
/// @Date 11/09/2019
/// Approx Time Taken: 4h

#include <iostream>
#include <time.h>
#include <Windows.h>
#include "string"

typedef struct Position
{
	int x;
	int y;

	std::string getString()
	{
		return std::to_string(x) + ", " + std::to_string(y);
	}
} Coordinates;

enum WarHead
{
	Explosive,
	Nuclear,
};

// Structs
typedef struct Enemy
{
	Coordinates coordinates;
	bool active;
} Target;

struct Missile
{
	WarHead payload;

	Coordinates coordinates;

	Target target;

	bool armed;

	void arm()
	{
		armed = !armed;
	}

	void update()
	{
		coordinates.x += 1;
		coordinates.y += 2;
	}
};

// Variables
const unsigned NUM_ENEMIES{ 8 };

Enemy enemies[NUM_ENEMIES];
int health = 10;
int enemiesAlive = NUM_ENEMIES;
int command = 10;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Function declarations
void start();
void backstory();
void shipControl();
void scan();
void chooseWarHead();
void launchMissile();
void enemyAttack();
void typewrite(std::string t_string);
void dotDelay(int t_delay);

// Function definitions
int main()
{
	srand(static_cast<unsigned>(time(nullptr)));

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		enemies[i].active = true;
		enemies[i].coordinates.x = rand() % 20;
		enemies[i].coordinates.y = rand() % 20;
	}

	start();

	return 0;
}

void start()
{
	std::cout
		<< "\t _____ _____ __ __ _____    _____ _____ _____ _____ _____ __    _____ \n"
		<< "\t|_   _|   __|  |  |_   _|  |     |     |   __|   __|     |  |  |   __|\n"
		<< "\t  | | |   __|-   -| | |    | | | |-   -|__   |__   |-   -|  |__|   __|\n"
		<< "\t  |_| |_____|__|__| |_|    |_|_|_|_____|_____|_____|_____|_____|_____|\n";

	std::cout << "\n\n\n\n\t\t\t";
	system("pause");
	backstory();
}

void backstory()
{
	system("cls");
	typewrite("You are the captain of the SS Bourke.\n\nOn a routine supply trip to an offshore military base, your ship was suddenly attacked, and seemingly surrounded.\n");
	typewrite("Your ship, while mainly a cargo ship, is equiped with a single, slow firing, mounted missile launcher. You are vastly\noutnumbered, but with quick thinking, and ");
	typewrite("precise aim you think you can take down the attackers");
	dotDelay(400);
	std::cout << "\n\n";
	system("pause");

	shipControl();
}

void shipControl()
{
	while (command != 0)
	{
		system("cls");
		typewrite("Enter a number coresponding to a command:\n= 0 = exit program\n= 1 = Scan for enemy ships\n= 2 = Launch Missile\n= 3 = Choose Missile Type\n");
		typewrite("-----------------------------------------------\n");

		std::cin >> command;

		switch (command)
		{
		case 1: // Scan for enemies
			scan();
			break;
		case 2:
			launchMissile();
			break;
		case 3:
			chooseWarHead();
			break;
		default:
			break;
		}

		if (command > 0 && command < 3)
		{
			enemyAttack();
		}

		if (health <= 0)
		{
			typewrite("\nYour ship is going down");
			dotDelay(400);
			typewrite("\nThere is no escape");
			dotDelay(400);
			std::cout << "\n";
			command = 0;
			system("pause");
		}
	}

	system("cls");
	typewrite("Shutting Down");
	dotDelay(600);
	Sleep(200);
}

void scan()
{
	system("cls");
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemies[i].active)
		{
			typewrite("Enemy scanned at " + enemies[i].coordinates.getString() + "\n");
		}
	}
	system("pause");
}

void chooseWarHead()
{
	system("cls");

	typewrite("Enter a number coresponding to a war head:\n= 0 = Exit war head selection\n= 1 = Explosive (Unlimited)\n= 2 = Nuclear (4 left)\n");
	typewrite("-----------------------------------------------\n");

	int missileType;

	std::cin >> missileType;

	switch (missileType)
	{
	case 0:
		break;
	case 1: // Scan for enemies
		typewrite("Explosive missile selected\n");
		dotDelay(400);
		break;
	case 2:
		typewrite("Nuclear missile selected\n");
		dotDelay(400);
		break;
	default:
		typewrite("Unrecognised value entered, default missile selected: Explosive\n");
		dotDelay(400);
		break;
	}
}

void launchMissile()
{
	// Set up variables
	bool targetHit = false;
	Coordinates targetCoords;

	// Get input
	typewrite("Enter coordinates (x and y separated by a space): ");
	std::cin >> targetCoords.x >> targetCoords.y;
	
	// Check coordinates are within range
	if (targetCoords.x < 20 && targetCoords.y < 20 && targetCoords.x >= 0 && targetCoords.y >= 0)
	{
		// Loop the enemies
		for (int i = 0; i < NUM_ENEMIES; i++)
		{
			// Check if the coordinates match and the enemy is active
			if (enemies[i].coordinates.x == targetCoords.x && enemies[i].coordinates.y == targetCoords.y && enemies[i].active)
			{
				typewrite("Target hit at " + enemies[i].coordinates.getString() + "\n");
				enemies[i].active = false;
				targetHit = true;
				enemiesAlive--;
				break;
			}
		}

		// Check if target was hit, display message if not
		if (!targetHit)
		{
			typewrite("MISS: No target spotted at " + std::to_string(targetCoords.x) + ", " + std::to_string(targetCoords.y) + "\n");
		}
	}
	else // Display message if coordinates out of range
	{
		typewrite("ERROR: Coordinates out of range.\n");
	}

	system("pause");
}

void enemyAttack()
{
	typewrite("\nScanning for enemy action");
	dotDelay(400);
	std::cout << "\n";

	if (enemiesAlive <= 0)
	{
		typewrite("No enemies alive\n");
		SetConsoleTextAttribute(hConsole, 6);
		typewrite("\nWe made it out alive! Good job, Captain!\n\n");
		SetConsoleTextAttribute(hConsole, 15);
		command = 0;
	}
	else
	{
		for (int i = 0; i < NUM_ENEMIES; i++)
		{
			if (enemies[i].active)
			{
				typewrite("\nMissile Fired!\n");

				if (rand() % NUM_ENEMIES == 0)
				{
					health--;
					if (health < 0)
					{
						health = 0;
					}
					SetConsoleTextAttribute(hConsole, 4);
					typewrite("Ship hit!\n");
					SetConsoleTextAttribute(hConsole, 15);
					typewrite("Ship Health: " + std::to_string(health) + "\n");
				}
				else
				{
					typewrite("It missed!\n");
				}
			}
		}
	}

	system("pause");
}

void typewrite(std::string t_string)
{
	int len = t_string.length();
	for (int i = 0; i < len; i++)
	{
		std::cout << t_string[i];
		Sleep(2);
	}
}

void dotDelay(int t_delay)
{
	for (int i = 0; i < 3; i++)
	{
		Sleep(t_delay);
		std::cout << ".";
	}
	Sleep(t_delay);
}