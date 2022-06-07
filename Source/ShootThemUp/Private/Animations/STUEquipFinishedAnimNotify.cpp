// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Animations/STUEquipFinishedAnimNotify.h"

void USTUEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //вызываем созданный делегат
    OnNotified.Broadcast(MeshComp);

    //вызываем стандартную функцию
    Super::Notify(MeshComp, Animation);
}
