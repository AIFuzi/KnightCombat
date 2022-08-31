#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaEnded);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float MaxStamina;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float RegenStaminaRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float SprintStaminaRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float RegenStaminaStartDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float StaminaForSprint;

	UPROPERTY(BlueprintAssignable, Category="Stamina")
	FHealthEnded OnStaminaEnded;

	UFUNCTION(BlueprintCallable, Category="Health")
	void GetDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category="Health")
	void StartRegenHealth();

	UFUNCTION(BlueprintCallable, Category="Health")
	void StopRegenHealth();

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void StartUseStamina();

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void StopUseStamina();

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void UseStaminaValue(float Value);

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void StartRegenStamina();

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void StopRegenStamina();

private:

	FTimerHandle RegenHealthTimer;
	FTimerHandle RegenStaminaTimer;
	FTimerHandle UseStaminaTimer;

	UFUNCTION(Unreliable, Server, WithValidation, Category="Health")
	void Server_GetDamage(float Damage);

	UFUNCTION(Unreliable, Server, WithValidation, Category="Health")
	void Server_RegenHealth();

	UFUNCTION(Unreliable, Server, WithValidation, Category="Stamina")
	void Server_UseStamina();

	UFUNCTION(Unreliable, Server, WithValidation, Category="Stamina")
	void Server_RegenStamina();

	UFUNCTION(Unreliable, Server, WithValidation, Category="Stamina")
	void Server_UseStaminaValue(float Value);
};
