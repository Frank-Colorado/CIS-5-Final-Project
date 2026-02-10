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

	Player(const string& name, int hp, int atkPwr) : name(name), hp(hp), atkPwr(atkPwr) {}
};

int main() {

	cout << "Welcome to this simple DnD like game!" << endl;
	cout << "Please enter the name of your character: ";
	string playerName;
	cin >> playerName;

	Player player(playerName, 100, 20);

	cout << "\nYou have entered the world as " << player.name << " with " << player.hp << " HP and " << player.atkPwr << " attack power." << endl;

	return 0;
}