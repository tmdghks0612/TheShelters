#include "Monster.h"
#include "LevelControl.h"

void ALevelControl::PrintMap()
{
    
    //UE_LOG(LogTemp, Warning, TEXT("<<<<< MAP >>>>>"));
    for (unsigned int x = 0; x < maxHeight; x++)
    {
        FString line = FString();
        for (unsigned int y = 0; y < maxWidth; y++)
        {
            int idx = x * maxWidth + y;
            int32 monsterId = GameMap[idx]->RoomId();
            TArray<FStringFormatArg> args;
            args.Add(FStringFormatArg(monsterId));
            line += FString::Format(TEXT("{0} "), args);
        }
    //    UE_LOG(LogTemp, Warning, TEXT("%s"), *line);
    }
    
}

void ALevelControl::PrintTestMessage(const TCHAR *testName, const int num, const bool success)
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


void ALevelControl::TestScenario(FString _LevelString)
{
    bool result;
    testResult.Add(true, 0);
    testResult.Add(false, 0);

    InitGame(10, 10, _LevelString);

    InsertMonster(MonsterType::Ghoul, 1, 4); // Monster 1: 0, 4
    InsertMonster(MonsterType::Grima, 1, 3); // Monster 2: 1, 3

    result = GameMap[4]->MonsterId() == 1 && GameMap[13]->MonsterId() == 2;
    PrintTestMessage(TEXT("InsertMonster"), 1, result);

    result = GameMap[13]->MonsterId() == 0 && GameMap[14]->MonsterId() == 2;
    PrintTestMessage(TEXT("MonsterMovement"), 2, result);

    URoom *room = FindRoomByLocation(3, 3);
    room->CloseDoor(Direction::Left);
    result = GameMap[33]->GetDoor(Direction::Left)->Status() == DoorStatus::Close;
    PrintTestMessage(TEXT("CloseDoor"), 1, result);

    result = GameMap[32]->GetDoor(Direction::Right)->Status() == DoorStatus::Close;
    PrintTestMessage(TEXT("CloseDoor"), 2, result);

    result = GameMap[77]->GetDoor(Direction::Right)->Status() == DoorStatus::Close;
    PrintTestMessage(TEXT("LevelStringTest"), 2, result);

    UE_LOG(LogTemp, Warning, TEXT("***[SUCCESS: %d FAIL: %d]"), testResult[true], testResult[false])
}
