// Unreal Engine includes
#include "Items/Item.h"
#include "Components/CapsuleComponent.h"
#include "Characters/BaseCharacter.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up components
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	// Set up collision for the mesh
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events for the capsule component
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnCapsuleOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AItem::OnCapsuleEndOverlap);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Getter for item definition
UItemDef* AItem::GetItemDef() const
{
	return ItemDef;
}

// On Capsule Overlap Function
void AItem::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a character and set the overlapping item
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		BaseCharacter->SetOverlappingItem(this);
		ItemState = EItemState::EIS_Overlapping;
	}
}

// On Capsule End Overlap Function
void AItem::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlapping actor is a character and clear the overlapping item
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		BaseCharacter->SetOverlappingItem(nullptr);
		ItemState = EItemState::EIS_None;
	}
}
