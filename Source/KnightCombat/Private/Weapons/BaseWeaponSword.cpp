#include "Weapons/BaseWeaponSword.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ABaseWeaponSword::ABaseWeaponSword()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(WeaponMesh);
}
