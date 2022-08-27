#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class KNIGHTCOMBAT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float Value);
	void MoveRight(float Right);
	
};
