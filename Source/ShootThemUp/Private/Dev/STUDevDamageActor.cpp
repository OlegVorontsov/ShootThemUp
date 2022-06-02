// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Dev/STUDevDamageActor.h"
//подключаем библиотеку позвол€ющую рисовать примитивы
#include "DrawDebugHelpers.h"
//библиотека позвол€ет получать всех акторов со сцены
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //создаем root актора
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //каждый тик рисуем сферу
    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

    //наносим ущерб всему что в сфере через получение всех акторов со сцены
    //с использованием функции TakeDamage всех акторов
    UGameplayStatics::ApplyRadialDamage(
        GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
}
