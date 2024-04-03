// Hello :)


#include "Weapon/Weapon.h"

#include "Character/BMCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/BMPlayerController.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetIsReplicated(true);
	
	WeaponMesh->SetCustomDepthStencilValue(250);
	WeaponMesh->MarkRenderStateDirty();

	PickUpSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpSphere"));
	PickUpSphere->SetupAttachment(RootComponent);
	PickUpSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickUpSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);

}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	PickUpSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickUpSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	PickUpSphere->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnSphereOverlap);
	PickUpSphere->OnComponentEndOverlap.AddDynamic(this,&AWeapon::OnSphereEndOverlap);
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(false);
	}
	
}
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon,WeaponState);
}


void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if(Owner == nullptr)
	{
		PlayerCharacter = nullptr;
		PlayerController = nullptr;
	}
}
void AWeapon::SetHudAmmo()
{
}
void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EW_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld,true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	PlayerCharacter = nullptr;
	PlayerController = nullptr;
}


void AWeapon::ShowPickUpWidget(bool bShowWidget)
{
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(bShowWidget);
	}
}
void AWeapon::OnRep_WeaponState()
{
	OnWeaponStateSet();
}
void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateSet();
}
void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EW_Equipped:
		OnEquipped();
		break;
	case EWeaponState::EW_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EW_Dropped:
		OnDropped();
		break;
	}
}
void AWeapon::OnEquipped()
{
	ShowPickUpWidget(false);
	GetPickUpSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AWeapon::OnDropped()
{
	if(HasAuthority())
	{
		GetPickUpSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
}

void AWeapon::OnEquippedSecondary()
{
	ShowPickUpWidget(false);
	GetPickUpSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABMCharacter* Character = Cast<ABMCharacter>(OtherActor);
	if(Character)
	{
		Character->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABMCharacter* Character = Cast<ABMCharacter>(OtherActor);
	if(Character)
	{
		
		Character->SetOverlappingWeapon(nullptr);
	}
}


