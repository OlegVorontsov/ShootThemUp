#pragma once

class STUUtils
{
public:

    //шаблонная функция возвращающая указатель на PlayerComponent
	template<typename T>
	static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	{
        if (!PlayerPawn)
            return nullptr;

        //получаем указатель на STUHealthComponent
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
	}
};