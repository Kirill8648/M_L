// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MLShootingWeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MERCLIFE_API UMLShootingWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMLShootingWeaponAttributeSet();

	/*UPROPERTY(BlueprintReadOnly, Category="Ammo")
	FGameplayAttributeData */

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
