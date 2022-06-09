#pragma once

class AnimUtils
{
public:
    //��������� ������� ������ notify
    template <typename T> static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation)
            return nullptr;

        //������� ������ �������
        const auto NotifyEvents = Animation->Notifies;

        //���������� ������
        for (auto NotifyEvent : NotifyEvents)
        {
            //������� ������ notify � ���������� � ����������
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);

            //���� ����� ������ notify ������ ������� ����� �������
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};
