#include "Room.h"
#include <iostream>

UPanicRoom::UPanicRoom()
{
}

UPanicRoom::~UPanicRoom()
{
}

void UPanicRoom::InitPanicRoom(const DoorStatus left, const DoorStatus right, const DoorStatus up,
                               const DoorStatus down, const int newRoomId)
{
    if ((left == Open && right == Open && up == Open && down == Open) ||
        (left == Close && right == Close && up == Close && down == Close))
    {
        // randomise
        UE_LOG(LogTemp, Warning, TEXT("***Why not running?***"));
    }
    else
    {
        TMap<Direction, DoorStatus> param;
        param.Add(Left, left);
        param.Add(Right, right);
        param.Add(Up, up);
        param.Add(Down, down);

        for (auto &i : param)
        {
            if (GetDoor(i.Key).connectedRoom == nullptr)
            {
                continue;
            }
            if (i.Value == Open)
            {
                OpenDoor(i.Key);
            }
            else
            {
                CloseDoor(i.Key);
            }
        }
    }
}