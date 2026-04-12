// Copyright 2026 Sam Thao. All Rights Reserved.

#pragma once

// Unreal Engine includes
#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

// Enum for item state
UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_None UMETA(DisplayName = "None"),
	EIS_Overlapping UMETA(DisplayName = "Overlapping"),
};

// Enum for item type
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Consumable UMETA(DisplayName = "Consumable"),
	Material UMETA(DisplayName = "Material"),
	Misc UMETA(DisplayName = "Miscellaneous")
};