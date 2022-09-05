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

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CreateWeaponSword(TSubclassOf<class ABaseWeaponSword> WeaponClass);

private:

	UFUNCTION(Unreliable, Server, WithValidation, Category="Weapon")
	void Server_CreateWeaponSword(TSubclassOf<class ABaseWeaponSword> WeaponClass);
		
};
