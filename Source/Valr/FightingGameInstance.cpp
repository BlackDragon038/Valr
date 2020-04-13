// Code by Sultan Iljasov, 2020.


#include "FightingGameInstance.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Misc/Paths.h"

UFightingGameInstance::UFightingGameInstance()
{
	
}

void UFightingGameInstance::Init()
{
	Super::Init();
	FString path = FPaths::ProjectSavedDir() + "PlayerData.sav";
	std::string fullPath = TCHAR_TO_UTF8(*path);
	std::ifstream file;
	file.open(fullPath, std::ifstream::in);
	if (!file.is_open())
	{
		SavePlayerFile();
	}
	else
	{
		std::string oneLine;
		std::string oneWord;

		while (std::getline(file, oneLine))
		{
			std::stringstream sStream;
			sStream << oneLine;
			oneWord.clear();
			sStream >> oneWord;
			if (oneWord == "Coins")
			{
				sStream >> oneWord;
				PlayerCoins = std::stoi(oneWord);
				UE_LOG(LogTemp, Warning, TEXT("Coins: %i"), PlayerCoins)
			}
			else if (oneWord == "Crusader")
			{
				sStream >> oneWord;
				Crusader.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Crusader.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Viking")
			{
				sStream >> oneWord;
				Viking.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Viking.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Samurai")
			{
				sStream >> oneWord;
				Samurai.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Samurai.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Knight")
			{
				sStream >> oneWord;
				Knight.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Knight.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Ninja")
			{
				sStream >> oneWord;
				Ninja.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Ninja.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Spartan")
			{
				sStream >> oneWord;
				Spartan.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Spartan.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Soldier")
			{
				sStream >> oneWord;
				Soldier.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Soldier.Special_2.B = std::stof(oneWord);
			}
			else if (oneWord == "Janissary")
			{
				sStream >> oneWord;
				Janissary.Headgear.R = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Headgear.G = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Headgear.B = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Armor_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Armor_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Armor_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Armor_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Armor_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Armor_2.B = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Special_1.R = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Special_1.G = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Special_1.B = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Special_2.R = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Special_2.G = std::stof(oneWord);
				sStream >> oneWord;
				Janissary.Special_2.B = std::stof(oneWord);
			}
		}
		file.close();
	}
}

void UFightingGameInstance::SavePlayerFile()
{
		FString path = FPaths::ProjectSavedDir() + "PlayerData.sav";
		std::string fullPath = TCHAR_TO_UTF8(*path);
		std::ofstream out(fullPath);
		if (!out.good())
		{
			return;
		}
		out << "Coins ";
		out << PlayerCoins << "\n";

		out << "Crusader ";
		out << Crusader.Headgear.R << " " << Crusader.Headgear.G << " " << Crusader.Headgear.B << " ";
		out << Crusader.Armor_1.R << " " << Crusader.Armor_1.G << " " << Crusader.Armor_1.B << " ";
		out << Crusader.Armor_2.R << " " << Crusader.Armor_2.G << " " << Crusader.Armor_2.B << " ";
		out << Crusader.Special_1.R << " " << Crusader.Special_1.G << " " << Crusader.Special_1.B << " ";
		out << Crusader.Special_2.R << " " << Crusader.Special_2.G << " " << Crusader.Special_2.B << " ";

		out << "\nViking ";
		out << Viking.Headgear.R << " " << Viking.Headgear.G << " " << Viking.Headgear.B << " ";
		out << Viking.Armor_1.R << " " << Viking.Armor_1.G << " " << Viking.Armor_1.B << " ";
		out << Viking.Armor_2.R << " " << Viking.Armor_2.G << " " << Viking.Armor_2.B << " ";
		out << Viking.Special_1.R << " " << Viking.Special_1.G << " " << Viking.Special_1.B << " ";
		out << Viking.Special_2.R << " " << Viking.Special_2.G << " " << Viking.Special_2.B << " ";

		out << "\nSamurai ";
		out << Samurai.Headgear.R << " " << Samurai.Headgear.G << " " << Samurai.Headgear.B << " ";
		out << Samurai.Armor_1.R << " " << Samurai.Armor_1.G << " " << Samurai.Armor_1.B << " ";
		out << Samurai.Armor_2.R << " " << Samurai.Armor_2.G << " " << Samurai.Armor_2.B << " ";
		out << Samurai.Special_1.R << " " << Samurai.Special_1.G << " " << Samurai.Special_1.B << " ";
		out << Samurai.Special_2.R << " " << Samurai.Special_2.G << " " << Samurai.Special_2.B << " ";

		out << "\nKnight ";
		out << Knight.Headgear.R << " " << Knight.Headgear.G << " " << Knight.Headgear.B << " ";
		out << Knight.Armor_1.R << " " << Knight.Armor_1.G << " " << Knight.Armor_1.B << " ";
		out << Knight.Armor_2.R << " " << Knight.Armor_2.G << " " << Knight.Armor_2.B << " ";
		out << Knight.Special_1.R << " " << Knight.Special_1.G << " " << Knight.Special_1.B << " ";
		out << Knight.Special_2.R << " " << Knight.Special_2.G << " " << Knight.Special_2.B << " ";

		out << "\nNinja ";
		out << Ninja.Headgear.R << " " << Ninja.Headgear.G << " " << Ninja.Headgear.B << " ";
		out << Ninja.Armor_1.R << " " << Ninja.Armor_1.G << " " << Ninja.Armor_1.B << " ";
		out << Ninja.Armor_2.R << " " << Ninja.Armor_2.G << " " << Ninja.Armor_2.B << " ";
		out << Ninja.Special_1.R << " " << Ninja.Special_1.G << " " << Ninja.Special_1.B << " ";
		out << Ninja.Special_2.R << " " << Ninja.Special_2.G << " " << Ninja.Special_2.B << " ";

		out << "\nSpartan ";
		out << Spartan.Headgear.R << " " << Spartan.Headgear.G << " " << Spartan.Headgear.B << " ";
		out << Spartan.Armor_1.R << " " << Spartan.Armor_1.G << " " << Spartan.Armor_1.B << " ";
		out << Spartan.Armor_2.R << " " << Spartan.Armor_2.G << " " << Spartan.Armor_2.B << " ";
		out << Spartan.Special_1.R << " " << Spartan.Special_1.G << " " << Spartan.Special_1.B << " ";
		out << Spartan.Special_2.R << " " << Spartan.Special_2.G << " " << Spartan.Special_2.B << " ";

		out << "\nSoldier ";
		out << Soldier.Headgear.R << " " << Soldier.Headgear.G << " " << Soldier.Headgear.B << " ";
		out << Soldier.Armor_1.R << " " << Soldier.Armor_1.G << " " << Soldier.Armor_1.B << " ";
		out << Soldier.Armor_2.R << " " << Soldier.Armor_2.G << " " << Soldier.Armor_2.B << " ";
		out << Soldier.Special_1.R << " " << Soldier.Special_1.G << " " << Soldier.Special_1.B << " ";
		out << Soldier.Special_2.R << " " << Soldier.Special_2.G << " " << Soldier.Special_2.B << " ";

		out << "\nJanissary ";
		out << Janissary.Headgear.R << " " << Janissary.Headgear.G << " " << Janissary.Headgear.B << " ";
		out << Janissary.Armor_1.R << " " << Janissary.Armor_1.G << " " << Janissary.Armor_1.B << " ";
		out << Janissary.Armor_2.R << " " << Janissary.Armor_2.G << " " << Janissary.Armor_2.B << " ";
		out << Janissary.Special_1.R << " " << Janissary.Special_1.G << " " << Janissary.Special_1.B << " ";
		out << Janissary.Special_2.R << " " << Janissary.Special_2.G << " " << Janissary.Special_2.B << " ";

		out.close();
}
