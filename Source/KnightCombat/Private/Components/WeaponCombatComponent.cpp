#include "Components/WeaponCombatComponent.h"

#include "Engine/World.h"
#include "Weapons/BaseWeaponSword.h"

UWeaponCombatComponent::UWeaponCombatComponent()
{

}

void UWeaponCombatComponent::CreateWeaponSword(TSubclassOf<ABaseWeaponSword> WeaponClass)
{
	Server_CreateWeaponSword(WeaponClass);
}

void UWeaponCombatComponent::Server_CreateWeaponSword_Implementation(TSubclassOf<ABaseWeaponSword> WeaponClass)
{
	FVector SpawnLoc(0.f, 0.f, 0.f);
	FRotator SpawnRot(0.f, 0.f, 0.f);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	
}

bool UWeaponCombatComponent::Server_CreateWeaponSword_Validate(TSubclassOf<ABaseWeaponSword> WeaponClass) { return true; }
