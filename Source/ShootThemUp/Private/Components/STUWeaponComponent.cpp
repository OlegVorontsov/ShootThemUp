// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

//создаем собственную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

//ограничитель кол-ва единиц оружия
constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("This character can hold only %i weapon items!"), WeaponNum);

    //функция для подписания на AnimNotify
    InitAnimations();

    CurrentWeaponIndex = 0;

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
    for (auto OneWeaponData : WeaponData)
    {
        //спавним оружие созданного класса и записываем в переменную
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon)
            continue;

        //подписываемся на делегат
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);

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
    //проверяем WeaponIndex
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(WeaponComponentLog, Warning, TEXT("Invalid weapon index!"));
        return;
    }

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

    //получаем указатель на структуру текущего оружия
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                //
    });

    //проверяем не нулевой ли указатель CurrentWeaponData. если не нулевой устанавливаем анимацию
    //нулевой - делаем указатель nullptr
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    //аттачим оружие к руке персонажа
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    //начинаем процесс смены оружия
    EquipAnimInProgress = true;

    //вызываем функцию воспроизведения анимации смены оружия
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
    // c помощью шаблонной функции ищем нужный notify и записываем в переменную
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);

    //если нашли нужный notify биндим функцию через делегат
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is forgotten to set!"));
        checkNoEntry();
    }

    //проходимся по массиву WeaponData
    for (auto OneWeaponData : WeaponData)
    {
        // c помощью шаблонной функции ищем нужный notify и записываем в переменную
        auto ReloadFinishedNotify =
            AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);

        //если не нашли нужный notify продолжим искать. нашли биндим функцию через делегат
        if (!ReloadFinishedNotify)
        {
            UE_LOG(WeaponComponentLog, Error, TEXT("Reload anim notify is forgotten to set!"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
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

    //заканчиваем процесс анимации смены оружия
    EquipAnimInProgress = false;
}

//функция биндинга на делегат
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    //создаем переменную куда записываем владельца этого компонента
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //только для нашего персонажа
    if (!Character || MeshComponent != Character->GetMesh())
        return;

    //заканчиваем процесс анимации смены оружия
    ReloadAnimInProgress = false;
}

//функции позволяющие стрелять/менять/перезаряжать оружие
bool USTUWeaponComponent::CanFire() const
{
    //проверяем есть ли оружие и не идет ли смена оружия и не идет ли перезарядка
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
    //проверяем есть ли оружие и не идет ли смена оружия и не идет ли перезарядка и может ли оружие перезаряжаться
    return CurrentWeapon            //
           && !EquipAnimInProgress  //
           && !ReloadAnimInProgress //
           && CurrentWeapon->CanReload();
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

//функция перезарядки по нажатой клавише игрока
void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

//функция бинда на делегат OnClipEmpty - закончились пастроны в BaseWeapon
void USTUWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

//функция перезарядки оружия
void USTUWeaponComponent::ChangeClip()
{
    //проверяем если не можем перезаряжаться выходим из функции
    if (!CanReload())
        return;
    //останавливаем стрельбу
    CurrentWeapon->StopFire();

    //заменяем магазин
    CurrentWeapon->ChangeClip();

    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

//функция возвращает данные об иконках текущего оружия
bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

//функция возвращает данные о текущем арсенале оружия
bool USTUWeaponComponent::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}
