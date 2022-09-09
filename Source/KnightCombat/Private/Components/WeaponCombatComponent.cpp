#include "Components/WeaponCombatComponent.h"

#include "Characters/BaseCharacter.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/BaseWeaponSword.h"
#include "Net/UnrealNetwork.h"

UWeaponCombatComponent::UWeaponCombatComponent()
{
	
}

void UWeaponCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponCombatComponent, CurrentWeaponSword);
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
						CurrentWeaponSword->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketWeaponSpawnName);
				}
			}
		}
	}
}

bool UWeaponCombatComponent::Server_CreateWeaponSword_Validate(TSubclassOf<ABaseWeaponSword> WeaponClass) { return true; }

void UWeaponCombatComponent::StartTraceSwordAttack()
{
	if(CurrentWeaponSword)
	{
		for(int i = 0; i <= TraceValue; i++)
		{
			FVector SocketStartLoc = CurrentWeaponSword->WeaponMesh->GetSocketLocation(SocketStartName);
			FVector SocketEndLoc = CurrentWeaponSword->WeaponMesh->GetSocketLocation(SocketEndName);
			float LerpAlpha = static_cast<float>(i) / static_cast<float>(TraceValue);
			FVector LerpVec = FMath::Lerp(SocketStartLoc, SocketEndLoc, LerpAlpha);

			LastTraceHitLoc.Add(LerpVec);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(SwordAttackTimer, this, &UWeaponCombatComponent::Server_SwordAttackTrace, SwordTracingRate, true, -1.f);
}

void UWeaponCombatComponent::StopTraceSwordAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(SwordAttackTimer);

	LastTraceHitLoc.Empty();
	HitActors.Empty();
}

void UWeaponCombatComponent::Server_SwordAttackTrace_Implementation()
{
	Multicast_SwordAttackTrace();
}

bool UWeaponCombatComponent::Server_SwordAttackTrace_Validate() { return true; }

void UWeaponCombatComponent::Multicast_DrawDebugInfo_Implementation(FVector LerpVecVal, FVector EndLoc, float LerpVal)
{
	DrawDebugSphere(GetWorld(), LerpVecVal, 5.f, 12, FColor::Green, false, SwordTracingRate, 0, 0.f);
	DrawDebugString(GetWorld(), LerpVecVal, FString::SanitizeFloat(LerpVal), nullptr, FColor::Red, SwordTracingRate, false, 1.f);
	DrawDebugLine(GetWorld(), LerpVecVal, EndLoc, FColor::Red, false, 1.f, 0, 1.f);
}

bool UWeaponCombatComponent::Multicast_DrawDebugInfo_Validate(FVector LerpVecVal, FVector EndLoc, float LerpVal) { return true; }

void UWeaponCombatComponent::Multicast_SwordAttackTrace_Implementation()
{
	if(LastTraceHitLoc.Num() > 0)
	{
		bool bCanBlockAttack = false;
		
		for(int i = 0; i <= TraceValue; i++)
		{
			FVector SocketStartLoc = CurrentWeaponSword->WeaponMesh->GetSocketLocation(SocketStartName);
			FVector SocketEndLoc = CurrentWeaponSword->WeaponMesh->GetSocketLocation(SocketEndName);
			float LerpAlpha = static_cast<float>(i) / static_cast<float>(TraceValue);
			FVector LerpVec = FMath::Lerp(SocketStartLoc, SocketEndLoc, LerpAlpha);

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(GetOwner());
			QueryParams.AddIgnoredActor(CurrentWeaponSword);
			
			if(GetWorld()->LineTraceSingleByChannel(HitResult, LerpVec, LastTraceHitLoc[i], ECC_GameTraceChannel1, QueryParams))
			{
				if(Cast<ABaseCharacter>(HitResult.GetActor()))
				{
					if(!HitActors.Contains(HitResult.GetActor()))
					{
						DrawDebugBox(GetWorld(), HitResult.Location, FVector(5.f, 5.f, 5.f), FColor::Cyan, false, 5.f, 0, 0.3f);
					
						TSubclassOf<UDamageType> DamageTypeClass;
						DamageTypeClass = UDamageType::StaticClass();
						UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10.f, GetOwner()->GetInstigatorController(), GetOwner(), DamageTypeClass);

						HitActors.AddUnique(HitResult.GetActor());
					}	
				}
				else
				{
					bCanBlockAttack = true;
					break;
				}
			}
				
			if(DrawDebugInfo) Multicast_DrawDebugInfo(LerpVec,  LastTraceHitLoc[i], LerpAlpha);
			LastTraceHitLoc[i] = LerpVec;
		}

		if(bCanBlockAttack)
		{
			if(ABaseCharacter* CharOwner = Cast<ABaseCharacter>(GetOwner()))
				CharOwner->PlayAnimMontage(BlockAttackImpactAnimMontage);
			StopTraceSwordAttack();
		}
	}
}

bool UWeaponCombatComponent::Multicast_SwordAttackTrace_Validate() { return true; }

ABaseWeaponSword* UWeaponCombatComponent::GetCurrentWeaponSword() const
{
	return CurrentWeaponSword;
}

void UWeaponCombatComponent::AttackSword()
{
	Server_AttackSword();
}

void UWeaponCombatComponent::Server_AttackSword_Implementation()
{
	Multicast_AttackSword();	
}

void UWeaponCombatComponent::Multicast_AttackSword_Implementation()
{
	if(!bIsAttackCooldown)
	{
		if(ABaseCharacter* CharOwner = Cast<ABaseCharacter>(GetOwner()))
        {
			CharOwner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
        	bIsAttackCooldown = true;
			
        	const float CooldownRate = CharOwner->PlayAnimMontage(AttackAnimMontage);
        	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UWeaponCombatComponent::ClearCooldown, CooldownRate, false);
        }
	}
}

void UWeaponCombatComponent::ClearCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);

	if(const ABaseCharacter* CharOwner = Cast<ABaseCharacter>(GetOwner())) CharOwner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bIsAttackCooldown = false;
}