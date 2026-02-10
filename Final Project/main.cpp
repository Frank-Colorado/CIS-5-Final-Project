#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::setw;
using std::left;
using std::right;
using std::fixed;
using std::setprecision;
using std::srand;
using std::rand;
using std::time;
using std::min;

// ------------------------------------------------
// PLAYER COMBAT ENUM
// ------------------------------------------------
enum PlayerAction {
	ATTACK = 1,
	BLOCK,
	USE_ITEM,
	EXIT
};

// ------------------------------------------------
// COMBAT RESULT ENUM
// ------------------------------------------------
enum CombatResult {
	PLAYER_WON,
	PLAYER_DIED,
	PLAYER_EXITED,
};

// ------------------------------------------------
// ITEM ENUM
// ------------------------------------------------
enum Item {
	HEALTH_POTION,
	STRENGTH_ELIXIR
};

// ------------------------------------------------
// PLAYER STRUCTURE
// ------------------------------------------------
struct Player {
	string name;
	int hp;
	int atkPwr;
	int block = 0; // Block stat that can be increased by using a block action in combat. I set it to 0 by default and I will reset it to 0 at the end of each combat.
	Item inventory[3] = {}; // Simple inventory with 3 slots
	int inventorySize = 0; // Inventory starts empty

	Player(const string& name, int hp, int atkPwr) : name(name), hp(hp), atkPwr(atkPwr) {}

	void displayStats() const {
		cout << "You currently have " << hp << " HP and " <<atkPwr << " attack power." << endl;
	}

	string itemToString(const Item& item) const {
		switch (item) {
		case HEALTH_POTION:
			return "Health Potion";
		case STRENGTH_ELIXIR:
			return "Strength Elixir";
		default:
			return "Unknown Item";
		}
	}

	void addItem(const Item item) {
		// Check the current inventory size before adding an item
		if (inventorySize < 3) {
			inventory[inventorySize] = item;
			inventorySize++;
			cout << "You have added " << itemToString(item) << " to your inventory." << endl;
		}
		else {
			cout << "Your inventory is full! You cannot add " << itemToString(item) << "." << endl;
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
			cout << (i + 1) << ": " << itemToString(inventory[i]) << endl;
		}

		// Display final option for closing the inventory 
		cout << "0: Close Inventory";
	}

	// Method to use items from the player's inventory 
	void useItem() {

		// Check to see if there are any items in the inventory before displaying
		if (inventorySize == 0) {
			cout << "Your inventory is empty!" << endl;
			return; // Exit the method if there are no items to use
		}

		// Call the method to display the player's inventory
		displayInventory();

		// Get the player's choice
		cout << "Select the number of the item you want to use or 0 to close your inventory: ";
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
		Item selectedItem = inventory[choice - 1];

		switch (selectedItem) {
		case HEALTH_POTION:
			hp += 50; // Heal the player by 50 HP
			// I don't want the player's HP to exceed 100 so I check to see if it does and if so I set it back to 100
			if (hp > 100) {
				hp = 100;
			};
			// Display the player's new stat total
			displayStats();
			break;
		case STRENGTH_ELIXIR:
			// If the selected item is a strength elixr then we add 20 points to their atk
			atkPwr += 20;
			// Display the player's new stat total
			displayStats();
			break;
		default:
			cout << "Invalid item! Please select a valid item number." << endl;
			return;
		};
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
CombatResult combat(Player& player, Monster& monster);
int rollD20();
void applyDamage(Player& player, int damage);




int main() {
	srand(static_cast<unsigned int>(time(nullptr)));

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

		// ------------------------------------------------ ROOM 1 ------------------------------------------------
		case 1:
			// Here we initiate the first room of the dungeon and give the player a choice to either explore the chamber or head down the hallway
			// We also give the player the basic options to check their stats, inventory, or exit the game
			// Because we are giving so many options for a player to choose from in each room we will use nested switch statements to handle the logic for each choice.
			cout << "You enter a large dark and musty chamber. In front of you looms a long and narrow hallway. Do you choose to head down the hallway or explore the chamber first?" << endl;
			cout << "1: Move down the hallway" << endl;
			cout << "2: Explore the chamber" << endl;
			cout << "3. Current Stats" << endl;
			cout << "4. Inventory" << endl;
			cout << "5. Exit Game" << endl;

			// Here we get the player's choice for what they want to do in the first room
			int roomChoice;
			cin >> roomChoice;

			// ----------------------------------------------- ROOM 1 CHOICES ------------------------------------------------
			switch (roomChoice) {
			case 1:
				// If the player chooses to head down the hallway then we move to the next room 
				// No rewards or consequences for choosing this option
				cout << "You cautiously make your way down the hallway..." << endl;
				currentRoom++; // Move to the next room
				break;
			case 2: {
				// If the player choose to explore the chamber then we give them a reward but also a consequence for taking something that wasn't theirs
				cout << "You decide to explore the chamber and find a health potion hidden in a chest! But something in the shadows of the great chamber seems upset that you took something that wasn't yours. You are confronted by a Goblin!" << endl;
				// We add the health potion to the player's inventory
				player.addItem(HEALTH_POTION);
				// Here we would call a function to handle the combat between the player and the goblin
				CombatResult combatResult = combat(player, goblin);
				// After the combat we check to return type of the combat result 
				if (combatResult == PLAYER_WON) {
					cout << "After your victory you decide to head down the hallway..." << endl;
					// If the player won the combat then we move to the next room
					currentRoom++;
				}
				else if (combatResult == PLAYER_DIED) {
					// If the player died in combat then we tell the player that they have died and end the game by setting gameOver to true
					cout << "You have died in combat to the " << goblin.name << " . Game Over." << endl;
					gameOver = true;
				}
				else if (combatResult == PLAYER_EXITED) {
					// If the player chose to exit combat then we tell the player that they have fled and abandoned their quest and end the game by setting gameOver to true
					cout << "You have fled from combat and abandoned your quest. Game Over." << endl;
					gameOver = true;
				}
				break;
			}
			case 3: 
				// If the player chooses to check stats then we call displayStats method
				player.displayStats();
				// Since the player is still in the same room we don't change the current room number and we just break and the previous switch statement will run again with the same room options for the player to choose from
				break;
			case 4:
				// If the player chooses to check their inventory then we call the useItem method which will display the player's inventory and allow them to use an item if they choose to
				player.useItem();
				// Again we break without changing the current room number
				break;
			case 5:
				// If the player chooses to exit the game then we set gameOver to true to end the loop and end the game
				cout << "You have chosen to exit the game." << endl;
				gameOver = true;
				break;
			default:
				// If the player enters an invalid choice then we display an error message
				cout << "Invalid choice! Please select a valid option number." << endl;
				break;
			}
			break;

		// ------------------------------------------------ ROOM 2 ------------------------------------------------
		case 2:
			cout << "After traversing the long, narrow hallway you enter a dimly lit room. As your stumble around the room you find your self face to face with a large orc!" << endl;
			cout << "You realize the orc seems to be holding something shiny that you might want." << endl;
			cout << "You could challenge the orc and take what it's holding for yourself or you could use the darkness of the room to flee but you might not escape unscathed.";
			cout << "1: Challenge the orc for the shiny object." << endl;
			cout << "2. Make a run for it!" << endl;
			cout << "3. Currents Stats" << endl;
			cout << "4. Inventory" << endl;
			cout << "5. Exit Game" << endl;
			
			// We get the player's choice for this room 
			int room2Choice;
			cin >> room2Choice;
			// ----------------------------------------------- ROOM 2 CHOICES ------------------------------------------------
			switch (room2Choice) {
			case 1: {
				cout << "You decide to challenge the orc for the shiny object. You engage in combat with the orc!" << endl;
				// Here we call the function to handle the combat between the player and the orc
				CombatResult combatResult = combat(player, orc);
				// After the combat we check to return type of the combat result
				if (combatResult == PLAYER_WON) {
					cout << "After your victory you take the shiny object off the orc and find that it was a strength elixir! You add it to your inventory and then you head to the next room..." << endl;
					// Add the strength elix to the player's inventory 
					player.addItem(STRENGTH_ELIXIR);
					// Then we move on to the next room 
					currentRoom++;
				}
				else if (combatResult == PLAYER_DIED) {
					// If the player died in combat then we tell the player that they have died and end the game by setting gameOver to true
					cout << "You have died in combat to the " << orc.name << " . Game Over." << endl;
					gameOver = true;
				}
				else if (combatResult == PLAYER_EXITED) {
					// If the player chose to exit combat then we tell the player that they have fled and abandoned their quest and end the game by setting gameOver to true
					cout << "You have fled from combat and abandoned your quest. Game Over." << endl;
					gameOver = true;
				}
			}
				  break;
			case 2: {
				cout << "You bring your body low to the ground and cloak yourself in the darkness of the room. You attempt to slip past the orc but he catches you with a blow to the side." << endl;
				// Here we roll a D20 to check how much dmg the player takes from the orc's attack as they try to flee
				// We will also use this roll to see if the blow was a critical hit (15 or higher) or if it was a critical fail (5 or lower)
				int fleeRoll = rollD20();
				// If the flee roll is 15 or higher then we consider that a critical hit.
				// The player will take the damage and be caught by the orc and forced into combat.
				if (fleeRoll >= 15) {
					cout << "Critical Hit! The orc's blow is especially powerful and knocks you to the ground. Now you have no way to escape and have to fight to survive!" << endl;
					// We calculate the damage the orc deals to the player by rolling a D20 and adding the orc's attack power.
					int orcDmg = rollD20() + orc.atkPwr;
					// Then we subtract that damage from the player's HP. 
					applyDamage(player, orcDmg);
					// After the player takes damage from the orc's attack then we check to see if they are still alive. 
					if (player.hp > 0) {
						// If the player is still alive then we call the combat function to handle the combat between the player and the orc
						CombatResult combatResult = combat(player, orc);
						// After the combat we check to return type of the combat result
						if (combatResult == PLAYER_WON) {
							cout << "After your victory you take the shiny object off the orc and find that it was a strength elixir! You add it to your inventory and then you head to the next room..." << endl;
							// Add the strength elix to the player's inventory 
							player.addItem(STRENGTH_ELIXIR);
							// Then we move on to the next room 
							currentRoom++;
						}
						else if (combatResult == PLAYER_DIED) {
							// If the player died in combat then we tell the player that they have died and end the game by setting gameOver to true
							cout << "You have died in combat to the " << orc.name << " . Game Over." << endl;
							gameOver = true;
						}
						else if (combatResult == PLAYER_EXITED) {
							// If the player chose to exit combat then we tell the player that they have fled and abandoned their quest and end the game by setting gameOver to true
							cout << "You have fled from combat and abandoned your quest. Game Over." << endl;
							gameOver = true;
						}
					}
					else {
						// If the player is not alive after taking damage from trying to flee then we tell them that they have died and end the game by setting gameOver to true
						cout << "The blow from the orc was too much for you and you died as you tried to flee. Game Over." << endl;
						gameOver = true;
					}
				} else if (fleeRoll <= 5) {
						// If the flee roll is 5 or lower then we consider that a critical fail. 
						// The player takes no damage from the orc and they are able to swipe the shiny object off its waist in the process
						cout << "Critical Miss! Your dexterity is unmatched and the orc's blow slides of your body as if you were darkness iteself." << endl;
						cout << "The orc stumbles after his failed attack and you use this opportunity to swipe the shiny object off his waist! You find that it was a strength elixir and you add it to your inventory." << endl;
						player.addItem(STRENGTH_ELIXIR);
						cout << "You successfully flee to the next room and leave the confused orc behind you..." << endl;
						currentRoom++;
				} else {
					// If the flee roll is between 6 and 14 then we consider that a normal attempt to flee. The player takes damage from the orc's attack but they are able to escape and flee to the next room
					cout << "You take the hit from the orc and lose your footing." << endl;
					// We calculate the damage the orc deals to the player by rolling a D20 and adding the orc's attack power.
					int orcDmg = rollD20() + orc.atkPwr;
					// Then we subtract that damage from the player's HP.
					applyDamage(player, orcDmg);
					// After the player takes damage from the orc's attack then we check to see if they are still alive.
					if (player.hp > 0) {
						cout << "The damage wasn't enough to bring you down and you successfully flee to the next room and leave the orc behind you..." << endl;
						currentRoom++;
					}
					else {
						// If the player is not alive after taking damage from trying to flee then we tell them that they have died and end the game by setting gameOver to true
						cout << "The blow from the orc was too much for you and you died as you tried to flee. Game Over." << endl;
						gameOver = true;
					}
				}
			}
			break;
			case 3:
				// If the player chooses to check stats then we call displayStats method
				player.displayStats();
				// Since the player is still in the same room we don't change the current room number and we just break and the previous switch statement will run again with the same room options for the player to choose from
				break;
			case 4:
				// If the player chooses to check their inventory then we call the useItem method which will display the player's inventory and allow them to use an item if they choose to
				player.useItem();
				// Again we break without changing the current room number
				break;
				break;
			case 5: 
				// If the player chooses to exit the game then we set gameOver to true to end the loop and end the game
				cout << "You have chosen to exit the game." << endl;
				gameOver = true;
				break;
			default:
				// If the player enters an invalid choice then we display an error message
				cout << "Invalid choice! Please select a valid option number." << endl;
				break;
			}
			break;
		
		// ------------------------------------------------ ROOM 3 ------------------------------------------------
		case 3:
			cout << "You enter the final room and face off against a powerful necromancer!" << endl;
			// Here we would call a function to handle the combat between the player and the necromancer
			// If the player wins then they have completed the game and we set gameOver to true to end the loop
			gameOver = true;
			break;
		default: {
			// If the current room number does not match any of the cases then we end the game 
			cout << "You have exited the dungeon<" << endl;
			gameOver = true; // End the game if there is an invalid room number
			break;
			}
		}
	}

	cout << "Thanks for playing " << player.name << "!" << endl;
	return 0;
}

// This function will roll a 20 sided die and return the result as an integer. 
int rollD20() {
	return rand() % 20 + 1; 
}

// This function will apply damage to the player. It will calculate the damage taken while taking in to consideration the player's block stat
// This function takes 2 parameters
// 1. A reference to the player object
// 2. An integer for the amount of damage being dealt to the player before block is applied
void applyDamage(Player& player, int damage) {
	// First we check to see if the player has any block at all
	if (player.block > 0) {
		// If they do then we check to see if either the player's block or the damage being dealt is greater 
		// Min finds the smaller of the two values and that is the amount of damage that will be absorbed by the player's block
		int absorbedDamage = min(player.block, damage);
		// We subtract the absorber damage from the player's block stat
		// For example if the player has 10 block and they are being dealt 15 damage then 'absorbedDamage' will be 10 and the player's block will be reduced to 0
		// If the player has 10 block and they are being dealt 5 damage then 'absorbedDamage' will be 5 and the player's block will be reduced to 5
		player.block -= absorbedDamage;
		// Then we also subtract the absorbed damage from the total damage being dealt to the player so that we can calculate the final damage that will be subtracter from the player's HP
		// In this case, if the player has 10 block and they are being dealt 15 damage then 'absorbedDamage' will be 10 and the damage will be reduced to 5 which is the amount that will be subtracted from the player's HP
		// But if the player had 10 block and they were being dealt 5 damage then 'absorbedDamage' will be 5 and the damage will be reduced to 0.
		// This means the future conditional check for damage > 0 will be false and the player's HP stat won't be modified
		damage -= absorbedDamage;

		// Tell the player how much damage their block absorbed and how much block they have left
		cout << "Your block absorbed " << absorbedDamage << " damage!" << endl;
		cout << "Your remaining block is: " << player.block << endl;
	}

	// After we have checked to see if the player has any block then we check to see if there is any damage left to apply to the player's HP after block has been applied
	// In this case, if the player had no block then the damage value passed in would not be modified and then the full damage would be done to the player's HP.
	// If the player did block, then the damage value passed in would be reduced by the amount of block that the player had or the full amount might have been absorbed.
	if (damage > 0) {
		player.hp -= damage; 
		cout << "You take " << damage << " damage! Your remaining HP is: " << player.hp << endl;
	}
}

// This function will handle the combat between the player and a monster.
// It takes 2 parameters
// 1. A reference to the player object
// 2. A reference to the monster object for the monster that the player is fighting
// Since we are passing the player and monster by reference any changes made to their stats in this function will change the original objects that were passed in.
CombatResult combat(Player& player, Monster& monster) {
	// Create a variable to track the result of the combat and initialize it to PLAYER_EXITED. This is because if the player chooses to exit combat then we will return this result and end the combat loop.
	CombatResult result = PLAYER_EXITED;
	// We also create a boolean variable to track if the combat is over or not and initialize it to false. This is because the combat loop will continue until the combat is over.
	bool combatOver = false;

	// First we display the name and stats of the monster that the player is fighting
	cout << "You are fighting a " << monster.name << "!" << endl;

	// We start the combat loop and it continues as long as both the player and the monster are alive
	while (!combatOver && player.hp > 0 && monster.hp > 0) {
		// We display the player and monster's current stats at the start of each turn
		cout << "Player HP: " << player.hp << " | Monster HP: " << monster.hp << endl;
		// We give the player a choice of actions to take during their turn
		cout << "Choose your action:" << endl;
		cout << "1: Attack\n2: Block\n3: Use Item\n4: Exit Combat" << endl;

		// Then we get the player's choice for what action they want to take during their turn
		int actionChoice;
		cin >> actionChoice;


		switch (actionChoice) {
		case ATTACK: {
			// If the player chooses to attack then we calculate the damage they deal the monster by rolling a 20 sided die
			// Then we add the player's attack power to the damage rolled 
			int playerDmg = rollD20() + player.atkPwr;
			// We subtract the damage dealt from the monster's HP
			monster.hp -= playerDmg;
			// Then we print out the damage dealt to the monster
			cout << "You attack the " << monster.name << " and deal " << playerDmg << " damage!" << endl;
			break;
		}
	
		case BLOCK: {
			// If the player chooses to block then we will increase their block stat by rolling a D20. 
			// This block stat will reduce the damage taken from attacks 
			int blockAmount = rollD20();
			// We add the block amount to the player's block stat
			player.block += blockAmount;
			// We also print out the amount that the player has blocked for this turn
			cout << "You block and increase your block stat by " << blockAmount << " for this turn!" << endl;
			// Then we print what the player's new block stat total is
			cout << "Your current block stat is: " << player.block << endl;
			break;
		}

		case USE_ITEM: {
			// If the player chooses to use an item then we call the useItem method from the Player structure 
			player.useItem();
			break;
		}
		
		case EXIT: {
			// If the player chooses to exit combat then we set combatOver to true to end the combat loop and we return the result of PLAYER_EXITED
			cout << "You have chosen to exit combat." << endl;
			combatOver = true;
			result = PLAYER_EXITED;
			break;
		}
	
		default:
			cout << "Invalid choice! Please select a valid action number." << endl;
			break;
		}

		// After the player's turn we check to see if the monster is still alive. 
		if (monster.hp > 0) {
			// We calculate the damage the monster deals to the player by rolling a D20 and adding the monster's attack power
			int monsterDmg = rollD20() + monster.atkPwr;
			// Then we print out the damage that the monster is trying to deal to the player
			cout << "The " << monster.name << " attacks you for " << monsterDmg << " damage!" << endl;
			// Then we call the applyDamage function to apply the damage to the player
			applyDamage(player, monsterDmg);
		}
		// After the monster's turn the while loop will check again to see if both the player and monster are still alive and if combatOver is still false.
		// If any of those conditions are false then the combat loop will break and we will check the result of the combat to see if the player won, died, or exited combat.
	}

	// After the combat loop we check to see if the player is still alive and if so we return that they won the combat. If the player is not alive then we return that they died in combat. If the player chose to exit combat then we return that they exited combat.
	if (player.hp > 0) {
		cout << "You have defeated the " << monster.name << "!" << endl;
		result = PLAYER_WON;
	}
	else {
		cout << "You have been defeated by the " << monster.name << "! Game Over." << endl;
		result = PLAYER_DIED;
	}

	return result;
}

