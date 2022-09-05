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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	class UBoxComponent* SwordCollision;

	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing = OnRep_WeaponOwner, Category="Weapon")
	class ABaseCharacter* WeaponOwner;

	UFUNCTION()
	void OnRep_WeaponOwner();
	
};
