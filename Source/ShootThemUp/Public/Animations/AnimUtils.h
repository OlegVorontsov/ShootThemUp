#pragma once

class AnimUtils
{
public:
    //шаблонная функция поиска notify
    template <typename T> static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation)
            return nullptr;

        //создаем массив эвентов
        const auto NotifyEvents = Animation->Notifies;

        //перебираем массив
        for (auto NotifyEvent : NotifyEvents)
        {
            //находим нужный notify и записываем в переменную
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);

            //если нашли нужный notify биндим функцию через делегат
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};
