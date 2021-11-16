// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MercLife/MercLife.h"
#include "MLCharacterBase.generated.h"

UCLASS()
class MERCLIFE_API AMLCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMLCharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase")
	virtual bool IsAlive() const;

	// Switch on AbilityID to return ability level
	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase")
	virtual int32 GetAbilityLevel(EMLAbilityInputID AbilityID) const;
	
	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase")
	virtual void FinishDying();


	/**
	* Getters for attributes from GASAttributeSetBase
	**/

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetHealthRegenRate() const;

	/*UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetStaminaRegenRate() const;*/

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "MercLife|MLCharacterBase|Attributes")
	float GetMoveSpeedBaseValue() const;

protected:
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;
	
	// Reference to the ASC. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	UPROPERTY()
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	// Reference to the AttributeSetBase. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	UPROPERTY()
	class UGASAttributeSetBase* AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MercLife|MLCharacterBase")
	FText CharacterName;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MercLife|Abilities")
	TArray<TSubclassOf<class UGASGameplayAbilityBase>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MercLife|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MercLife|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();


	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/

	virtual void SetHealth(float Health);
	//virtual void SetStamina(float Stamina);
};
