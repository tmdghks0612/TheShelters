#include "PlayerStat.h"

UPlayerStat::UPlayerStat() {}
UPlayerStat::~UPlayerStat() {}

void UPlayerStat::InitPlayerStat(double _food, double _water, double _mental, double _progress, double _electricity)
{
	this->food = _food;
	this->water = _water;
	this->mental = _mental;
	this->progress = _progress;
	this->electricity = _electricity;
}

const double &UPlayerStat::Food() const
{
	return this->food;
}

const double &UPlayerStat::Water() const
{
	return this->water;
}

const double &UPlayerStat::Mental() const
{
	return this->mental;
}

const double &UPlayerStat::Progress() const
{
	return this->progress;
}

const double &UPlayerStat::Electricity() const
{
	return this->electricity;
}

const double &UPlayerStat::Food(const double diff)
{
	this->food += diff;
	this->food = std::max(0.0, this->food);
	this->food = std::min(this->food, this->max);
	return this->food;
}

const double &UPlayerStat::Water(const double diff)
{
	this->water += diff;
	this->water = std::max(0.0, this->water);
	this->water = std::min(this->water, this->max);
	return this->water;
}
const double &UPlayerStat::Mental(const double diff)
{
	this->mental += diff;
	this->mental = std::max(0.0, this->mental);
	this->mental = std::min(this->mental, this->max);
	return this->mental;
}
const double &UPlayerStat::Progress(const double diff)
{
	this->progress += diff;
	this->progress = std::max(0.0, this->progress);
	this->progress = std::min(this->progress, this->max);
	return this->progress;
}
const double &UPlayerStat::Electricity(const double diff)
{
	this->electricity += diff;
	this->electricity = std::max(0.0, this->electricity);
	this->electricity = std::min(this->electricity, this->max);
	return this->electricity;
}

void UPlayerStat::Consume()
{
	double foodPerTime = 1;
	double waterMultiplier = 2;
	Food(-foodPerTime);
	Water(-foodPerTime * waterMultiplier);
}

const double UPlayerStat::MentalMultiplier() const
{
	double foodMultiplier, waterMultiplier;
	if (this->food < 30)
	{
		foodMultiplier = (std::log(this->food + 1) / std::log(31.0)) - 2;
	}
	else if (this->food > 60)
	{
		foodMultiplier = std::log(this->food - 59) / std::log(41) + 1;
	}
	else
	{
		foodMultiplier = (this->food / 15) - 3;
	}

	if (this->water < 30)
	{
		waterMultiplier = (std::log(this->water + 1) / std::log(31.0)) - 2;
	}
	else if (this->food > 60)
	{
		waterMultiplier = std::log(this->water - 59) / std::log(41) + 1;
	}
	else
	{
		waterMultiplier = (this->water / 15) - 3;
	}

	return round((foodMultiplier + waterMultiplier) * 100.0) / 100.0;
}

void UPlayerStat::EndTurn()
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