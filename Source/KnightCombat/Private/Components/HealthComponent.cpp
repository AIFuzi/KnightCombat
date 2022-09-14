#include "Components/HealthComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"

UHealthComponent::UHealthComponent()
{
	
}

void UHealthComponent::GetDamage(float Damage)
{
	if(!bIsInvisibleActivate)
	{
		StopRegenHealth();
		
		CurrentHealth = CurrentHealth - Damage;
		CurrentHealth = FMath::Max(CurrentHealth, 0.f);

		if(CurrentHealth <= 0.f) OnHealthEnded.Broadcast();
		else StartRegenHealth();
	}
}

void UHealthComponent::StartRegenHealth()
{
	GetWorld()->GetTimerManager().SetTimer(RegenHealthTimer, this, &UHealthComponent::RegenHealth, RegenHealthRate, true, RegenHealthStartDelay);
}

void UHealthComponent::StopRegenHealth()
{
	GetWorld()->GetTimerManager().ClearTimer(RegenHealthTimer);
}

void UHealthComponent::SetInvisibleMode(bool ActivateInvisible)
{
	Server_SetInvisibleMode(ActivateInvisible);
}

void UHealthComponent::Server_SetInvisibleMode_Implementation(bool ActivateInvisible)
{
	bIsInvisibleActivate = ActivateInvisible;
}

void UHealthComponent::RegenHealth()
{
	CurrentHealth++;
	CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);
		
	if(CurrentHealth >= MaxHealth) StopRegenHealth();
}


void UHealthComponent::StartUseStamina()
{
	if(CurrentStamina >= StaminaForSprint) GetWorld()->GetTimerManager().SetTimer(UseStaminaTimer, this, &UHealthComponent::Server_UseStamina, SprintStaminaRate, true, 0.f);
}

void UHealthComponent::StopUseStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(UseStaminaTimer);
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
		CurrentStamina++;
		CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);

		if(CurrentStamina >= MaxStamina) StopRegenStamina();
	}
}

bool UHealthComponent::Server_RegenStamina_Validate() { return true; }

void UHealthComponent::Server_UseStamina_Implementation()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		CurrentStamina--;
		CurrentStamina = FMath::Max(CurrentStamina, 0.f);

		if(CurrentStamina <= 0.f)
		{
			StopUseStamina();
			OnStaminaEnded.Broadcast();
		}
		else StartRegenStamina();
	}
}

bool UHealthComponent::Server_UseStamina_Validate() { return true; }

bool UHealthComponent::IsInvisibleModeActivate() const
{
	return bIsInvisibleActivate;
}