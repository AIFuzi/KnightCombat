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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordAnimations")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordAnimations")
	UAnimMontage* BlockAttackImpactAnimMontage;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CreateWeaponSword(TSubclassOf<class ABaseWeaponSword> WeaponClass);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttackSword();

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

	UFUNCTION(Unreliable, Server, Category="Weapon")
	void Server_AttackSword();

	UFUNCTION(Unreliable, NetMulticast, Category="Weapon")
	void Multicast_AttackSword();

	UPROPERTY(Replicated)
	ABaseWeaponSword* CurrentWeaponSword;

	UPROPERTY()
	TArray<AActor*> HitActors;
	
	FTimerHandle SwordAttackTimer;
	FTimerHandle CooldownTimer;
	
	TArray<FVector> LastTraceHitLoc;
	bool bIsAttackCooldown;

	void ClearCooldown();
	
		
};
