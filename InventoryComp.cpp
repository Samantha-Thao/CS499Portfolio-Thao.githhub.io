// Copyright 2026 Sam Thao. All Rights Reserved.

// Unreal Engine includes
#include "Inventory/InventoryComp.h"

UInventoryComp::UInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();

	InitializeInventory();
}

// Initialize inventory with empty slots
void UInventoryComp::InitializeInventory()
{
	InventorySlots.Empty();
	InventorySlots.SetNum(MaxInventorySize);

	HotbarSlots.Empty();
	HotbarSlots.SetNum(MaxHotbarSize);
}

// Check if inventory is full
bool UInventoryComp::IsInventoryFull() const
{
	return FindEmptySlotIndex() == INDEX_NONE; 
}

// Add item to inventory, returns true if successful
bool UInventoryComp::AddItem(UItemDef* ItemDef, int32 ItemQuantity)
{
	// Basic validation
	if (!ItemDef || ItemQuantity <= 0)
	{
		return false;
	}

	// Check if there's space for the item and quantity in inventory
	if (!HasSpaceForItem(ItemDef, ItemQuantity))
	{
		return false;
	}

	int32 RemainingQuantity = ItemQuantity;

	// First try to add to existing stacks if the item is stackable
	if (ItemDef->bStackable)
	{
		for (FInventorySlot& Slot : InventorySlots)
		{
			// Check if this slot has the same item and has space for more
			if (Slot.ItemDef == ItemDef && Slot.ItemQuantity < ItemDef->MaxStackSize)
			{
				const int32 SpaceInStack = ItemDef->MaxStackSize - Slot.ItemQuantity;
				const int32 QuantityToAdd = FMath::Min(SpaceInStack, RemainingQuantity);
				Slot.ItemQuantity += QuantityToAdd;
				RemainingQuantity -= QuantityToAdd;

				if (RemainingQuantity <= 0)
				{
					return true;
				}
			}
		}
	}

	// If there are still items to add, try to add them to empty slots
	while (RemainingQuantity > 0)
	{
		const int32 EmptySlotIndex = FindEmptySlotIndex();

		if (EmptySlotIndex == INDEX_NONE)
		{
			return false;
		}

		FInventorySlot& EmptySlot = InventorySlots[EmptySlotIndex];
		EmptySlot.ItemDef = ItemDef;

		// If the item is stackable, add as many as possible to this slot, otherwise just add one
		if (ItemDef->bStackable)
		{
			const int32 QuantityToAdd = FMath::Min(ItemDef->MaxStackSize, RemainingQuantity);
			EmptySlot.ItemQuantity = QuantityToAdd;
			RemainingQuantity -= QuantityToAdd;
		}
		else
		{
			EmptySlot.ItemQuantity = 1;
			RemainingQuantity -= 1;
		}
	}

	return true;
}

// Remove item from inventory, returns true if successful
bool UInventoryComp::DeleteItem(UItemDef* ItemDef, int32 ItemQuantity)
{
	// Basic validation
	if (!ItemDef || ItemQuantity <= 0)
	{
		return false;
	}

	// Check if we have enough of the item to delete
	if (GetItemQuantity(ItemDef) < ItemQuantity)
	{
		return false;
	}

	int32 RemainingQuantity = ItemQuantity;

	// First try to remove from existing stacks
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemDef == ItemDef)
		{
			// Remove as many items as possible from this slot
			const int32 QuantityToRemove = FMath::Min(Slot.ItemQuantity, RemainingQuantity);
			Slot.ItemQuantity -= QuantityToRemove;
			RemainingQuantity -= QuantityToRemove;

			if (Slot.ItemQuantity <= 0)
			{
				Slot.Clear();
			}

			if (RemainingQuantity <= 0)
			{
				return true;
			}
		}
	}

	return false;

}

// Check if there's space for a specific item and quantity in inventory
bool UInventoryComp::HasSpaceForItem(UItemDef* ItemDef, int32 ItemQuantity) const
{
	// Basic validation
	if (!ItemDef || ItemQuantity <= 0)
	{
		return false;
	}

	int32 RemainingQuantity = ItemQuantity;

	// First check existing stacks for stackable items
	if (ItemDef->bStackable)
	{
		for (const FInventorySlot& Slot : InventorySlots)
		{
			// Check if this slot has the same item and has space for more
			if (Slot.ItemDef == ItemDef && Slot.ItemQuantity < ItemDef->MaxStackSize)
			{
				const int32 SpaceInStack = ItemDef->MaxStackSize - Slot.ItemQuantity;
				RemainingQuantity -= SpaceInStack;

				if (RemainingQuantity <= 0)
				{
					return true;
				}
			}
		}
	}

	// Then check for empty slots
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			// If the item is stackable, this slot can hold a full stack, otherwise it can hold one item
			if (ItemDef->bStackable)
			{
				const int32 SpaceInStack = ItemDef->MaxStackSize;
				RemainingQuantity -= SpaceInStack;
			}
			else
			{
				RemainingQuantity -= 1;
			}
			if (RemainingQuantity <= 0)
			{
				return true;
			}
		}
	}

	return false;
}

// Find an empty slot index in inventory
int32 UInventoryComp::FindEmptySlotIndex() const
{
	// Look for an empty slot
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].IsEmpty())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

const TArray<FInventorySlot>& UInventoryComp::GetInventorySlots() const
{
	return InventorySlots;
}

// Get total quantity of a specific item in inventory
int32 UInventoryComp::GetItemQuantity(UItemDef* ItemDef) const
{
	// Basic validation
	if (!ItemDef)
	{
		return 0;
	}
	int32 TotalQuantity = 0;

	// Sum up the quantity of the specified item across all inventory slots
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemDef == ItemDef)
		{
			TotalQuantity += Slot.ItemQuantity;
		}
	}

	return TotalQuantity;
}