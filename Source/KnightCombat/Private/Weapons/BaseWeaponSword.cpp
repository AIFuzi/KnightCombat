#include "Weapons/BaseWeaponSword.h"

#include "Components/BoxComponent.h"
#include "Characters/BaseCharacter.h"

ABaseWeaponSword::ABaseWeaponSword()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(WeaponMesh);
}
