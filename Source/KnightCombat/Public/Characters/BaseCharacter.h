#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class KNIGHTCOMBAT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
