// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Animations/STUEquipFinishedAnimNotify.h"

void USTUEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //�������� ��������� �������
    OnNotified.Broadcast(MeshComp);

    //�������� ����������� �������
    Super::Notify(MeshComp, Animation);
}
