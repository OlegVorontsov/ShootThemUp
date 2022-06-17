// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
//#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    //переменная по умолчанию
    auto ImpactData = DefaultImpactData;

    if (Hit.PhysMaterial.IsValid())
    {
        //записываем в переменную указатель на физ материал в зав-ти от того куда попали
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat))
        {
            //переопределяем стандартный эффект
            ImpactData = ImpactDataMap[PhysMat];
        }
    }
    //вызываем функцию спавна эффекта
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
        ImpactData.NiagaraEffect,                              //
        Hit.ImpactPoint,                                       //
        Hit.ImpactNormal.Rotation());

    //вызываем спавн decal
    auto DecalComponent = UGamePlayStatics::SpawnDecalAtLocation(GetWorld(), //
        ImpactData.DecalData.Material,                                       //
        ImpactData.DecalData.Size,                                           //
        Hit.ImpactPoint,                                                     //
        Hit.ImpactNormal.Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}
