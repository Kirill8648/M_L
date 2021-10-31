// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "Characters/MLCharacterBase.h"
#include "MLHeroCharacter.generated.h"

class AMLWeapon;

/**
* The base Character class for the game. Everything with an AbilitySystemComponent in this game will inherit from this class.
* This class should not be instantiated and instead subclassed.
*/
UCLASS()
class MERCLIFE_API AMLHeroCharacter : public AMLCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMLHeroCharacter(const class FObjectInitializer& ObjectInitializer);

	FGameplayTag CurrentWeaponRightTag;
	FGameplayTag CurrentWeaponLeftTag;
	FGameplayTag CurrentWeaponBothTag;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MercLife|MLHeroCharacter")
	USkeletalMeshComponent* GetFirstPersonMesh();

	UFUNCTION(BlueprintCallable, Category="MercLife|TEMPINVENTORY")
	void EquipWeapon(AMLWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, Category="MercLife|TEMPINVENTORY")
	virtual void NextWeapon();

	UFUNCTION(BlueprintCallable, Category="MercLife|TEMPINVENTORY")
	virtual void PreviousWeapon();

	FName GetWeaponAttachPoint() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	// TEMPORARY
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MercLife|TEMPINVENTORY")
	TArray<TSubclassOf<AMLWeapon>> WeaponsDefaultInventory;

protected:
	//TEMPORARY
	UPROPERTY()
	AMLWeapon* CurrentWeapon;
	UPROPERTY(BlueprintReadOnly, Category="MercLife|TEMPINVENTORY")
	TArray<AMLWeapon*> Weapons;

	UPROPERTY(BlueprintReadOnly, Category = "MercLife|MLHeroCharacter")
	FVector StartingThirdPersonMeshLocation;

	UPROPERTY(BlueprintReadOnly, Category = "MercLife|MLHeroCharacter")
	FVector StartingFirstPersonMeshLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MercLife|Camera")
	float BaseTurnRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MercLife|Camera")
	float BaseLookUpRate;

	bool bASCInputBound;

	// Set to true when we change the weapon predictively and flip it to false when the Server replicates to confirm.
	// We use this if the Server refused a weapon change ability's activation to ask the Server to sync the client back up
	// with the correct CurrentWeapon.
	bool bChangedWeaponLocally;

	UPROPERTY(BlueprintReadOnly, Category = "MercLife|Camera")
	float Default1PFOV;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Merclife|MLHeroCharacter")
	FName WeaponAttachPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MercLife|Camera")
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MercLife|MLHeroCharacter")
	TSubclassOf<UGameplayEffect> DeathEffect;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

	void LookUp(float Value);

	void Turn(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	// Client only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

	// Server spawns default inventory
	void SpawnDefaultInventory();

	void SetCurrentWeapon(AMLWeapon* NewWeapon, AMLWeapon* LastWeapon);

	// Unequips the specified weapon. Used when OnRep_CurrentWeapon fires.
	void UnEquipWeapon(AMLWeapon* WeaponToUnEquip);

	// Unequips the current weapon. Used if for example we drop the current weapon.
	void UnEquipCurrentWeapon();

	//void OnAbilityActivationFailed(const class UGameplayAbility* FailedAbility, const FGameplayTagContainer& FailTags);
};
