#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

struct Player {
	string name;
	int hp;
	int atkPwr;
	string inventory[3]; // Simple inventory with 3 slots
	int inventorySize = 0; // Inventory starts empty

	Player(const string& name, int hp, int atkPwr) : name(name), hp(hp), atkPwr(atkPwr) {}

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

	void displayInventory() {

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

};

// Function prototypes
void displayPlayerStats(const Player& player);



int main() {

	cout << "Welcome to this simple DnD like game!" << endl;
	cout << "Please enter the name of your character: ";
	string playerName;
	cin >> playerName;

	Player player(playerName, 100, 20);

	// intro dialogue displaying the player's name and stats
	cout << "Welcome, " << player.name << "! You are a brave adventurer embarking on a quest." << endl;
	displayPlayerStats(player);


	

	return 0;
}

void displayPlayerStats(const Player& player) {
	cout << "You currently have " << player.hp << " HP and " << player.atkPwr << " attack power." << endl;
}

