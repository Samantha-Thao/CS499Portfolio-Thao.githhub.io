// Copyright 2026 Sam Thao. All Rights Reserved.

#pragma once

// Unreal Engine includes
#include "CoreMinimal.h"
#include "Inventory/ItemDef.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

// Forward Declarations
class UCapsuleComponent;

UCLASS()
class TWOX_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItem();

	virtual void Tick(float DeltaTime) override;

	// Get the item definition
	UFUNCTION(BlueprintCallable, Category = "Item")	
	UItemDef* GetItemDef() const;

protected:
	
	virtual void BeginPlay() override;

	/* Overlap events for the capsule component */
	UFUNCTION()
	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Mesh component for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* ItemMesh;

	// Item Definition
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UItemDef* ItemDef;

	// Item State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EItemState ItemState = EItemState::EIS_None;

	// Capsule component for detecting overlaps with player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UCapsuleComponent* Capsule;

};
