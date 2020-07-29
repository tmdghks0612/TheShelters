#pragma once

#include <algorithm>
#include <cmath>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SurvivorStat.generated.h"

/* << USurvivorStat : UObject >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitSurvivorStat
 * Property:
 * - Food
 * - Water
 * - Mental
 * - Progress
 * - Electricity
 * 
 * Description:
 * Food is for hunger stat and water is for thirst stat. Survivor can use
 * food resource and water resource to increase corresponding stats.
 * Mental stat is not shown to Survivor but it will trigger some events.
 * Electricty is 
 */
UCLASS()
class THESHELTERS_API USurvivorStat : public UObject
{
	GENERATED_BODY()
public:
	// Constructors and Initializers
	USurvivorStat();
	~USurvivorStat();
	void InitSurvivorStat();
	void InitSurvivorStat(int _food, int _water, int _elec, int _hunger, int _thirst, int _mental, int _progress);

	// Getters
	const int &Food() const;
	const int &Water() const;
	const int &Electricity() const;

	const int &Hunger() const;
	const int &Thirst() const;
	const double &Mental() const;

	const int &Progress() const;

	// Setters
	const int &Food(const int diff);
	const int &Water(const int diff);
	const int &Electricity(const int diff);

	const int &Hunger(const int diff);
	const int &Thirst(const int diff);
	const double &Mental(const double diff);

	const int &Progress(const int diff);

	// Methods for playing game
	void EndTurn();

private:
	int max = 100;
	int food;
	int water;
	int electricity;

	int electricityUsage;

	int hunger;
	int thirst;
	double mental;

	int progress;

	// Private methods for playing game
	void Consume();
	const double MentalMultiplier() const;
};
