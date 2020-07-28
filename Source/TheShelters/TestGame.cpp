#include "RoomControl.h"

void ARoomControl::PrintMap()
{
	for (unsigned int x = 0; x < maxHeight; x++)
	{
		FString line = FString();
		for (unsigned int y = 0; y < maxWidth; y++)
		{
			int idx = x * maxWidth + y;
			int32 monsterId = GameMap[idx]->MonsterId();
			TArray<FStringFormatArg> args;
			args.Add(FStringFormatArg(monsterId));
			line += FString::Format(TEXT("{0} "), args);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *line);
	}
}

void ARoomControl::PrintTestMessage(const TCHAR *testName, const int num, const bool success)
{
	FString line = FString();
	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(testName));
	args.Add(FStringFormatArg(num));
	if (success)
	{
		line += FString::Format(TEXT("[{0}] TEST {1}: SUCCESS"), args);
	}
	else
	{
		line += FString::Format(TEXT("[{0}] TEST {1}: FAIL"), args);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *line);
	testResult[success] += 1;
}

void ARoomControl::TestScenario(FString _LevelString)
{
	bool result;
	testResult.Add(true, 0);
	testResult.Add(false, 0);

	InitGame(10, 10, _LevelString);
	result = this->panicRoomId == 5;
	PrintTestMessage(TEXT("InitPanicRoom"), 1, result);

	result = GameMap[5]->GetDoor(Left).status == Close;
	PrintTestMessage(TEXT("InitPanicRoom"), 2, result);
	result = GameMap[5]->GetDoor(Up).status == Open;
	PrintTestMessage(TEXT("InitPanicRoom"), 3, result);
	result = GameMap[5]->GetDoor(Right).status == Open;
	PrintTestMessage(TEXT("InitPanicRoom"), 4, result);
	result = GameMap[5]->GetDoor(Down).status == Open;
	PrintTestMessage(TEXT("InitPanicRoom"), 5, result);

	result = GameMap[4]->GetDoor(Right).status == Close;
	PrintTestMessage(TEXT("InitPanicRoom"), 6, result);
	result = GameMap[6]->GetDoor(Left).status == Open;
	PrintTestMessage(TEXT("InitPanicRoom"), 7, result);
	result = GameMap[15]->GetDoor(Up).status == Open;
	PrintTestMessage(TEXT("InitPanicRoom"), 8, result);

	InsertMonster(DefaultMonster, 0, 4); // Monster 1: 0, 4
	InsertMonster(DefaultMonster, 1, 3); // Monster 2: 1, 3

	MoveMonster(1, Left);  // Monster 1: 0, 3
	MoveMonster(2, Right); // Monster 2: 1, 4

	// EXPECT
	result = GameMap[4]->MonsterId() == 0 && GameMap[3]->MonsterId() == 1;
	PrintTestMessage(TEXT("MonsterMovement"), 1, result);

	result = GameMap[13]->MonsterId() == 0 && GameMap[14]->MonsterId() == 2;
	PrintTestMessage(TEXT("MonsterMovement"), 2, result);

	URoom *room = FindRoomByLocation(3, 3);
	room->CloseDoor(Left);
	result = GameMap[33]->GetDoor(Left).status == Close;
	PrintTestMessage(TEXT("CloseDoor"), 1, result);

	result = GameMap[32]->GetDoor(Right).status == Close;
	PrintTestMessage(TEXT("CloseDoor"), 2, result);

	result = playerStat->Food() == 100;
	PrintTestMessage(TEXT("InitPlayerStat"), 1, result);
	result = playerStat->Water() == 100;
	PrintTestMessage(TEXT("InitPlayerStat"), 2, result);
	result = playerStat->Mental() == 50;
	PrintTestMessage(TEXT("InitPlayerStat"), 3, result);
	result = playerStat->Progress() == 0;
	PrintTestMessage(TEXT("InitPlayerStat"), 4, result);
	result = playerStat->Electricity() == 100;
	PrintTestMessage(TEXT("InitPlayerStat"), 5, result);

	playerStat->EndTurn();
	result = playerStat->Food() == 99;
	PrintTestMessage(TEXT("PlayerStatEndTurn"), 1, result);
	result = playerStat->Water() == 98;
	PrintTestMessage(TEXT("PlayerStatEndTurn"), 2, result);
	result = playerStat->Mental() == 54;
	PrintTestMessage(TEXT("PlayerStatEndTurn"), 3, result);

	playerStat->EndTurn();
	result = playerStat->Food() == 98;
	PrintTestMessage(TEXT("PlayerStatSecondEndTurn"), 1, result);
	result = playerStat->Water() == 96;
	PrintTestMessage(TEXT("PlayerStatSecondEndTurn"), 2, result);
	result = playerStat->Mental() == 57.98;
	PrintTestMessage(TEXT("PlayerStatSecondEndTurn"), 3, result);

	result = GameMap[77]->GetDoor(Right).status == Close;
	PrintTestMessage(TEXT("LevelStringTest"), 2, result);

	UE_LOG(LogTemp, Warning, TEXT("***[SUCCESS: %d FAIL: %d]"), testResult[true], testResult[false])
}
