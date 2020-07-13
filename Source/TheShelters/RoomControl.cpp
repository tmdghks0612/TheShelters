// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomControl.h"

// Sets default values
ARoomControl::ARoomControl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

//	InitGame(3, 3);

}

// Called when the game starts or when spawned
void ARoomControl::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomControl::EndTurn() {
	InitGame(3, 3);

	for (int i = 0; i < 9; ++i) {
		UE_LOG(LogTemp, Warning, TEXT("Room no in endturn : %d"), GameMap[i]->GetRoomNo());
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime!"));
	UMonster* NewMonster = NewObject<UMonster>();
	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime1!"));

	NewMonster->InitMonster(DefaultMonster, 1);
	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime2!"));
	InsertMonster(NewMonster, 1, 1);
	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime3!"));
	UMonster* monster = FindMonsterById(1);
	UE_LOG(LogTemp, Warning, TEXT("Printing to Log runtime4!"));
	URoom* room = FindRoomByLocation(1, 1);
	UE_LOG(LogTemp, Warning, TEXT("Room no : %d"), room->GetRoomNo());
	Door* d = room->GetDoor(Left);
	//Location l = room->GetDoor(Left)->ConnectedRoom->GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Room coord : %d, %d"), l.x, l.y);
	/*MoveMonster(monster, Left);

	URoom* room = FindRoomByLocation(1, 0);
	UE_LOG(LogTemp, Warning, TEXT("Monster found id : %d"), 13);
	UE_LOG(LogTemp, Warning, TEXT("Monster found id : %d"), room->GetMonster()->MonsterId());
	*/
}

void ARoomControl::InitGame(const unsigned int m, const unsigned int n) {
	MaxHeight = m;
	MaxWidth = n;

	int size = MaxHeight * MaxWidth;

	// Initializing Map
	//GameMap = new URoom*[m*n];

	for (int i = 0; i < size; i++) {
		int x = i / MaxWidth;
		int y = i % MaxWidth;

		URoom *NewRoom = NewObject<URoom>();
		NewRoom->InitRoom(i);
		NewRoom->SetLocation(x, y);
		GameMap[i] = NewRoom;
		UE_LOG(LogTemp, Warning, TEXT("loop : %d"), i);
		UE_LOG(LogTemp, Warning, TEXT("Room no : %d"), NewRoom->GetRoomNo());
	}

	// Connect all rooms
	for (int i = 0; i < size; i++) {
		// A idx is MaxWidth * x + y
		int x = i / MaxWidth;
		int y = i % MaxWidth;

		if (x != 0) {
			GameMap[i]->SetDoor(Up, GameMap[MaxWidth * (x - 1) + y], Open);
		}
		if (x != (MaxHeight - 1)) {
			GameMap[i]->SetDoor(Down, GameMap[MaxWidth * (x + 1) + y], Open);
		}
		if (y != 0) {
			GameMap[i]->SetDoor(Left, GameMap[MaxWidth * x + (y - 1)], Open);
		}
		if (y != (MaxWidth - 1)) {
			GameMap[i]->SetDoor(Right, GameMap[MaxWidth * x + (y + 1)], Open);
		}
	}
}


URoom* ARoomControl::FindRoomByLocation(const unsigned int x, const unsigned int y) {
	if (x < 0 || x >= MaxHeight) {
		throw "You've exceeded map height";
	}
	if (y < 0 || y >= MaxWidth) {
		throw "You've exceeded map width";
	}

	int idx = MaxWidth * x + y;

	return GameMap[idx];
}

void ARoomControl::InsertMonster(UMonster* monster, URoom* room) {
	room->InsertMonster(monster);
	Monsters.push_back(std::make_pair(monster, room));
}

void ARoomControl::InsertMonster(UMonster* monster, const unsigned int x, const unsigned int y) {
	URoom* room = FindRoomByLocation(x, y);
	InsertMonster(monster, room);
}

void ARoomControl::DeleteMonster(URoom* room) {
	UMonster* monster = room->DeleteMonster();

	// FIXME: If there are two same monster id, it can go wrong...
	int targetId = monster->MonsterId();
	for (MonsterList::iterator it = Monsters.begin(); it != Monsters.end(); it++) {
		if (it->first->MonsterId() == targetId) {
			Monsters.erase(it);
		}
	}
	delete monster;
}

void ARoomControl::DeleteMonster(const unsigned int x, const unsigned int y) {
	URoom* room = FindRoomByLocation(x, y);
	DeleteMonster(room);
}

void ARoomControl::MoveMonster(UMonster* monster, Direction d) {
	int targetId = monster->MonsterId();
	MoveMonster(targetId, d);
}

void ARoomControl::MoveMonster(int monsterId, Direction d) {
	for (MonsterList::iterator it = Monsters.begin(); it != Monsters.end(); it++) {
		if (it->first->MonsterId() == monsterId) {
			const Door* door = it->second->GetDoor(d);

			if (door->ConnectedRoom == 0 || door->Status == Close) {
				throw "Cannot move";
			}

			UMonster* m = it->second->DeleteMonster();
			it->second = door->ConnectedRoom;
			it->second->InsertMonster(m);

			break;
		}
	}
}

UMonster* ARoomControl::FindMonsterById(const unsigned int id) {
	
	for (MonsterList::iterator it = Monsters.begin(); it != Monsters.end(); it++) {
		if (it->first->MonsterId() == id) {
			UE_LOG(LogTemp, Warning, TEXT("Hello pretty monster"));
			UMonster* m = it->first;

			return m;
		}
		
		break;
	}
	return 0;
}

/*ARoomControl::~ARoomControl() {
	int size = MaxHeight * MaxWidth;

	UMonster* m;
	while (!Monsters.empty()) {
		m = Monsters.back().first;
		Monsters.pop_back();
		delete m;
	}

	if (!GameMap) {
		for (int i = 0; i < size; i++) {
			delete GameMap[i];
		}
		delete[] GameMap;
	}
}*/