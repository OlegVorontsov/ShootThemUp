// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //вызываем созданный делегат
    OnNotified.Broadcast(MeshComp);

    //вызываем стандартную функцию
    Super::Notify(MeshComp, Animation);
}
