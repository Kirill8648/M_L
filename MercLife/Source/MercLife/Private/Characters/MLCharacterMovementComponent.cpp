// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/MLCharacterMovementComponent.h"

#include "Characters/MLCharacterBase.h"


UMLCharacterMovementComponent::UMLCharacterMovementComponent()
{
	SprintSpeedMultiplier = 1.4f;
	ADSSpeedMultiplier = 0.8f;
}

float UMLCharacterMovementComponent::GetMaxSpeed() const
{
	AMLCharacterBase* Owner = Cast<AMLCharacterBase>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive())
	{
		return 0.0f;
	}

	if (RequestToStartSprinting)
	{
		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}

	if (RequestToStartADS)
	{
		return Owner->GetMoveSpeed() * ADSSpeedMultiplier;
	}

	return Owner->GetMoveSpeed();
}

void UMLCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	// The Flags parameter contains the compressed input flags that are stored in the saved move.
	// UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	// It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UMLCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	if (!ClientPredictionData)
	{
		UMLCharacterMovementComponent* MutableThis = const_cast<UMLCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FMLNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.0f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
	}

	return ClientPredictionData;
}

void UMLCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UMLCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UMLCharacterMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UMLCharacterMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}

void UMLCharacterMovementComponent::FMLSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UMLCharacterMovementComponent::FMLSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UMLCharacterMovementComponent::FMLSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	// Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FMLSavedMove*)NewMove.Get())->SavedRequestToStartSprinting)
		return false;

	if (SavedRequestToStartADS != ((FMLSavedMove*)NewMove.Get())->SavedRequestToStartADS)
		return false;

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UMLCharacterMovementComponent::FMLSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel,
                                                             FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UMLCharacterMovementComponent* CharacterMovement = Cast<UMLCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UMLCharacterMovementComponent::FMLSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UMLCharacterMovementComponent* CharacterMovement = Cast<UMLCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UMLCharacterMovementComponent::FMLNetworkPredictionData_Client::FMLNetworkPredictionData_Client(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UMLCharacterMovementComponent::FMLNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FMLSavedMove());
}
