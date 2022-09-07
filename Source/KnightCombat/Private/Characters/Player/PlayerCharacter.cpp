#include "Characters/Player/PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetMesh());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);
}

void APlayerCharacter::MoveForward(float Value)
{
	if(Controller && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(FRotator(0.f, Rotation.Yaw, 0.f)).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if(Controller && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(FRotator(0.f, Rotation.Yaw, 0.f)).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * MouseSensitivity);
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * MouseSensitivity);
}

void APlayerCharacter::StartSprint()
{
	Server_StartSprint();
}

void APlayerCharacter::StopSprint()
{
	Server_StopSprint();
}

void APlayerCharacter::Server_StartSprint_Implementation()
{
	if(HealthComponent->CurrentStamina >= HealthComponent->StaminaForSprint && GetVelocity().Length() > 0.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		Client_UpdateCharacterSpeed(RunSpeed);

		HealthComponent->StartUseStamina();
		HealthComponent->OnStaminaEnded.AddDynamic(this, &APlayerCharacter::Server_StopSprint);
	}
}

bool APlayerCharacter::Server_StartSprint_Validate() { return true; }

void APlayerCharacter::Server_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Client_UpdateCharacterSpeed(WalkSpeed);

	HealthComponent->StopUseStamina();
}

bool APlayerCharacter::Server_StopSprint_Validate() { return true; }

void APlayerCharacter::Client_UpdateCharacterSpeed_Implementation(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

bool APlayerCharacter::Client_UpdateCharacterSpeed_Validate(float Speed) { return true; }


