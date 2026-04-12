#pragma once

// Unreal Engine includes
#include "CoreMinimal.h"
#include "InventorySlot.generated.h"

// Forward Declarations
class UItemDef;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

public:

	// The item definition for this slot, nullptr if the slot is empty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UItemDef* ItemDef = nullptr;

	// The quantity of the item in this slot, 0 if the slot is empty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 ItemQuantity = 0;

	// Helper function to check if the slot is empty  
	bool IsEmpty() const { return ItemDef == nullptr || ItemQuantity <= 0; }

	// Helper function to clear the slot
	void Clear()
	{
		ItemDef = nullptr;
		ItemQuantity = 0;
	}

};

