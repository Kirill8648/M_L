// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "GASAttributeSetBase.h"
#include "Net/UnrealNetwork.h"

UGASAttributeSetBase::UGASAttributeSetBase()
{
}

void UGASAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, HealthRegenRate, COND_None, REPNOTIFY_Always);
	/*DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, StaminaRegenRate, COND_None, REPNOTIFY_Always);*/
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, WeaponSpreadX, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSetBase, WeaponSpreadY, COND_None, REPNOTIFY_Always);
}

void UGASAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, Health, OldHealth);
}

void UGASAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UGASAttributeSetBase::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, HealthRegenRate, OldHealthRegenRate);
}

/*
void UGASAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, Stamina, OldStamina);
}

void UGASAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, MaxStamina, OldMaxStamina);
}

void UGASAttributeSetBase::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, StaminaRegenRate, OldStaminaRegenRate);
}
*/

void UGASAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

void UGASAttributeSetBase::OnRep_WeaponSpreadX(const FGameplayAttributeData& OldWeaponSpreadX)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, WeaponSpreadX, OldWeaponSpreadX);
}

void UGASAttributeSetBase::OnRep_WeaponSpreadY(const FGameplayAttributeData& OldWeaponSpreadY)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSetBase, WeaponSpreadY, OldWeaponSpreadY);
}
