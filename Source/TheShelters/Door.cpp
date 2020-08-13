#include "Door.h"
#include "Room.h"

unsigned int ADoor::nextId = 1;

ADoor::ADoor()
{
}
ADoor::~ADoor()
{
}

void ADoor::InitDoor(URoom *roomA, URoom *roomB, TEnumAsByte<DoorStatus> s)
{
    if (connectedRooms.Num() != 0)
        throw "Door is not empty, Cannot initialize...";

    id = nextId++;
    connectedRooms.Add(roomA);
    connectedRooms.Add(roomB);
    /*
    FILE* f;
    f = fopen("D:\\trialDoor.txt", "a+");
    fprintf(f, "Connected Room[0]: %d Connected Room[1]: %d\n", connectedRooms[0]->RoomId(), connectedRooms[1]->RoomId());
    fclose(f);
    */
    status = s;
}

void ADoor::Open()
{
    status = DoorStatus::Open;
}

void ADoor::Close()
{
    status = DoorStatus::Close;
}


void ADoor::SwitchStatus()
{
    
    if (status == DoorStatus::Open)
    {
        status = DoorStatus::Close;
        UE_LOG(LogTemp, Warning, TEXT("closed"));
    }
    else if (status == DoorStatus::Close)
    {
        status = DoorStatus::Open;
    }
}

URoom *ADoor::GetBeyond(URoom *room)
{
    
    //FILE* f;
    //f = fopen("D:\\trial.txt", "a+");
    int idx = connectedRooms.Find(room);

    //fprintf(f, "Room: %d Connected Room[0]: %d Connected Room[1]: %d\n", room->RoomId(), connectedRooms[0]->RoomId(), connectedRooms[1]->RoomId());
    //fclose(f);
    //int temp = time(NULL)+1;
    //while (temp > time(NULL));

    if (idx == INDEX_NONE)
        throw "A given room is not connected to the door...";

    int newIdx = (idx + 1) % 2;
    return connectedRooms[newIdx];
}

const unsigned int ADoor::Id()
{
    return id;
}

const TEnumAsByte<DoorStatus> ADoor::Status() const
{
    return status;
}