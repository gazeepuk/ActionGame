// replcae later


#include "AGBaseAnimInstance.h"

#include "FunctionLibraries/AGFunctionLibrary.h"

bool UAGBaseAnimInstance::DoesOwnerHasTag(const FGameplayTag TagToCheck) const
{
	if(APawn* OwningPawn = TryGetPawnOwner())
	{
		return UAGFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}
	
	return false;
}
