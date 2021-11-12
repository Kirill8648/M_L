// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/Heroes/MLHeroCharacter.h"

#include "GASAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Abilities/MLAbilitySystemGlobals.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/MLWeapon.h"

AMLHeroCharacter::AMLHeroCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	//FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetVisibility(true, true);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	GetMesh()->bCastHiddenShadow = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
	//TODO Create AI Controller 
	//AIControllerClass = AGSHeroAIController::StaticClass();
}

USkeletalMeshComponent* AMLHeroCharacter::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

void AMLHeroCharacter::EquipWeapon(AMLWeapon* NewWeapon)
{
	SetCurrentWeapon(NewWeapon, CurrentWeapon);
}

void AMLHeroCharacter::NextWeapon()
{
	if (Weapons.Num() < 2)
	{
		return;
	}

	int32 CurrentWeaponIndex = Weapons.Find(CurrentWeapon);
	UnEquipCurrentWeapon();

	if (CurrentWeaponIndex == INDEX_NONE)
	{
		EquipWeapon(Weapons[0]);
	}
	else
	{
		EquipWeapon(Weapons[(CurrentWeaponIndex + 1) % Weapons.Num()]);
	}
}

void AMLHeroCharacter::PreviousWeapon()
{
	if (Weapons.Num() < 2)
	{
		return;
	}

	int32 CurrentWeaponIndex = Weapons.Find(CurrentWeapon);
	UnEquipCurrentWeapon();

	if (CurrentWeaponIndex == INDEX_NONE)
	{
		EquipWeapon(Weapons[0]);
	}
	else
	{
		int32 IndexOfPrevWeapon = FMath::Abs(CurrentWeaponIndex - 1 + Weapons.Num()) % Weapons.Num();
		EquipWeapon(Weapons[IndexOfPrevWeapon]);
	}
}

FName AMLHeroCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
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

AMLWeapon* AMLHeroCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
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

	//SpawnDefaultInventory();

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
	if (IsAlive())AddControllerYawInput(Value);
}

void AMLHeroCharacter::MoveForward(float Value)
{
	if (IsAlive())
	{
		//AddMovementInput(GetActorForwardVector(), Value);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AMLHeroCharacter::MoveRight(float Value)
{
	if (IsAlive())
	{
		//AddMovementInput(GetActorRightVector(), Value);
		WeaponSwayMoveSide = Value;
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
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
	//TODO Not final method
	int32 NumWeaponClasses = WeaponsDefaultInventory.Num();
	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (!WeaponsDefaultInventory[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("%s %s An empty item was added to the Array in blueprint"), *FString(__FUNCTION__), *GetName());
			continue;
		}

		AMLWeapon* NewWeapon = GetWorld()->SpawnActorDeferred<AMLWeapon>(WeaponsDefaultInventory[i], FTransform::Identity, this,
		                                                                 this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewWeapon->FinishSpawning(FTransform::Identity);

		bool bEquipFirstWeapon = i == 0;

		Weapons.Add(NewWeapon);
		NewWeapon->SetOwningCharacter(this);
		NewWeapon->AddAbilities();

		if (bEquipFirstWeapon)
		{
			EquipWeapon(NewWeapon);
		}
	}
}

void AMLHeroCharacter::SetCurrentWeapon(AMLWeapon* NewWeapon, AMLWeapon* LastWeapon)
{
	if (NewWeapon == LastWeapon)
	{
		return;
	}

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	// Cancel active weapon abilities
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilityTagsToCancel = FGameplayTagContainer(UMLAbilitySystemGlobals::MLGet().WeaponAbilityTag);
		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel);
	}

	/*UAnimMontage* Equip1PMontage = NewWeapon->GetEquipMontage();
	if (Equip1PMontage && GetFirstPersonMesh())
	{
		GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(Equip1PMontage);
	}

	TimerDel.BindUFunction(this, FName("SetCurrentWeaponAfterDelay"), NewWeapon, LastWeapon);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 1.f/*Equip1PMontage->GetPlayLength() / 2#1#, false);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, "TimerSet");*/

	UnEquipWeapon(LastWeapon);

	if (NewWeapon)
	{
		// Controller changes

		// TODO add support for AI

		UAnimMontage* Equip1PMontage = NewWeapon->GetEquipMontage();
		if (Equip1PMontage && GetFirstPersonMesh())
		{
			GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(Equip1PMontage);
		}
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwningCharacter(this);
		CurrentWeapon->Equip();

		FirstPersonMesh->LinkAnimClassLayers(CurrentWeapon->GetHeroFPAnimInstance());
	}
	else
	{
		UnEquipCurrentWeapon();
	}
}

void AMLHeroCharacter::UnEquipWeapon(AMLWeapon* WeaponToUnEquip)
{
	if (WeaponToUnEquip)
	{
		FirstPersonMesh->UnlinkAnimClassLayers(WeaponToUnEquip->GetHeroFPAnimInstance());
		WeaponToUnEquip->UnEquip();
	}
}

void AMLHeroCharacter::UnEquipCurrentWeapon()
{
	UnEquipWeapon(CurrentWeapon);
	CurrentWeapon = nullptr;
}

/*void AMLHeroCharacter::SetCurrentWeaponAfterDelay(AMLWeapon* NewWeapon, AMLWeapon* LastWeapon)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, "TimerExecute");
	UnEquipWeapon(LastWeapon);

	if (NewWeapon)
	{
		// Controller changes

		// TODO add support for AI

		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwningCharacter(this);
		CurrentWeapon->Equip();

		FirstPersonMesh->LinkAnimClassLayers(CurrentWeapon->GetHeroFPAnimInstance());
	}
	else
	{
		UnEquipCurrentWeapon();
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}*/
