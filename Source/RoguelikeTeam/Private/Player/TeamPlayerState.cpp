// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeamPlayerState.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Formation/FormationActorComponent.h"

class ATeamAIController;

ATeamPlayerState::ATeamPlayerState()
{
	FormationActorComponent = CreateDefaultSubobject<UFormationActorComponent>("FormationActorComponent");
}

void ATeamPlayerState::BeginPlay()
{
	Super::BeginPlay();

	ATeamCharacter* TeamCharacter = Cast<ATeamCharacter>(GetPawn());
	if (!TeamCharacter) return;

	FormationActorComponent -> InitPoint(TeamCharacter -> FormationInfo);
	FormationActorComponent->RootSceneComponent->AttachToComponent(TeamCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SpawnFormationTeam(TeamCharacter);
}

UFormationActorComponent* ATeamPlayerState::GetFormationActorComponent() const
{
	return FormationActorComponent;
}

void ATeamPlayerState::SpawnFormationTeam(ATeamCharacter* TeamCharacter)
{
	if (!FormationActorComponent) return;
	
	APointCollection* PointCollection = FormationActorComponent-> PointCollection;

	
	FVector ActorLocation = TeamCharacter->GetActorLocation();

	FormationActorComponent -> FormationCharacterMap.Add(0, TeamCharacter);
	for (int i = 1; i < PointCollection -> NumPoints; ++i)
	{
		USceneComponent* SceneComponent = PointCollection -> ImmutablePts[i];
		FVector RelativeLocation = SceneComponent-> GetRelativeLocation();
		if (!FormationCharacterMap.Contains(i))
		{
			continue;
		}

		TSubclassOf<ATeamCharacter> CharacterClass = FormationCharacterMap[i];

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		// 在指定位置生成角色
		ATeamCharacter* Character = GetWorld()->SpawnActor<ATeamCharacter>(
			CharacterClass,
			ActorLocation + RelativeLocation ,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (Character)
		{
			// 添加角色到Formation组件的槽位映射
			FormationActorComponent -> FormationCharacterMap.Add(i, Character);
            
			// 分配控制器
			if (i != 0)
			{
				// 创建AI控制器
				ATeamAIController* AIController = GetWorld()->SpawnActor<ATeamAIController>(
					TeamAIController,
					FVector::ZeroVector,
					FRotator::ZeroRotator
				);
				Character->PossessedBy(AIController);
				AIController->Possess(Character);
			}
		}
	}
}
