#pragma once

// Unreal Engine includes
#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "ItemDef.h"
#include "InventoryDB.h"
#include "Components/ActorComponent.h"
#include "InventoryComp.generated.h"

// Forward Declarations
class UItemDef;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UInventoryComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventoryComp();

	// Delegate for inventory updates
	FInventoryDB InventoryDB;

	// Inventory functions
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull() const;

	// Add or delete items from inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemDef* ItemDef, int32 ItemQuantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DeleteItem(UItemDef* ItemDef, int32 ItemQuantity);
	
	// Check if there's space for a specific item and quantity
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasSpaceForItem(UItemDef* ItemDef, int32 ItemQuantity) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindEmptySlotIndex() const;

	// Getter for inventory properties
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventorySlot>& GetInventorySlots() const;

	// Get Item Quantity in Inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemQuantity(UItemDef* ItemDef) const;

protected:
	
	virtual void BeginPlay() override;

	// Inventory properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySize = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxHotbarSize = 6;

	// Inventory slots for main inventory and hotbar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> HotbarSlots;
	
};
