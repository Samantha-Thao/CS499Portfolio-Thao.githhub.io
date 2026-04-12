#pragma once

// Unreal Engine includes
#include "CoreMinimal.h"
#include "SQLiteDatabase.h"
#include "InventorySlot.h"

// Forward Declarations
class UItemDef;

class GAME_API FInventoryDB
{

public:
	
	// Open and close database
	bool OpenDatabase();
	void CloseDatabase();

	// Save inventory to database
	bool SaveInventory(const TArray<FInventorySlot>& InventorySlots);

	// Load inventory from database
	TArray<FInventorySlot> LoadInventory();

private:

	// SQLite database instance
	FSQLiteDatabase Database;

	// Create inventory table if it doesn't exist
	bool CreateInventoryTable();

	// Helper function to find item definition by ID
	UItemDef* FindItemDefByID(const FName& ItemID);
};