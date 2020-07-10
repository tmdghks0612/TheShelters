// Fill out your copyright notice in the Description page of Project Settings.


#include "ShelterControl.h"

// Sets default values
AShelterControl::AShelterControl()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShelterControl::BeginPlay()
{
	Super::BeginPlay();

}

void AShelterControl::InitLevel() {

	InitGame(5, 5);
	InsertMonster(DefaultMonster, 4);
	InsertMonster(DefaultMonster, 8);
	return;
}

void AShelterControl::EndTurn() {

	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime!"));
	for (const TPair<int32, UMonster*>& it : Monsters) {
		if (!MoveMonster(it.Key, Direction(rand() % 4))) {
			MoveMonster(it.Key, Direction(rand() % 4));
		}
	}
	for (int i = 0; i < 5; ++i) {
		UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d %d"), GameMap[i*5]->GetMonsterId(), GameMap[i*5+1]->GetMonsterId(), GameMap[i*5+2]->GetMonsterId(), GameMap[i*5 + 3]->GetMonsterId(), GameMap[i*5 + 4]->GetMonsterId());
	}
	
	/*UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime3!"));
	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime4!"));
	URoom* room = FindRoomByLocation(1, 1);
	UE_LOG(LogTemp, Warning, TEXT("Room no : %d"), room->GetRoomNo());
	Door* d = room->GetDoor(Left);
	//Location l = room->GetDoor(Left)->ConnectedRoom->GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Room coord : %d, %d"), l.x, l.y);

	URoom* room = FindRoomByLocation(1, 0);
	UE_LOG(LogTemp, Warning, TEXT("Monster found id : %d"), 13);
	UE_LOG(LogTemp, Warning, TEXT("Monster found id : %d"), room->GetMonster()->GetMonsterId());
	*/
}

void AShelterControl::InitGame(const unsigned int m, const unsigned int n) {
	MaxHeight = m;
	MaxWidth = n;

	int size = MaxHeight * MaxWidth;
	
	for (int i = 0; i < size; i++) {
		int x = i / MaxWidth;
		int y = i % MaxWidth;

		UShelter *NewRoom = NewObject<UShelter>();
		NewRoom->InitShelter(i);
		NewRoom->SetLocation(x, y);
		GameMap.Add(NewRoom);
		UE_LOG(LogTemp, Warning, TEXT("loop : %d"), i);
		UE_LOG(LogTemp, Warning, TEXT("Room no : %d"), NewRoom->GetShelterNum());
	}

	// Connect all rooms
	for (int i = 0; i < size; i++) {
		// A idx is MaxWidth * x + y
		int x = i / MaxWidth;
		int y = i % MaxWidth;

		if (x != 0) {
			GameMap[i]->SetDoor(Up, MaxWidth * (x - 1) + y, Open);
		}
		if (x != (MaxHeight - 1)) {
			GameMap[i]->SetDoor(Down, MaxWidth * (x + 1) + y, Open);
		}
		if (y != 0) {
			GameMap[i]->SetDoor(Left, MaxWidth * x + (y - 1), Open);
		}
		if (y != (MaxWidth - 1)) {
			GameMap[i]->SetDoor(Right, MaxWidth * x + (y + 1), Open);
		}
	}
}


UShelter* AShelterControl::FindShelterByLocation(const unsigned int x, const unsigned int y) {
	if (x < 0 || x >= MaxHeight) {
		throw "You've exceeded map height";
	}
	if (y < 0 || y >= MaxWidth) {
		throw "You've exceeded map width";
	}

	int idx = MaxWidth * x + y;

	return GameMap[idx];
}

void AShelterControl::InsertMonster(MonsterType monsterType, int shelterNum) {
	GameMap[shelterNum]->InsertMonster(nextMonsterId);
	
	UMonster* newMonster = NewObject<UMonster>();
	newMonster->InitMonster(monsterType, nextMonsterId);
	Monsters.Add(nextMonsterId, newMonster);
	Shelters.Add(nextMonsterId, shelterNum);
	nextMonsterId++;
}

/*void AShelterControl::InsertMonster(const unsigned int x, const unsigned int y) {
	UShelter* shelter = FindShelterByLocation(x, y);
	InsertMonster(nextMonsterId, shelter);
	nextMonsterId++;
}*/ 
 /*
void AShelterControl::DeleteMonster(UShelter* shelter) {
	UMonster* monster = shelter->DeleteMonster();

	// FIXME: If there are two same monster id, it can go wrong...
	int targetId = monster->GetMonsterId();

	for (MonsterList::iterator it = Monsters.begin(); it != Monsters.end(); it++) {
		if (it->first->GetMonsterId() == targetId) {
			Monsters.erase(it);
		}
	}
	delete monster;
}

void AShelterControl::DeleteMonster(const unsigned int x, const unsigned int y) {
	URoom* room = FindRoomByLocation(x, y);
	DeleteMonster(room);
}
*/

bool AShelterControl::MoveMonster(int monsterId, Direction d) {
	for (const TPair<int32, int32>& it : Shelters) {
		if (it.Key == monsterId) {
			Door door = GameMap[it.Value]->GetDoor(d);

			if (door.connectedShelter == 0 || door.status == Close || GameMap[door.connectedShelter]->GetMonsterId() != 0) {
				UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
				return false;
			}
			UE_LOG(LogTemp, Warning, TEXT("monster %d moving from room %d..."),it.Key, it.Value);
			GameMap[it.Value]->DeleteMonster();
			Shelters[it.Key] = door.connectedShelter;
			GameMap[it.Value]->InsertMonster(monsterId);

			break;
		}
	}
	return true;
}

// monster's effect on shelters
void AShelterControl::AffectShelters(int monsterId, int status) {
	if (status == 1) {
		if (Monsters[monsterId]->IsRadioactive()) {
			for (const TPair<int32, int32>& it : Shelters) {
				if (it.Key == monsterId) {
					// radiate rooms in all directions
					Door door = GameMap[it.Value]->GetDoor(Left);
					if (door.connectedShelter == 0 || door.status == Close || GameMap[door.connectedShelter]->GetMonsterId() != 0) {
						UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
					}
					else {
						GameMap[door.connectedShelter]->Radiated();
					}
					door = GameMap[it.Value]->GetDoor(Right);

					if (door.connectedShelter == 0 || door.status == Close || GameMap[door.connectedShelter]->GetMonsterId() != 0) {
						UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
					}
					else {
						GameMap[door.connectedShelter]->Radiated();
					}
					door = GameMap[it.Value]->GetDoor(Up);

					if (door.connectedShelter == 0 || door.status == Close || GameMap[door.connectedShelter]->GetMonsterId() != 0) {
						UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
					}
					else {
						GameMap[door.connectedShelter]->Radiated();
					}
					door = GameMap[it.Value]->GetDoor(Down);

					if (door.connectedShelter == 0 || door.status == Close || GameMap[door.connectedShelter]->GetMonsterId() != 0) {
						UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
					}
					else {
						GameMap[door.connectedShelter]->Radiated();
					}

					break;
				}
			}
		}
	}
}

UMonster* AShelterControl::FindMonsterById(const unsigned int id) {

	for (const TPair<int32, UMonster*>& it : Monsters) {
		if (it.Key == id) {
			UE_LOG(LogTemp, Warning, TEXT("Hello pretty monster"));
			UMonster* m = it.Value;

			return m;
		}
	}
	return NULL;
}


