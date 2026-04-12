// Copyright 2026 Sam Thao. All Rights Reserved.

#pragma once

// Unreal Engine includes
#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Engine/Texture2D.h"
#include "Engine/DataAsset.h"
#include "ItemDef.generated.h"

UCLASS(BlueprintType)
class TWOX_API UItemDef : public UDataAsset
{
	GENERATED_BODY()

public:

	/// Item properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDesc = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bStackable = false;
};