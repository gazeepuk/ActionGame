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

	static void Print(const FString& FloatTitle, float FloatValue, const FColor& MessageColor = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if(GEngine)
		{
			const FString DebugMessage = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValue);

			GEngine->AddOnScreenDebugMessage(InKey, 5.f, MessageColor, DebugMessage);

			UE_LOG(LogTemp,	Warning, TEXT("%s"), *DebugMessage);
		}
	}
}
