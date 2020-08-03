#include "Monster.h"
#include "RoomControl.h"

void ARoomControl::PrintMap()
{
    UE_LOG(LogTemp, Warning, TEXT("<<<<< MAP >>>>>"));
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

    InsertMonster(DefaultMonster, 0, 4); // Monster 1: 0, 4
    InsertMonster(DefaultMonster, 1, 3); // Monster 2: 1, 3

    result = GameMap[4]->MonsterId() == 1 && GameMap[13]->MonsterId() == 2;
    PrintTestMessage(TEXT("InsertMonster"), 1, result);

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

    result = survivorStat->Food() == 100;
    PrintTestMessage(TEXT("InitSurvivorStat"), 1, result);
    result = survivorStat->Water() == 100;
    PrintTestMessage(TEXT("InitSurvivorStat"), 2, result);
    result = survivorStat->Hunger() == 100;
    PrintTestMessage(TEXT("InitSurvivorStat"), 3, result);
    result = survivorStat->Thirst() == 100;
    PrintTestMessage(TEXT("InitSurvivorStat"), 4, result);
    result = survivorStat->Mental() == 50;
    PrintTestMessage(TEXT("InitSurvivorStat"), 5, result);
    result = survivorStat->Progress() == 100;
    PrintTestMessage(TEXT("InitSurvivorStat"), 6, result);
    result = survivorStat->Electricity() == 100;
    PrintTestMessage(TEXT("InitSurvivorStat"), 7, result);

    survivorStat->EndTurn();
    result = survivorStat->Hunger() == 99;
    PrintTestMessage(TEXT("SurvivorStatEndTurn"), 1, result);
    result = survivorStat->Thirst() == 98;
    PrintTestMessage(TEXT("SurvivorStatEndTurn"), 2, result);
    result = survivorStat->Mental() == 54;
    PrintTestMessage(TEXT("SurvivorStatEndTurn"), 3, result);

    survivorStat->EndTurn();
    result = survivorStat->Hunger() == 98;
    PrintTestMessage(TEXT("SurvivorStatSecondEndTurn"), 1, result);
    result = survivorStat->Thirst() == 96;
    PrintTestMessage(TEXT("SurvivorStatSecondEndTurn"), 2, result);
    result = survivorStat->Mental() == 57.98;
    PrintTestMessage(TEXT("SurvivorStatSecondEndTurn"), 3, result);

    result = GameMap[77]->GetDoor(Right).status == Close;
    PrintTestMessage(TEXT("LevelStringTest"), 2, result);

    UE_LOG(LogTemp, Warning, TEXT("***[SUCCESS: %d FAIL: %d]"), testResult[true], testResult[false])
}
