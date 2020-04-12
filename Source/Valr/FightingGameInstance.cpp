// Code by Sultan Iljasov, 2020.


#include "FightingGameInstance.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Misc/Paths.h"

UFightingGameInstance::UFightingGameInstance()
{
	FString path = FPaths::ProjectDir() + "Player/PlayerData.sav";
	std::string fullPath = TCHAR_TO_UTF8(*path);
	std::stringstream buffer;
	std::ifstream file;
	file.open(fullPath, std::ifstream::in);
	if (!file.is_open()) return;
	buffer << file.rdbuf();
	PlayerCoins = std::atoi(buffer.str().c_str());
	UE_LOG(LogTemp, Warning, TEXT("c: %i"),PlayerCoins)
	file.close();
}

void UFightingGameInstance::SavePlayerFile()
{
	FString path = FPaths::ProjectDir() + "Player/PlayerData.sav";
	std::string fullPath = TCHAR_TO_UTF8(*path);
	std::ofstream out(fullPath);
	if (!out.good())
	{
		return;
	}
	out << PlayerCoins;
	out.close();
}
