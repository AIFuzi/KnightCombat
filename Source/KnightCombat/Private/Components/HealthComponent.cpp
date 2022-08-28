#include "Components/HealthComponent.h"
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
	CurrentHealth = CurrentHealth - Damage;
	CurrentHealth = FMath::Max(CurrentHealth, 0.f);

	if(CurrentHealth <= 0.f) OnHealthEnded.Broadcast();
}

bool UHealthComponent::Server_GetDamage_Validate(float Damage) { return true; }
