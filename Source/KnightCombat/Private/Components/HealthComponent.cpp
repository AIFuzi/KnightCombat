#include "Components/HealthComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, CurrentStamina);
}

void UHealthComponent::GetDamage(float Damage)
{
	Server_GetDamage(Damage);
}

void UHealthComponent::Server_GetDamage_Implementation(float Damage)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		StopRegenHealth();
		
		CurrentHealth = CurrentHealth - Damage;
		CurrentHealth = FMath::Max(CurrentHealth, 0.f);

		if(CurrentHealth <= 0.f) OnHealthEnded.Broadcast();
		else StartRegenHealth();
	}
}

bool UHealthComponent::Server_GetDamage_Validate(float Damage) { return true; }

void UHealthComponent::StartRegenHealth()
{
	GetWorld()->GetTimerManager().SetTimer(RegenHealthTimer, this, &UHealthComponent::Server_RegenHealth, RegenHealthRate, true, RegenHealthStartDelay);
}

void UHealthComponent::StopRegenHealth()
{
	GetWorld()->GetTimerManager().ClearTimer(RegenHealthTimer);
}

void UHealthComponent::Server_RegenHealth_Implementation()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		CurrentHealth = CurrentHealth + 1.f;
		CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);
		
		if(CurrentHealth >= MaxHealth) StopRegenHealth();
	}
}

bool UHealthComponent::Server_RegenHealth_Validate() { return true; }

void UHealthComponent::StartUseStamina()
{
//	GetWorld()->GetTimerManager().SetTimer()
}

void UHealthComponent::StopUseStamina()
{
	
}

void UHealthComponent::UseStaminaValue(float Value)
{
	Server_UseStaminaValue(Value);
}

void UHealthComponent::Server_UseStaminaValue_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(CurrentStamina >= Value)
		{
			StopRegenStamina();
			
			CurrentStamina = CurrentStamina - Value;
			CurrentStamina = FMath::Max(CurrentStamina, 0.f);

			StartRegenStamina();
		}
	}
}

bool UHealthComponent::Server_UseStaminaValue_Validate(float Value) { return true; }

void UHealthComponent::StartRegenStamina()
{
	GetWorld()->GetTimerManager().SetTimer(RegenStaminaTimer, this, &UHealthComponent::Server_RegenStamina, RegenStaminaRate, true, RegenStaminaStartDelay);
}

void UHealthComponent::StopRegenStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(RegenStaminaTimer);
}

void UHealthComponent::Server_RegenStamina_Implementation()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		
	}
}

bool UHealthComponent::Server_RegenStamina_Validate() { return true; }

void UHealthComponent::Server_UseStamina_Implementation()
{
	
}

bool UHealthComponent::Server_UseStamina_Validate() { return true; }
