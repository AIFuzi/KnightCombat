#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponCombatComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KNIGHTCOMBAT_API UWeaponCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWeaponCombatComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordSetup")
	int TraceValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordSetup")
	bool DrawDebugInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordSetup")
	FName SocketStartName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordSetup")
	FName SocketEndName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordSetup")
	FName SocketWeaponSpawnName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordSetup")
	float SwordTracingRate;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CreateWeaponSword(TSubclassOf<class ABaseWeaponSword> WeaponClass);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StartTraceSwordAttack();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StopTraceSwordAttack();

	UFUNCTION(BlueprintPure, Category="Weapon")
	ABaseWeaponSword* GetCurrentWeaponSword() const;

private:

	UFUNCTION(Unreliable, Server, WithValidation, Category="Weapon")
	void Server_CreateWeaponSword(TSubclassOf<ABaseWeaponSword> WeaponClass);

	UFUNCTION(Unreliable, Server, WithValidation, Category="Weapon")
	void Server_SwordAttackTrace();

	UFUNCTION(Unreliable, NetMulticast, WithValidation, Category="Weapon")
	void Multicast_SwordAttackTrace();

	UFUNCTION(Unreliable, NetMulticast, WithValidation, Category="Weapon")
	void Multicast_DrawDebugInfo(FVector LerpVecVal, FVector EndLoc, float LerpVal);

	UPROPERTY(Replicated)
	ABaseWeaponSword* CurrentWeaponSword;

	UPROPERTY()
	class ABaseCharacter* CharOwner;

	TArray<FVector> LastTraceHitLoc;

	FTimerHandle SwordAttackTimer;
	FTimerHandle CooldownTimer;

	
		
};
