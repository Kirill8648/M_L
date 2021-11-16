// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/MLCharacterBase.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSetBase.h"
#include "GASGameplayAbilityBase.h"
#include "Characters/MLCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMLCharacterBase::AMLCharacterBase(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UMLCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	// TODO ----------------TEMPORARY-----------------
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);

	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UGASAttributeSetBase>(TEXT("AttributeSetBase"));
	// -----------------------TEMP--------------------
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Pawn or PlayerController is responsible for this character actions
	bAlwaysRelevant = true;
}

UAbilitySystemComponent* AMLCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AMLCharacterBase::IsAlive() const
{
	//TODO
	return 1/*GetHealth() > 0.0f*/;
}

int32 AMLCharacterBase::GetAbilityLevel(EMLAbilityInputID AbilityID) const
{
	//TODO
	return 1;
}

void AMLCharacterBase::RemoveCharacterAbilities()
{
	//TODO
}

void AMLCharacterBase::Die()
{
	//TODO
}

void AMLCharacterBase::FinishDying()
{
	Destroy();
}

float AMLCharacterBase::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float AMLCharacterBase::GetMaxHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float AMLCharacterBase::GetHealthRegenRate() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealthRegenRate();
	}
	return 0.0f;
}

/*float AMLCharacterBase::GetStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStamina();
	}
	return 0.0f;
}

float AMLCharacterBase::GetMaxStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxStamina();
	}
	return 0.0f;
}

float AMLCharacterBase::GetStaminaRegenRate() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStaminaRegenRate();
	}
	return 0.0f;
}*/

float AMLCharacterBase::GetMoveSpeed() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}
	return 0.0f;
}

float AMLCharacterBase::GetMoveSpeedBaseValue() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase)->GetBaseValue();
	}
	return 0.0f;
}


// Called when the game starts or when spawned
void AMLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMLCharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server
	/*if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->
		bCharacterAbilitiesGiven)
	{
		return;
	}*/

	for (TSubclassOf<UGASGameplayAbilityBase>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility,
		                                                         GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
		                                                         static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
		                                                         this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AMLCharacterBase::InitializeAttributes()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."),
		       *FString(__FUNCTION__), *GetName())
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	//TODO change 1 to getLevel if we gonna have levels
	FGameplayEffectSpecHandle GEHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (GEHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());
	}
}

void AMLCharacterBase::AddStartupEffects()
{
	/*if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->
		bStartupEffectsApplied)
	{
		return;
	}*/

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle GEHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (GEHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*GEHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void AMLCharacterBase::SetHealth(float Health)
{
	if (IsValid(AttributeSetBase))
	{
		AttributeSetBase->SetHealth(Health);
	}
}

/*void AMLCharacterBase::SetStamina(float Stamina)
{
	if (IsValid(AttributeSetBase))
	{
		AttributeSetBase->SetStamina(Stamina);
	}
}*/
