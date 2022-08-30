#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEnded);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KNIGHTCOMBAT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHealthComponent();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float RegenHealthRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float RegenHealthStartDelay;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FHealthEnded OnHealthEnded;

	UFUNCTION(BlueprintCallable, Category="Health")
	void GetDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category="Health")
	void StartRegenHealth();

	UFUNCTION(BlueprintCallable, Category="Health")
	void StopRegenHealth();

private:

	FTimerHandle RegenHealthTimer;
	FTimerHandle RegenStaminaTimer;

	UFUNCTION(Unreliable, Server, WithValidation, Category="Health")
	void Server_GetDamage(float Damage);

	UFUNCTION(Unreliable, Server, WithValidation, Category="Health")
	void Server_RegenHealth();
		
};
