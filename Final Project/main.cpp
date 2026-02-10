#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

// ------------------------------------------------
// PLAYER STRUCTURE
// ------------------------------------------------
struct Player {
	string name;
	int hp;
	int atkPwr;
	string inventory[3]; // Simple inventory with 3 slots
	int inventorySize = 0; // Inventory starts empty

	Player(const string& name, int hp, int atkPwr) : name(name), hp(hp), atkPwr(atkPwr) {}

	void displayStats() const {
		cout << "You currently have " << hp << " HP and " <<atkPwr << " attack power." << endl;
	}

	void addItem(const string& item) {
		// Check the current inventory size before adding an item
		if (inventorySize < 3) {
			inventory[inventorySize] = item;
			inventorySize++;
			cout << "You have added " << item << " to your inventory." << endl;
		}
		else {
			cout << "Your inventory is full! You cannot add " << item << "." << endl;
		}

	}

	void displayInventory() const {

		// Check to see if there are any items in the inventory before displaying
		if (inventorySize == 0) {
			cout << "Your inventory is empty!" << endl;
		}

		// Otherwise we display the player's inventory 
		cout << "Your Inventory:" << endl;

		// Loop through the Player's inventory and display it
		for (int i = 0; i < inventorySize; i++) {
			cout << (i + 1) << ": " << inventory[i] << endl;
		}

		// Display final option for closing the inventory 
		cout << "0: Close Inventory";
	}

	// Method to use items from the player's inventory 
	void useItem() {

		// Check to see if there are any items in the inventory before displaying
		if (inventorySize == 0) {
			cout << "Your inventory is empty!" << endl;
		}

		// Call the method to display the player's inventory
		displayInventory();

		// Get the player's choice
		int choice; 
		cin >> choice;

		// If the user picks 0 then they close their inventory
		if (choice == 0) {
			cout << "You close your inventory." << endl;
			return;
		}

		// Then we check to see if the player's choice is valid and if so we use the item
		if (choice < 1 || choice > inventorySize) {
			cout << "Invalid choice! Please select a valid item number." << endl;
			return;
		}

		// I create a variable to hold the string value of the item that the player has chosen to use
		string selectedItem = inventory[choice - 1];

		// Then we check to see what item the player has chosen and apply its effects
		if (selectedItem == "Health Potion") {
			hp += 50; // Heal the player by 50 HP
			// I don't want the player's HP to exceed 100 so I check to see if it does and if so I set it back to 100
			if (hp > 100) {
				hp = 100;
			};
			// Display the player's new stat total
			displayStats()
		}
		else if (selectedItem == "Strength Elixir") {
			// If the selected item is a strength elixr then we add 20 points to their atk
			atkPwr += 20;
			// Display the player's new stat total
		}

		// Finally we remove the used item from the player's inventory 
		// We set i to the index of the item that the player has chosen to use
		// We loop through the inventory as long as i is less than the inventory size -1. This is because we want to shift the items and the new last item in the inventory will be empty after the shift so we don't want to include it in the loop
		for (int i = choice - 1; i < inventorySize - 1; i++) {
			inventory[i] = inventory[i + 1]; // We grab the item that was previously in the next slot and move it to the current slot
		}
		inventorySize--; // Decrease the inventory size by 1
	}

};

// ------------------------------------------------
// MONSTER STRUCTURE
// ------------------------------------------------
struct Monster {
	string name;
	int hp;
	int atkPwr;
	Monster(const string& name, int hp, int atkPwr) : name(name), hp(hp), atkPwr(atkPwr) {}
};

// -----------------------------------------------
// FUNCTION PROTOTYPES
// -----------------------------------------------
void combat(Player& player, Monster& monster);
int rollDmg();

// ------------------------------------------------
// PLAYER COMBAT ENUM
// ------------------------------------------------
enum PlayerAction {
	ATTACK = 1,
	BLOCK,
	USE_ITEM,
	EXIT
};




int main() {
	srand(time(nullptr));

	cout << "Welcome to this simple DnD like game!" << endl;
	cout << "Please enter the name of your character: ";
	string playerName;
	cin >> playerName;

	// Create a player with the name entered by the user and default stats
	Player player(playerName, 100, 20);
	// Monsters for the player to fight
	Monster goblin("Goblin", 50, 10);
	Monster orc("Orc", 80, 15);
	Monster necromancer("Necromancer", 150, 25);

	// intro dialogue displaying the player's name and stats
	cout << "Welcome, " << player.name << "! You are a brave adventurer embarking on a quest." << endl;
	player.displayStats();

	// Dialogue for entering the crypt
	cout << "You find yourself standing in front of a dark and ominous crypt. Do you wish to enter? (yes/no) ";
	string choice;
	cin >> choice;

	// If the player chooses to enter the crypt then we start the game
	if (choice == "yes") {
		cout << "You step into the crypt and the door slams shut behind you. You are now trapped inside!" << endl;
	}
	else {
		cout << "You decide to stay outside and miss out on the adventure that awaits inside the crypt." << endl;
		return 0; // End the game if the player chooses not to enter the crypt
	}

	// Dungeon gameplay loop
	// First we start by setting the current room the player is in to 1
	int currentRoom = 1;
	// We also create a boolean variable to track whether the game is over or not.
	bool gameOver = false;

	// We start the game loop and it continues as long as the game is not over
	while (!gameOver) {
		// We use a switch statement to determine what happens in each room based on the current room number
		switch (currentRoom) {
		case 1:
			// Here we initiate the first room of the dungeon and give the player a choice to either explore the chamber or head down the hallway
			cout << "You enter a large dark and musty chamber. In front of you looms a long and narrow hallway. Do you choose to head down the hallway or explore the chamber first?" << endl;
			cout << "1: Move down the hallway\n2: Explore the chamber" << endl;
			// We get the player's choice for what they want to do in the first room
			int roomChoice;
			cin >> roomChoice;
			// If the player chooses to head down the hallway then we move them to the next room.
			if (roomChoice == 1) {
				cout << "You cautiously make your way down the hallway..." << endl;
				currentRoom++; // Move to the next room
			} else if (roomChoice == 2) {
				// If the player choose to explore the chamber then we give them a reward but also a consequence for taking something that wasn't theirs
				cout << "You decide to explore the chamber and find a health potion hidden in a chest! But something in the shadows of the great chamber seems upset that you took something that wasn't yours. You are confronted by a Goblin!" << endl;
				// We add the health potion to the player's inventory
				player.addItem("Health Potion"); 
				// Here we would call a function to handle the combat between the player and the goblin
				// After the combat we check to see if the player is still alive 
				if (player.hp <= 0) {
					cout << "You have been defeated by the goblin! Game Over." << endl;
					gameOver = true; // End the game if the player has been defeated) 
				}
				else {
					// If the player is still alive then we move them to the next room
					cout << "You have defeated the goblin and continue on to the next room." << endl;
					currentRoom++;
				}
			}
			break;
		case 2:
			cout << "You enter the second room and are confronted by an orc!" << endl;
			// Here we would call a function to handle the combat between the player and the orc
			currentRoom++;
			break;
		case 3:
			cout << "You enter the final room and face off against a powerful necromancer!" << endl;
			// Here we would call a function to handle the combat between the player and the necromancer
			// If the player wins then they have completed the game and we set gameOver to true to end the loop
			gameOver = true;
			break;
		default:
			// If the current room number does not match any of the cases then we end the game 
			cout << "You have exited the dungeon<" << endl;
			gameOver = true; // End the game if there is an invalid room number
			break;
		}
	}

	cout << "Thanks for playing" << player.name << "!" << endl;
	return 0;
}

// This function will roll a 20 sided die and return the result as an integer. 
int rollDmg() {
	return rand() % 20 + 1; 
}

// This function will handle the combat between the player and a monster.
// It takes 2 parameters
// 1. A reference to the player object
// 2. A reference to the monster object for the monster that the player is fighting
// Since we are passing the player and monster by reference any changes made to their stats in this function will change the original objects that were passed in.
void combat(Player& player, Monster& monster) {
	// First we display the name and stats of the monster that the player is fighting
	cout << "You are fighting a " << monster.name << "!" << endl;

	// We start the combat loop and it continues as long as both the player and the monster are alive
	while (player.hp > 0 && monster.hp > 0) {
		// We display the player and monster's current stats at the start of each turn
		cout << "Player HP: " << player.hp << " | Monster HP: " << monster.hp << endl;
		// We give the player a choice of actions to take during their turn
		cout << "Choose your action:" << endl;
		cout << "1: Attack\n2: Block\n3: Use Item\n4: Exit Combat" << endl;

		// Then we get the player's choice for what action they want to take during their turn
		int actionChoice;
		cin >> actionChoice;


		switch (actionChoice) {
		case ATTACK:
		case BLOCK:
		case USE_ITEM:
		case EXIT:
		default:
			cout << "Invalid choice! Please select a valid action number." << endl;
			break;
		}

		// After the player's turn we check to see if the monster is still alive. 
		if (monster.hp > 0) {
			// If the monster is still alive then it takes its turn to attack the player
			cout << "The " << monster.name << " attacks you!" << endl;
			player.hp -= monster.atkPwr; // The monster's attack power is subtracted from the player's HP
		}
		
	}
}

