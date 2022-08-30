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
