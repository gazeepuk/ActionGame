#pragma once

namespace Debug
{
	static void Print(const FString& Message, const FColor& MessageColor = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 5.f, MessageColor, Message);

			UE_LOG(LogTemp,	Warning, TEXT("%s"), *Message);
		}
	}	
}
