#include "Weapons/BaseWeaponSword.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Characters/BaseCharacter.h"

ABaseWeaponSword::ABaseWeaponSword()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(WeaponMesh);
}

void ABaseWeaponSword::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeaponSword, WeaponOwner);
}

void ABaseWeaponSword::OnRep_WeaponOwner()
{
	if(WeaponOwner)
	{
		SetInstigator(WeaponOwner);
		SetOwner(WeaponOwner);
	}
	else
	{
		SetInstigator(nullptr);
		SetOwner(nullptr);
	}
}
