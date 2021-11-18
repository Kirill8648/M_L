// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Weapons/MLWeapon.h"
#include "GASAbilitySystemComponent.h"
#include "GASGameplayAbilityBase.h"
#include "Characters/Heroes/MLHeroCharacter.h"
#include "Kismet/GameplayStatics.h"


AMLWeapon::AMLWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	AmmoType = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.None"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->CastShadow = true;
	WeaponMesh->SetVisibility(false, true);
	WeaponMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
}

UAbilitySystemComponent* AMLWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USkeletalMeshComponent* AMLWeapon::GetWeaponMesh() const
{
	return WeaponMesh;
}

void AMLWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMLWeapon::SetOwningCharacter(AMLHeroCharacter* InOwningCharacter)
{
	OwningCharacter = InOwningCharacter;

	if (OwningCharacter)
	{
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());
		SetOwner(InOwningCharacter);
		AttachToComponent(OwningCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		AbilitySystemComponent = nullptr;
		SetOwner(nullptr);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AMLWeapon::Equip() //TODO add variation for AI
{
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s OwningCharacter is nullptr"), *FString(__FUNCTION__), *GetName());
		return;
	}

	FName AttachPoint = OwningCharacter->GetWeaponAttachPoint();

	if (WeaponMesh)
	{
		WeaponMesh->AttachToComponent(OwningCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		WeaponMesh->SetRelativeLocation(WeaponMeshEquippedRelativeLocation);
		WeaponMesh->SetRelativeRotation(WeaponMeshEquippedRelativeRotation);
		WeaponMesh->SetVisibility(true, true);
	}


	UAbilitySystemComponent* OwningCharacterASC = OwningCharacter->GetAbilitySystemComponent();
	if (!IsValid(OwningCharacterASC))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), EquipSound);

	if (!DefaultSpread)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultSpread for %s. Please fill in the weapon's Blueprint."),
		       *FString(__FUNCTION__), *GetName())
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = OwningCharacterASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	//TODO change 1 to getLevel if we gonna have levels
	FGameplayEffectSpecHandle GEHandle = OwningCharacterASC->MakeOutgoingSpec(DefaultSpread, 1, EffectContext);
	if (GEHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = OwningCharacterASC->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());
	}
}

void AMLWeapon::UnEquip() //TODO add variation for AI
{
	if (OwningCharacter == nullptr)
	{
		return;
	}

	//Detach for prevent unwanted unhides
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponMesh->SetVisibility(false, true);

	UGameplayStatics::PlaySound2D(GetWorld(), UnEquipSound);

	OwningCharacter->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(DefaultSpread, nullptr, -1);
}

void AMLWeapon::AddAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		return;
	}

	UGASAbilitySystemComponent* ASC = Cast<UGASAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());

	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s ASC is null"), *FString(__FUNCTION__), *GetName());
		return;
	}

	if (GetLocalRole() != ROLE_Authority) // Only on the server
	{
		return;
	}

	for (TSubclassOf<UGASGameplayAbilityBase>& Ability : Abilities)
	{
		AbilitySpecHandles.Add(ASC->GiveAbility(
			FGameplayAbilitySpec(Ability, GetAbilityLevel(Ability.GetDefaultObject()->AbilityID),
			                     static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this)));
	}
}

void AMLWeapon::RemoveAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		return;
	}

	UGASAbilitySystemComponent* ASC = Cast<UGASAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());

	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s ASC is null"), *FString(__FUNCTION__), *GetName());
		return;
	}

	if (GetLocalRole() != ROLE_Authority) // Only on the server
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitySpecHandles)
	{
		ASC->ClearAbility(AbilitySpecHandle);
	}
}

int32 AMLWeapon::GetAbilityLevel(EMLAbilityInputID AbilityInputID)
{
	//TODO
	return 1;
}

void AMLWeapon::ResetWeapon()
{
	//TODO
}

UAnimMontage* AMLWeapon::GetEquipMontage() const
{
	return EquipMontage;
}

UAnimMontage* AMLWeapon::GetUnEquipMontage() const
{
	return UnEquipMontage;
}

TSubclassOf<UAnimInstance> AMLWeapon::GetHeroFPAnimInstance() const
{
	return HeroFPAnimInstance;
}

void AMLWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMLWeapon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
