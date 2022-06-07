// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"

//создаем собственную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;

    //функция для подписания на AnimNotify
    InitAnimations();

    //вызываем функцию спавна оружия
    SpawnWeapons();

    //функция определяющая текущее оружие и приаттачивание его к руке персонажа
    EquipWeapon(CurrentWeaponIndex);
}

//функция удаляющая оружие в конце игры
void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    //проходимся по массиву Weapons
    for (auto Weapon : Weapons)
    {
        //отсоединяем оружие от персонажа
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        //уничтожаем оружие
        Weapon->Destroy();
    }
    //очищаем массив оружия
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

//функция спавна оружия
void USTUWeaponComponent::SpawnWeapons()
{
    //создаем переменную куда записываем нашего персонажа - владельца этого компонента
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //проверяем на валидность игру и указатель на персонажа. если да выходим из функции
    if (!GetWorld() || !Character)
        return;

    //спавним оружие
    for (auto WeaponClass : WeaponClasses)
    {
        //спавним оружие созданного класса и записываем в переменную
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon)
            continue;

        //для каждого заспавненого оружия назначаем ownera - нашего персонажа
        Weapon->SetOwner(Character);

        //добавляем оружие в массив указателей
        Weapons.Add(Weapon);

        //аттачим оружие к сокету на спине персонажа с помощью функции
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

//функция присоединения оружия к сокету персонажа по имени сокета
void USTUWeaponComponent::AttachWeaponToSocket(
    ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent)
        return;

    //создаем переменную определяющую правила трансформации при присоединении
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    //аттачим к мешу персонажу с помощью переменной трансформации и переменной содержащей сокет присоединения
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

//функция определяющая текущее оружие и приаттачивание его к руке персонажа
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    //создаем переменную куда записываем владельца этого компонента
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    //убираем текущее оружие за спину
    if (CurrentWeapon)
    {
        //останавливаем стрельбу
        CurrentWeapon->StopFire();

        //атаччим текущее оружие к спине персонажа
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    //меняем указатель на другое оружие
    CurrentWeapon = Weapons[WeaponIndex];

    //аттачим оружие к руке персонажа
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    //начинаем процесс смены оружия
    EquipAnimInProgress = true;

    //вызываем функция воспроизведения анимации
    PlayAnimMontage(EquipAnimMontage);
}

//функция смены оружия
void USTUWeaponComponent::NextWeapon()
{
    //проверяем можно ли менять оружие
    if (!CanEquip())
        return;

    //увеличиваем индекс в переделах массива Weapons
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

//функция проигрывания анимации смены оружия/перезарядки
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    //создаем переменную куда записываем владельца этого компонента
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    //воспроизводим анимацию
    Character->PlayAnimMontage(Animation);
}

//функция для подписания на AnimNotify
void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage)
        return;

    //создаем массив эвентов
    const auto NotifyEvents = EquipAnimMontage->Notifies;

    //перебираем массив
    for (auto NotifyEvent : NotifyEvents)
    {
        //находим нужный notify и записываем в переменную
        auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);

        //если нашли нужный notify биндим функцию через делегат
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

//функция биндинга на делегат
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    //создаем переменную куда записываем владельца этого компонента
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //только для нашего персонажа
    if (!Character || MeshComponent != Character->GetMesh())
        return;

    //заканчиваем процесс смены оружия
    EquipAnimInProgress = false;
}

//функции позволяющие стрелять/менять оружие
bool USTUWeaponComponent::CanFire() const
{
    //проверяем есть ли оружие и не идет ли смена оружия
    return CurrentWeapon && !EquipAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress;
}

//функции стрельбы
void USTUWeaponComponent::StartFire()
{
    //проеверяем можем ли стрелять
    if (!CanFire())
        return;

    //вызываем функцию StartFire в STUBaseWeapon
    CurrentWeapon->StartFire();
}
void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    //вызываем функцию StopFire в STUBaseWeapon
    CurrentWeapon->StopFire();
}
