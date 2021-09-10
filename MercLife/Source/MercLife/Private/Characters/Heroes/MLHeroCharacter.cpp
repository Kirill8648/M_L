// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/Heroes/MLHeroCharacter.h"

#include "GASAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Abilities/MLAbilitySystemGlobals.h"
#include "Kismet/KismetMathLibrary.h"

AMLHeroCharacter::AMLHeroCharacter(const class FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;
	bASCInputBound = false;
	Default1PFOV = 90.0f;
	CurrentWeaponRightTag = UMLAbilitySystemGlobals::MLGet().NoWeaponTag;
	CurrentWeaponLeftTag = UMLAbilitySystemGlobals::MLGet().NoWeaponTag;
	CurrentWeaponBothTag = UMLAbilitySystemGlobals::MLGet().NoWeaponTag;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetVisibility(false, true);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	GetMesh()->bCastHiddenShadow = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
	//TODO Create AI Controller 
	//AIControllerClass = AGSHeroAIController::StaticClass();
}

void AMLHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//TODO Add replication for inventory, etc.
	//DOREPLIFETIME(AMLHeroCharacter, Inventory);
}

void AMLHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMLHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMLHeroCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMLHeroCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AMLHeroCharacter::Turn);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

// Server only
void AMLHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//TODO Not finished PossessedBy in MLHeroCharacter
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddStartupEffects();
	AddCharacterAbilities();
}

/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void AMLHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	StartingFirstPersonMeshLocation = FirstPersonMesh->GetRelativeLocation();

	//TODO Not finished BeginPlay in MLHeroCharacter
}

void AMLHeroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//TODO Not finished EndPlay in MLHeroCharacter
	Super::EndPlay(EndPlayReason);
}

void AMLHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//TODO Not finished PostInitializeComponents in MLHeroCharacter
	GetWorldTimerManager().SetTimerForNextTick(this, &AMLHeroCharacter::SpawnDefaultInventory);
}

void AMLHeroCharacter::LookUp(float Value)
{
	if (IsAlive()) AddControllerPitchInput(Value);
}

void AMLHeroCharacter::Turn(float Value)
{
	if (IsAlive()) AddControllerYawInput(Value);
}

void AMLHeroCharacter::MoveForward(float Value)
{
	if (IsAlive())
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AMLHeroCharacter::MoveRight(float Value)
{
	if (IsAlive())
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AMLHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//TODO Not finished OnRep_PlayerState in MLHeroCharacter
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	BindASCInput();
}

void AMLHeroCharacter::BindASCInput()
{
	if (!bASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EMLAbilityInputID",
		                                       static_cast<int32>(EMLAbilityInputID::Confirm),
		                                       static_cast<int32>(EMLAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);

		bASCInputBound = true;
	}
}

void AMLHeroCharacter::SpawnDefaultInventory()
{
	//TODO Spawn default inventory
}
