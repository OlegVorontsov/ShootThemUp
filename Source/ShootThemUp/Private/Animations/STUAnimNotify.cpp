// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //�������� ��������� �������
    OnNotified.Broadcast(MeshComp);

    //�������� ����������� �������
    Super::Notify(MeshComp, Animation);
}
