// Unreal Engine includes
#include "Inventory/InventoryDB.h"
#include "Inventory/InventoryComp.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

// Open the database, creating it if it doesn't exist, and ensure the inventory table is set up
bool FInventoryDB::OpenDatabase()
{
	// Determine the path for the inventory database
	FString DBPath = FPaths::ProjectSavedDir() / TEXT("Inventory.db");

	// Attempt to open the database
	if (!Database.Open(*DBPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open inventory database."));
		return false;
	}

	// Ensure the inventory table exists
	if (!CreateInventoryTable())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create inventory."));
		Database.Close();
		return false;
	}

	return true;
}

// Close the database 
void FInventoryDB::CloseDatabase()
{
	Database.Close();
}

// Save the inventory slots to the database, replacing any existing data
bool FInventoryDB::SaveInventory(const TArray<FInventorySlot>& InventorySlots)
{
	if (!OpenDatabase())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open inventory database for saving."));
		return false;
	}

	const bool bCleared = Database.Execute(TEXT("DELETE FROM Inventory"));

	// Check if the existing inventory data was successfully cleared before inserting new data
	if (!bCleared)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to clear existing inventory data."));
		CloseDatabase();
		return false;
	}

	// Insert each inventory slot into the database
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			continue;
		}

		// Convert the item ID to a string
		const FString ItemIDString = Slot.ItemDef ? Slot.ItemDef->ItemID.ToString() : TEXT("None");

		// Construct the SQL insert query for this inventory slot
		const FString InsertQuery = FString::Printf(
			TEXT("INSERT INTO Inventory (ItemID, ItemQuantity) VALUES ('%s', %d)"),
			*ItemIDString,
			Slot.ItemQuantity
		);

		// Execute the insert query for this inventory slot
		if (!Database.Execute(*InsertQuery))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to insert inventory slot: %s"), *ItemIDString);
			CloseDatabase();
			return false;
		}
	}

	CloseDatabase();
	return true;

}

// Load the inventory slots from the database
TArray<FInventorySlot> FInventoryDB::LoadInventory()
{
	TArray<FInventorySlot> LoadedInventory;

	if (!OpenDatabase())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open inventory database for loading."));
		return LoadedInventory;
	}

	// Query the database for all inventory slots
	FSQLitePreparedStatement ResultSet = Database.PrepareStatement(TEXT("SELECT ItemID, ItemQuantity FROM Inventory"));

	if (ResultSet.IsValid())
	{
		while (ResultSet.Step() == ESQLitePreparedStatementStepResult::Row)
		{
			FString ItemIDString;
			int32 ItemQuantity = 0;

			ResultSet.GetColumnValueByName(TEXT("ItemID"), ItemIDString);
			ResultSet.GetColumnValueByName(TEXT("ItemQuantity"), ItemQuantity);

			FName ItemID = FName(*ItemIDString);
			UItemDef* ItemDef = FindItemDefByID(ItemID);

			if (!ItemDef)
			{
				UE_LOG(LogTemp, Warning, TEXT("Item definition not found for ItemID: %s"), *ItemIDString);
				continue;
			}

			FInventorySlot Slot;
			Slot.ItemDef = ItemDef;
			Slot.ItemQuantity = ItemQuantity;
			LoadedInventory.Add(Slot);
		}
		ResultSet.Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to execute inventory load query."));
	}


	CloseDatabase();
	return LoadedInventory;
}

bool FInventoryDB::CreateInventoryTable()
{
	const TCHAR* CreateTableQuery = (
		TEXT("CREATE TABLE IF NOT EXISTS Inventory (")
		TEXT("ID INTEGER PRIMARY KEY AUTOINCREMENT, ")
		TEXT("ItemID TEXT NOT NULL, ")
		TEXT("ItemQuantity INTEGER NOT NULL)")
		);

	const bool bTableCreated = Database.Execute(CreateTableQuery);

	if (!bTableCreated)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create inventory table."));
	}

	return bTableCreated;
}

// Find Item Definition by ItemID
UItemDef* FInventoryDB::FindItemDefByID(const FName& ItemID)
{
	// Use the Asset Registry to find all assets of type UItemDef
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByClass(UItemDef::StaticClass()->GetClassPathName(), AssetDataList);

	// Iterate through the asset data to find the matching item definition by ItemID
	for (const FAssetData& AssetData : AssetDataList)
	{
		UItemDef* ItemDef = Cast<UItemDef>(AssetData.GetAsset());
		if (ItemDef && ItemDef->ItemID == ItemID)
		{
			return ItemDef;
		}
	}

	return nullptr;
}