// replcae later

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()
public:
	UPawnExtensionComponentBase();
	
protected:
	
	template<class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template parameter to GetPawn must be derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template parameter to GetController must be derived from AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
