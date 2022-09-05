#include "Components/WeaponCombatComponent.h"

#include "Characters/BaseCharacter.h"
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
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(WeaponClass)
		{
			const FVector SpawnLoc(0.f, 0.f, 0.f);
			const FRotator SpawnRot(0.f, 0.f, 0.f);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if(ABaseWeaponSword* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeaponSword>(WeaponClass, SpawnLoc, SpawnRot, SpawnParameters))
			{
				if(ABaseCharacter* WeaponOwner = Cast<ABaseCharacter>(GetOwner()))
				{
					SpawnedWeapon->WeaponOwner = WeaponOwner;
					SpawnedWeapon->OnRep_WeaponOwner();

					if(USceneComponent* OwnerMesh = Cast<USceneComponent>(WeaponOwner->GetMesh()))
						SpawnedWeapon->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "SK_Sword");
				}
			}
		}
	}

	
}

bool UWeaponCombatComponent::Server_CreateWeaponSword_Validate(TSubclassOf<ABaseWeaponSword> WeaponClass) { return true; }
