// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    //вызываем функцию спавна оружи€
    SpawnWeapon();
}

//функци€ спавна оружи€
void USTUWeaponComponent::SpawnWeapon()
{
    //провер€ем на валидность игру. если да выходим из функции
    if (!GetWorld())
        return;

    //создаем переменную куда записываем владельца этого компонента
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //провер€ем создалась ли переменна€
    if (!Character)
        return;

    //спавним оружие созданного класса и записываем в переменную
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);

    if (!CurrentWeapon)
        return;

    //создаем переменную определ€ющую правила трансформации при присоединении
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    //аттачим к мешу персонажу с помощью переменной трансформации и переменной содержащей сокет присоединени€
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);

    //указываем владельца оружи€
    CurrentWeapon->SetOwner(Character);
}

//функци€ определ€юща€ входную точку дл€ стрельбы
void USTUWeaponComponent::Fire()
{
    if (!CurrentWeapon)
        return;

    //вызываем функцию Fire в STUBaseWeapon
    CurrentWeapon->Fire();
}
