#pragma once

class STUUtils
{
public:

    //��������� ������� ������������ ��������� �� PlayerComponent
	template<typename T>
	static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	{
        if (!PlayerPawn)
            return nullptr;

        //�������� ��������� �� STUHealthComponent
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
	}
};