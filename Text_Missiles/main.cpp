/// @Author Michael Rainsford Ryan
/// @Date 11/09/2019
/// Approx Time Taken: 15h

#include <iostream>
#include <time.h>
#include <Windows.h>
#include "string"

/// ********************************************************************** 
typedef struct Position
{
	int x;
	int y;

	inline std::string getString() { return std::to_string(x) + ", " + std::to_string(y); }

	inline bool operator == (Position t_pos) { return (this->x == t_pos.x && this->y == t_pos.y); }
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
		armed = true;
	}

	void disarm()
	{
		armed = false;
	}

	// Moves the missile closer to the target each frame
	void update()
	{
		// Get the line between the missile and the target
		float lineX = static_cast<float>(target.coordinates.x - coordinates.x);
		float lineY = static_cast<float>(target.coordinates.y - coordinates.y);

		// Get the length of the line (distance from missile to target)
		float distance = sqrtf((lineX * lineX) + (lineY * lineY));

		// Get the unit vector terms
		float unitX = lineX / distance;
		float unitY = lineY / distance;

		// Move by the unit vector (static cast to deal with integer values)
		coordinates.x = static_cast<int>(round(static_cast<float>(coordinates.x) + unitX));
		coordinates.y = static_cast<int>(round(static_cast<float>(coordinates.y) + unitY));
	}
};

struct Player {
	Coordinates coordinates;
	int health; // Player health points
	int command; // Command number entered
};

/// ********************************************************************** 
/// Variables
const unsigned NUM_ENEMIES{ 8 };
const int MAP_SIZE{ 20 };
const int LAUNCH_CODE{ 48602 };

int numNukes; // How many nuclear missiles are left
int enemiesAlive = NUM_ENEMIES; // Current number of alive enemies
Enemy enemies[NUM_ENEMIES]; // Enemy objects
Player player;
Missile missile;

// For changing font colour 
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

/// ********************************************************************** 
// Function declarations
void start();
void backstory();
void shipControl();
void scan();
void chooseWarHead();
void launchMissile();
void updateLaunchedMissile();
void checkBlastRadius();
void enemyAttack();
void typewrite(std::string t_string);
void dotDelay(int t_delay);
void drawMap();

/// ********************************************************************** 
// Function definitions

/// <summary>
/// Main entry point. calls to the game loop after an intro
/// </summary>
/// <returns></returns>
int main()
{
	// Sets the random seed
	srand(static_cast<unsigned>(time(nullptr)));

	// Sets the player coordinates
	player.coordinates.x = 10;
	player.coordinates.y = 10;

	player.command = 10; // Sets the command to a base value
	player.health = NUM_ENEMIES; // Sets the player health to a base value

	missile.disarm(); // Discarm the missile by default

	numNukes = 4;

	// Sets up all the enemies
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		enemies[i].active = true; // Sets the enemy to active

		// Sets the enemy's coordinates
		enemies[i].coordinates.x = rand() % 20;
		enemies[i].coordinates.y = rand() % 20;

		// Checks the enemy is not colliding with the player
		if (enemies[i].coordinates.x == player.coordinates.x
			&& enemies[i].coordinates.y == player.coordinates.y)
		{
			// Moves the enemy if it is colliding
			enemies[i].coordinates.x++;
			enemies[i].coordinates.y++;
		}
	}

	// Starts the game
	start();

	return 0;
}

/// <summary>
/// Displays the title until the player presses a button
/// </summary>
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

/// <summary>
/// Displays the game backstory until the player presses a button
/// </summary>
void backstory()
{
	system("cls");
	SetConsoleTextAttribute(hConsole, 14);
	typewrite("You are the captain of the SS Bourke.\n\nOn a routine supply trip to an offshore military base, your ship was suddenly attacked, and seemingly surrounded.\n");
	typewrite("Your ship, while mainly a cargo ship, is equiped with a single, slow firing, mounted missile launcher. You are vastly\noutnumbered, but with quick thinking, and ");
	typewrite("precise aim you think you can take down the attackers");
	dotDelay(400);
	std::cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 15);
	system("pause");

	// Goes to the game's main game loop (shipControl)
	shipControl();
}

/// <summary>
/// Gives the player a number of options to choose from to control the ship:
/// <para>0. quits the game.</para>
/// <para>1. scans for enemies.</para>
/// <para>2. fires a missile.</para>
/// <para>3. Lets you select a warhead.</para>
/// <para>:</para>
/// <para>Runs the enemy attack script</para>
/// <para>If the player's health goes to zero, the game ends</para>
/// </summary>
void shipControl()
{
	while (player.command != 0)
	{
		system("cls");
		typewrite("Enter a number coresponding to a command:\n= 0 = exit program\n= 1 = Scan for enemy ships\n= 2 = Launch Missile\n= 3 = Choose Missile Type\n= 4 = Display Launch Code\n");
		typewrite("-----------------------------------------------\n");

		std::cin >> player.command;

		switch (player.command)
		{
		case 1: // Scan for enemies
			scan();
			break;
		case 2: // Launch a missile
			launchMissile();
			break;
		case 3: // Choose a missile type (warhead)
			chooseWarHead();
			break;
		case 4: // Output launch code
			typewrite("Launch code: " + std::to_string(LAUNCH_CODE) + "\n");
			system("pause");
		}

		// Lets the enemy attack if the player performed an action
		if (player.command > 0 && player.command < 3)
		{
			enemyAttack();
		}

		// If the player's health reaches zero, displays the death dialog and ends the game
		if (player.health <= 0)
		{
			SetConsoleTextAttribute(hConsole, 14);
			typewrite("\nYour ship is going down");
			dotDelay(400);
			typewrite("\nThere is no escape");
			dotDelay(400);
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "\n";
			player.command = 0;
			system("pause");
		}
	}

	// End game text
	system("cls"); // Clear the screen
	typewrite("Shutting Down"); // Type dialog
	dotDelay(600); // Delay with dots printing
	Sleep(200); // Short delay
}

/// <summary>
/// Draws enemies on screen and prints out coordinates
/// </summary>
void scan()
{
	system("cls"); // Clear the screen

	drawMap(); // Draw all the enemies

	typewrite("Enemies scanned at...\n"); // Type dialog

	int enemiesDisplayed = 0; // Set a variable for display formatting

	for (int i = 0; i < NUM_ENEMIES; i++) // Loop all enemies
	{
		if (enemies[i].active) // If the enemy is still alive
		{
			typewrite("{ " + enemies[i].coordinates.getString() + " }\t"); // Type the enemy coordinates

			enemiesDisplayed++; // Add to number of enemies displayed
			if (enemiesDisplayed % 4 == 0) // If 4 coordinates have been displayed, break to a new line
			{
				std::cout << std::endl;
			}
		}
	}

	// Break to a new line and pause
	std::cout << std::endl;
	system("pause");
}

/// <summary>
/// Lets the player select the missile type to use (warhead)
/// </summary>
void chooseWarHead()
{
	// Clears the screen and types the options
	system("cls");
	typewrite("Enter a number coresponding to a warhead:\n= 0 = Exit war head selection\n= 1 = Explosive (Unlimited)\n= 2 = Nuclear (" + std::to_string(numNukes) + ")\n");
	typewrite("-----------------------------------------------\n");

	int missileType; // Missile type input

	std::cin >> missileType; // Take input

	// Switches on input to set missile type
	switch (missileType)
	{
	case 0: // Exits missile select
		break;
	case 1: // Selects explosive type missile
		typewrite("Explosive missile selected\n"); // Types selection
		missile.payload = WarHead::Explosive; // Sets the missile type
		dotDelay(400); // Delay with dot display
		break;
	case 2: // Selects nuclear type missile
		if (numNukes > 0) // Check there are nukes available
		{
			typewrite("Nuclear missile selected\n");  // Types selection
			missile.payload = WarHead::Nuclear; // Sets the missile type
			dotDelay(400); // Delay with dot display
		}
		else // Set the default missile if no nukes left
		{
			typewrite("No Nuclear missiles left, default missile selected: Explosive\n");  // Types selection
			missile.payload = WarHead::Explosive; // Sets the missile type
			dotDelay(400); // Delay with dot display
		}
		break;
	default: // Any previously undefined value 
		typewrite("Unrecognised value entered, default missile selected: Explosive\n"); // Types that the input is an unknown value
		missile.payload = WarHead::Explosive; // Selects the explosive type missile
		dotDelay(400); // Delay with dot display
		break;
	}
}

void launchMissile()
{
	// Set up variables
	bool targetFound = false;
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
			if (enemies[i].active && enemies[i].coordinates == targetCoords)
			{
				targetFound = true;

				int launchCode;
				typewrite("Enter launch code: ");
				std::cin >> launchCode;

				if (LAUNCH_CODE == launchCode)
				{
					missile.target = enemies[i];
					missile.coordinates = player.coordinates;
					missile.arm();
					typewrite("Target at " + enemies[i].coordinates.getString() + " locked. Firing missile\n");
					dotDelay(400);

					updateLaunchedMissile();
				}
				else
				{
					typewrite("Code incorrect. Aborting missile launch");
					dotDelay(200);
					std::cout << std::endl;
				}
				
				break;
			}
		}

		// Display a message if no target was found
		if (!targetFound)
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

void updateLaunchedMissile()
{
	while (missile.armed)
	{
		system("cls");
		missile.update();
		drawMap();
		Sleep(200);

		for (int i = 0; i < NUM_ENEMIES; i++)
		{
			if (missile.coordinates == enemies[i].coordinates
				&& enemies[i].active)
			{
				enemies[i].active = false;
				enemiesAlive--;
				typewrite("Target hit " + enemies[i].coordinates.getString() + "\n");

				if (missile.payload == WarHead::Nuclear)
				{
					checkBlastRadius();
					numNukes--;
					if (numNukes == 0)
					{
						missile.payload = WarHead::Explosive;
						typewrite("No more nuclear missiles available. Switched to Explosive missiles.\n");
					}
				}

				missile.disarm();
				break;
			}
		}

		// If the missile leaves the map boundaries
		if (missile.coordinates.x < 0 || missile.coordinates.x >= MAP_SIZE
			|| missile.coordinates.y < 0 || missile.coordinates.y >= MAP_SIZE)
		{
			missile.disarm();
		}
	}
}

void checkBlastRadius()
{
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		// Check if there's an enemy to the left of the missile hit
		if (enemies[i].active
			&& enemies[i].coordinates.x == missile.coordinates.x - 1 && enemies[i].coordinates.y == missile.coordinates.y)
		{
			enemies[i].active = false;
			enemiesAlive--;
			typewrite("Target hit in the blast radius at " + enemies[i].coordinates.getString() + "\n");
		}
		// Check if there's an enemy to the right of the missile hit
		if (enemies[i].active
			&& enemies[i].coordinates.x == missile.coordinates.x + 1 && enemies[i].coordinates.y == missile.coordinates.y)
		{
			enemies[i].active = false;
			enemiesAlive--;
			typewrite("Target hit in the blast radius at " + enemies[i].coordinates.getString() + "\n");
		}
		// Check if there's an enemy to the left of the missile hit
		if (enemies[i].active
			&& enemies[i].coordinates.x == missile.coordinates.x && enemies[i].coordinates.y == missile.coordinates.y - 1)
		{
			enemies[i].active = false;
			enemiesAlive--;
			typewrite("Target hit in the blast radius at " + enemies[i].coordinates.getString() + "\n");
		}
		// Check if there's an enemy to the right of the missile hit
		if (enemies[i].active
			&& enemies[i].coordinates.x == missile.coordinates.x && enemies[i].coordinates.y == missile.coordinates.y + 1)
		{
			enemies[i].active = false;
			enemiesAlive--;
			typewrite("Target hit in the blast radius at " + enemies[i].coordinates.getString() + "\n");
		}
	}
}

void enemyAttack()
{
	typewrite("\nScanning for enemy action");
	dotDelay(400);
	std::cout << "\n";

	if (enemiesAlive <= 0)
	{
		typewrite("No enemies alive\n");
		SetConsoleTextAttribute(hConsole, 14);
		typewrite("\nWe made it out alive! Good job, Captain!\n\n");
		SetConsoleTextAttribute(hConsole, 15);
		player.command = 0;
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
					player.health--;
					if (player.health < 0)
					{
						player.health = 0;
					}
					SetConsoleTextAttribute(hConsole, 4);
					typewrite("Ship hit!\n");
					SetConsoleTextAttribute(hConsole, 15);
					typewrite("Ship Health: " + std::to_string(player.health) + "\n");
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

void drawMap()
{
	bool drawnEnemy = false;
	Coordinates cellCoords;

	for (int row = 0; row < MAP_SIZE; row++)
	{
		for (int col = 0; col < MAP_SIZE; col++)
		{
			cellCoords.x = col;
			cellCoords.y = row;

			if (missile.coordinates == cellCoords && missile.armed)
			{
				SetConsoleTextAttribute(hConsole, 10);
				std::cout << "<->";
			}
			else
			{
				for (int enemyIndex = 0; enemyIndex < NUM_ENEMIES; enemyIndex++)
				{
					if (enemies[enemyIndex].active && enemies[enemyIndex].coordinates == cellCoords)
					{

						SetConsoleTextAttribute(hConsole, 12);
						std::cout << "=+=";
						drawnEnemy = true;
						break;
					}
				}

				if (!drawnEnemy)
				{
					if (player.coordinates == cellCoords)
					{
						SetConsoleTextAttribute(hConsole, 10);
						std::cout << "=+=";
					}
					else
					{
						SetConsoleTextAttribute(hConsole, 9);
						std::cout << "~~~";
					}
				}
			}

			drawnEnemy = false;
		}

		std::cout << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 15);
}
