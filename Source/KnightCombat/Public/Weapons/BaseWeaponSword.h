#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponSword.generated.h"

UCLASS()
class KNIGHTCOMBAT_API ABaseWeaponSword : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseWeaponSword();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	class UBoxComponent* SwordCollision;

	UFUNCTION(BlueprintCallable, Category="WeaponSetup")
	void SetCharacterOwner(class ABaseCharacter* NewOwner);

protected:

	

private:

	UPROPERTY()
	ABaseCharacter* WeaponOwner;
	
};
