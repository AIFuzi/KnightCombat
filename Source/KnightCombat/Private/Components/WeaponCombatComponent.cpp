#include "Components/WeaponCombatComponent.h"

#include "Characters/BaseCharacter.h"
#include "Engine/World.h"
#include "Weapons/BaseWeaponSword.h"

UWeaponCombatComponent::UWeaponCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentWeaponSword)
	{
		for(int i = 0; i <= TraceValue; i++)
		{
			FVector SocketStartLoc = CurrentWeaponSword->WeaponMesh->GetSocketLocation("SK_AttackTraceStart");
			FVector SocketEndLoc = CurrentWeaponSword->WeaponMesh->GetSocketLocation("SK_AttackTraceEnd");
			float LerpAlpha = static_cast<float>(i) / static_cast<float>(TraceValue);
			FVector LerpVec = FMath::Lerp(SocketStartLoc, SocketEndLoc, LerpAlpha);

			if(DrawDebugInfo)
			{
				DrawDebugSphere(GetWorld(), LerpVec, 5.f, 12, FColor::Green, false, 0.f, 0, 0.f);
				DrawDebugString(GetWorld(), LerpVec, FString::SanitizeFloat(LerpAlpha), nullptr, FColor::Red, 0.f, false, 1.f);
				DrawDebugLine(GetWorld(), SocketStartLoc, SocketEndLoc, FColor::Red, false, 0.f, 0, 1.f);
			}
		}
	}
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
					CurrentWeaponSword = SpawnedWeapon;
					
					CurrentWeaponSword->WeaponOwner = WeaponOwner;
					CurrentWeaponSword->OnRep_WeaponOwner();

					if(USceneComponent* OwnerMesh = Cast<USceneComponent>(WeaponOwner->GetMesh()))
						CurrentWeaponSword->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "SK_Sword");
				}
			}
		}
	}
}

bool UWeaponCombatComponent::Server_CreateWeaponSword_Validate(TSubclassOf<ABaseWeaponSword> WeaponClass) { return true; }
