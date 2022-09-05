#include "Characters/BaseCharacter.h"

#include "Components/HealthComponent.h"
#include "Components/WeaponCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	if(HealthComponent) HealthComponent->SetIsReplicated(true);

	WeaponCombatComponent = CreateDefaultSubobject<UWeaponCombatComponent>(TEXT("WeaponCombatComponent"));
	if(WeaponCombatComponent) HealthComponent->SetIsReplicated(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
