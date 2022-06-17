// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

//создаем собственную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    //выключаем Tick
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    //в начале игры или спавне персонажа здоровье мах
    SetHealth(MaxHealth);

    //получаем указатель на нашего персонажа
    AActor* CompomemtOwner = GetOwner();
    if (CompomemtOwner)
    {
        //подписываемся на делегат OnTakeAnyDamage встроенный в функции TakeDamage актора
        //и биндим нашу функцию персонажа OnTakeAnyDamage
        CompomemtOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage); //наша функция
    }
}

//функция установки здоровья персонажу
void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    //изменяем здоровье в персонаже через делегат
    OnHealthChanged.Broadcast(Health);
}

//функция восстановления здоровья
void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    //останавливаем таймер если здоровье достигло max уровня
    //числа float проверяем с помощью функции IsNearlyEqual
    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

//функция нанесения любого ущерба персонажу
void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
    class AController* InstigatedBy, AActor* DamageCauser)
{
    //выходим из функции если Damage отрицательный и если уже мертвы
    if (Damage <= 0.0f || IsDead() || !GetWorld())
        return;
    //изменяем здоровье персонажа
    SetHealth(Health - Damage);

    //останавливаем таймер авто восстановления здоровья
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    //проверяем живы или нет
    if (IsDead())
    {
        //оповещаем всех что персонаж умер через делегат
        OnDeath.Broadcast();
    }
    //если не умерли проверяем включена ли AutoHeal
    else if (AutoHeal)
    {
        //включаем таймер
        //параметры - дискриптор, указатель на объект где вызывается таймер, ссылка на функцию, частота обновления,
        //вызов таймера циклично, задержка перед первым вызовом
        GetWorld()->GetTimerManager().SetTimer(
            HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
}

//функция вернет true когда здоровье max
bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

//функция добавления здоровья через пикап
bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    //условие выхода из функции: персонаж мертв или здоровье max или кол-во добав. здоровья отриц.
    if (IsDead() || IsHealthFull() || HealthAmount <= 0)
        return false;

    SetHealth(Health + HealthAmount);

    return true;
}
