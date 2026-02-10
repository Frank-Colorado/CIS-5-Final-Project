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

// Function prototypes



int main() {

	cout << "Welcome to this simple DnD like game!" << endl;
	cout << "Please enter the name of your character: ";
	string playerName;
	cin >> playerName;

	Player player(playerName, 100, 20);

	// intro dialogue displaying the player's name and stats
	cout << "Welcome, " << player.name << "! You are a brave adventurer embarking on a quest." << endl;
	player.displayStats();


	

	return 0;
}


