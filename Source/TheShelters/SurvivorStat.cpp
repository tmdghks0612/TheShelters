#include "SurvivorStat.h"

#define StatSetter(x, y, z)       \
	{                             \
		(x) += (y);               \
		(x) = std::max(0, (x));   \
		(x) = std::min((x), (z)); \
	}

USurvivorStat::USurvivorStat() {}
USurvivorStat::~USurvivorStat() {}

void USurvivorStat::InitSurvivorStat() {}
void USurvivorStat::InitSurvivorStat(int _food, int _water, int _elec, int _hunger, int _thirst, int _mental, int _progress)
{
	this->food = _food;
	this->water = _water;
	this->electricity = _elec;

	this->hunger = _hunger;
	this->thirst = _thirst;
	this->mental = _mental;

	this->progress = _progress;
}

const int &USurvivorStat::Food() const
{
	return this->food;
}

const int &USurvivorStat::Water() const
{
	return this->water;
}

const int &USurvivorStat::Electricity() const
{
	return this->electricity;
}

const int &USurvivorStat::Hunger() const
{
	return this->hunger;
}

const int &USurvivorStat::Thirst() const
{
	return this->thirst;
}

const double &USurvivorStat::Mental() const
{
	return this->mental;
}

const int &USurvivorStat::Progress() const
{
	return this->progress;
}

const int &USurvivorStat::Food(const int diff)
{
	StatSetter(food, diff, max);
	return food;
}

const int &USurvivorStat::Water(const int diff)
{
	StatSetter(water, diff, max);
	return water;
}

const int &USurvivorStat::Electricity(const int diff)
{
	StatSetter(electricity, diff, max);
	return electricity;
}

const int &USurvivorStat::Hunger(const int diff)
{
	StatSetter(hunger, diff, max);
	return hunger;
}

const int &USurvivorStat::Hunger(const int diff)
{
	StatSetter(thirst, diff, max);
	return thirst;
}

const double &USurvivorStat::Mental(const double diff)
{
	StatSetter(mental, diff, max);
	return mental;
}

const int &USurvivorStat::Progress(const int diff)
{
	StatSetter(progress, diff, max);
	return progress;
}

void USurvivorStat::Consume()
{
	int hungerPerTime = 1;
	int thirstMultiplier = 2;
	Food(-hungerPerTime);
	Water(-hungerPerTime * thirstMultiplier);
}

const double USurvivorStat::MentalMultiplier() const
{
	double hungerMultiplier, thirstMultiplier;
	if (this->hunger < 30)
	{
		hungerMultiplier = (std::log(this->hunger + 1) / std::log(31.0)) - 2;
	}
	else if (this->hunger > 60)
	{
		hungerMultiplier = std::log(this->hunger - 59) / std::log(41) + 1;
	}
	else
	{
		hungerMultiplier = (this->hunger / 15) - 3;
	}

	if (this->thirst < 30)
	{
		thirstMultiplier = (std::log(this->thirst + 1) / std::log(31.0)) - 2;
	}
	else if (this->hunger > 60)
	{
		thirstMultiplier = std::log(this->thirst - 59) / std::log(41) + 1;
	}
	else
	{
		thirstMultiplier = (this->thirst / 15) - 3;
	}

	return round((hungerMultiplier + thirstMultiplier) * 100.0) / 100.0;
}

void USurvivorStat::EndTurn()
{
	double mentalMultiplier = MentalMultiplier();
	// Consume food and water
	Consume();

	// Change mental
	double mentalDiff = 1 * mentalMultiplier;
	Mental(mentalDiff);

	// Consume electricity
	Electricity(-electricityUsage);
}