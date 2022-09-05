#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KNIGHTCOMBAT_API UWeaponCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWeaponCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TEST")
	int TraceValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TEST")
	bool DrawDebugInfo;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CreateWeaponSword(TSubclassOf<class ABaseWeaponSword> WeaponClass);

private:

	UFUNCTION(Unreliable, Server, WithValidation, Category="Weapon")
	void Server_CreateWeaponSword(TSubclassOf<class ABaseWeaponSword> WeaponClass);

	UPROPERTY()
	ABaseWeaponSword* CurrentWeaponSword;
		
};
