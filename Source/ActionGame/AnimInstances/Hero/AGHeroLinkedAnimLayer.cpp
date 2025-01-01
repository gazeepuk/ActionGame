// replcae later


#include "AGHeroLinkedAnimLayer.h"

#include "AGHeroAnimInstance.h"

UAGHeroAnimInstance* UAGHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UAGHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
