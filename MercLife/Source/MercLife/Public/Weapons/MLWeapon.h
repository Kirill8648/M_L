// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "MercLife/MercLife.h"
#include "MLWeapon.generated.h"

class AMLHeroCharacter;
class USkeletalMeshComponent;
class UAnimMontage;
class UGASAbilitySystemComponent;
class UGASGameplayAbilityBase;

UCLASS()
class MERCLIFE_API AMLWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMLWeapon();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MercLife|MLWeapon")
	FGameplayTag AmmoType;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MercLife|MLWeapon")
	virtual USkeletalMeshComponent* GetWeaponMesh() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetOwningCharacter(AMLHeroCharacter* InOwningCharacter);

	virtual void Equip();

	virtual void UnEquip();

	virtual void AddAbilities();

	virtual void RemoveAbilities();

	virtual int32 GetAbilityLevel(EMLAbilityInputID AbilityInputID);

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLWeapon")
	virtual void ResetWeapon();

	UFUNCTION(BlueprintCallable, Category = "MercLife|Animation")
	UAnimMontage* GetEquipMontage() const;

	UFUNCTION(BlueprintCallable, Category = "MercLife|Animation")
	TSubclassOf<UAnimInstance> GetHeroFPAnimInstance() const;

protected:
	UPROPERTY()
	UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "MercLife|MLWeapon")
	USkeletalMeshComponent* WeaponMesh;

	// Relative Location of weapon 1P Mesh when equipped
	UPROPERTY(EditDefaultsOnly, Category = "MercLife|MLWeapon")
	FVector WeaponMeshEquippedRelativeLocation;
	// Relative Rotation of weapon 1P Mesh when equipped
	UPROPERTY(EditDefaultsOnly, Category = "MercLife|MLWeapon")
	FRotator WeaponMeshEquippedRelativeRotation;

	UPROPERTY(BlueprintReadOnly, Category = "MercLife|MLWeapon")
	AMLHeroCharacter* OwningCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "MercLife|MLWeapon")
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY(EditAnywhere, Category = "MercLife|MLWeapon")
	TArray<TSubclassOf<UGASGameplayAbilityBase>> Abilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MercLife|MLWeapon")
	UAnimMontage* EquipMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MercLife|MLWeapon")
	TSubclassOf<UAnimInstance> HeroFPAnimInstance;

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
};
