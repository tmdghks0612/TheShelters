#include "Door.h"
#include "Room.h"

ADoor::ADoor() : id(nextId++)
{
}
ADoor::~ADoor()
{
}

void ADoor::InitDoor(URoom *roomA, URoom *roomB, DoorStatus s)
{
    if (connectedRooms.Num() != 0)
        throw "Door is not empty, Cannot initialize...";

    connectedRooms.Add(roomA);
    connectedRooms.Add(roomB);

    status = s;
}

void ADoor::Open()
{
    status = DoorStatusOpen;
}

void ADoor::Close()
{
    status = DoorStatusClose;
}

URoom *ADoor::GetBeyond(URoom *room)
{
    int idx = connectedRooms.Find(room);
    if (idx == INDEX_NONE)
        throw "A given room is not connected to the door...";

    int newIdx = (idx + 1) % 2;
    return connectedRooms[newIdx];
}

const unsigned int ADoor::Id() const
{
    return id;
}

const DoorStatus ADoor::Status() const
{
    return status;
}