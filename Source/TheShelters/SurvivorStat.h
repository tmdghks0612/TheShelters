#pragma once

#include <algorithm>
#include <cmath>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SurvivorStat.generated.h"

UCLASS()
class THESHELTERS_API USurvivorStat : public UObject
{
	GENERATED_BODY()
public:
	// Constructors and Initializers
	USurvivorStat();
	~USurvivorStat();
	void InitSurvivorStat(double _food, double _water, double _mental, double _progress, double _electricity);

	// Getters
	const double &Food() const;
	const double &Water() const;
	const double &Mental() const;
	const double &Progress() const;
	const double &Electricity() const;

	const double &Food(const double diff);
	const double &Water(const double diff);
	const double &Mental(const double diff);
	const double &Progress(const double diff);
	const double &Electricity(const double diff);

	// Methods for playing game
	void EndTurn();

private:
	double max = 100;
	double food;
	double water;
	double mental;
	double progress;
	double electricity;

	int electricityUsage;

	// Private methods for playing game
	void Consume();
	const double MentalMultiplier() const;
};
