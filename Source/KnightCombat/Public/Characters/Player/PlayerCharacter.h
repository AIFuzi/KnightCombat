#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class KNIGHTCOMBAT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
	float MouseSensitivity = 1.f;

protected:

	UFUNCTION(Server, Unreliable, Category="Movement")
	void Server_StartSprint();
	
	UFUNCTION(Server, Unreliable, Category="Movement")
	void Server_StopSprint();

	UFUNCTION(Client, Unreliable, Category="Movement")
	void Client_UpdateCharacterSpeed(float Speed);
	
private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void Turn(float Value);

	void StartSprint();
	void StopSprint();
	
};
